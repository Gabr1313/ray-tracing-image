use std::sync::{mpsc, Arc, Mutex};
use std::thread;

pub struct ThreadPool {
    workers: Vec<Worker>,
    sender: Option<mpsc::Sender<Job>>,
    waiter: mpsc::Receiver<()>,
    todos: Arc<Mutex<usize>>
}

type Job = Box<dyn FnOnce() + Send + 'static>;

impl ThreadPool {
    pub fn new(size: usize) -> Self {
        let (sender, receiver) = mpsc::channel();
        let receiver = Arc::new(Mutex::new(receiver));
        let mut workers = Vec::with_capacity(size);
        let (interrupt, waiter) = mpsc::channel();
        let todos = Arc::new(Mutex::new(0));
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver), Arc::clone(&todos), interrupt.clone()));
        }
        ThreadPool {
            workers,
            sender: Some(sender),
            waiter,
            todos
        }
    }

    pub fn execute<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        let job = Box::new(f);
        self.sender.as_ref().unwrap().send(job).unwrap();
    }

    pub fn wait(&self) {
        self.waiter.recv().unwrap();
    }

    pub fn add(&self, todos: usize) {
        *self.todos.lock().unwrap() += todos;
    }
}

impl Drop for ThreadPool {
    fn drop(&mut self) {
        drop(self.sender.take());
        for worker in &mut self.workers {
            if let Some(thread) = worker.thread.take() {
                thread.join().unwrap();
            }
        }
    }
}

struct Worker {
    _id: usize,
    thread: Option<thread::JoinHandle<()>>,
}

impl Worker {
    fn new(
        id: usize,
        receiver: Arc<Mutex<mpsc::Receiver<Job>>>,
        todos: Arc<Mutex<usize>>,
        interrupt: mpsc::Sender<()>
    ) -> Worker {
        let thread = thread::spawn(move || loop {
            let message = receiver.lock().unwrap().recv();
            match message {
                Ok(job) => job(),
                Err(_) => break,
            }
            sub_todo(&todos,&interrupt);
        });
        Worker {
            _id: id,
            thread: Some(thread),
        }
    }
}

#[inline]
fn sub_todo(todos: &Arc<Mutex<usize>>, interrupt: &mpsc::Sender<()>) {
    let mut n = todos.lock().unwrap();
    *n -= 1;
    if *n == 0 {
        interrupt.send(()).unwrap();
    }
}
