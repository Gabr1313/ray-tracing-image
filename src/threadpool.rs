use std::sync::{mpsc, Arc, Condvar, Mutex};
use std::thread;
pub struct ThreadPool {
    workers: Vec<Worker>,
    sender: Option<mpsc::Sender<Job>>,
    todos: Arc<(Mutex<usize>, Condvar)>,
}

type Job = Box<dyn FnOnce() + Send + 'static>;

impl ThreadPool {
    pub fn new(size: usize) -> Self {
        let (sender, receiver) = mpsc::channel();
        let receiver = Arc::new(Mutex::new(receiver));
        let mut workers = Vec::with_capacity(size);
        let todos = Arc::new((Mutex::new(0), Condvar::new()));
        for id in 0..size {
            workers.push(Worker::new(id, Arc::clone(&receiver), Arc::clone(&todos)));
        }
        ThreadPool {
            workers,
            sender: Some(sender),
            todos,
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
        let (lock, cvar) = &*self.todos;
        let _ = cvar.wait_while(lock.lock().unwrap(), |n| *n > 0);
    }

    pub fn add(&self, todos: usize) {
        *self.todos.0.lock().unwrap() += todos;
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
        todo: Arc<(Mutex<usize>, Condvar)>,
    ) -> Worker {
        let thread = thread::spawn(move || loop {
            let message = receiver.lock().unwrap().recv();
            match message {
                Ok(job) => job(),
                Err(_) => break,
            }
            sub_todo(&todo);
        });
        Worker {
            _id: id,
            thread: Some(thread),
        }
    }
}

#[inline]
fn sub_todo(todos: &Arc<(Mutex<usize>, Condvar)>) {
    let mut n = todos.0.lock().unwrap();
    *n -= 1;
    if *n == 0 {
        todos.1.notify_one();
    }
}
