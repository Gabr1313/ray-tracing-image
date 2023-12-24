use std::sync::{mpsc, Arc, Mutex};
use std::thread;

pub struct ThreadPool {
    workers: Vec<Worker>,
    sender: Option<mpsc::Sender<Job>>,
    todo: Arc<Mutex<usize>>,
    wait_receiver: mpsc::Receiver<()>,
    wait_sender: mpsc::Sender<()>,
}

type Job = Box<dyn FnOnce() + Send + 'static>;

impl ThreadPool {
    pub fn new(size: usize) -> Self {
        let (sender, receiver) = mpsc::channel();
        let receiver = Arc::new(Mutex::new(receiver));
        let mut workers = Vec::with_capacity(size);
        let todo = Arc::new(Mutex::new(0));
        let (wait_sender, wait_receiver) = mpsc::channel();
        for id in 0..size {
            workers.push(Worker::new(
                id,
                Arc::clone(&receiver),
                Arc::clone(&todo),
                wait_sender.clone(),
            ));
        }
        wait_sender.send(()).unwrap();
        ThreadPool {
            workers,
            sender: Some(sender),
            todo,
            wait_receiver,
            wait_sender,
        }
    }

    pub fn execute<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        add_todo(&self.todo, &self.wait_receiver);
        let job = Box::new(f);
        self.sender.as_ref().unwrap().send(job).unwrap();
    }

    pub fn wait(&self) {
        self.wait_receiver.recv().unwrap();
        self.wait_sender.send(()).unwrap();
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
        todo: Arc<Mutex<usize>>,
        wait_sender: mpsc::Sender<()>,
    ) -> Worker {
        let thread = thread::spawn(move || loop {
            let message = receiver.lock().unwrap().recv();
            match message {
                Ok(job) => job(),
                Err(_) => break,
            }
            sub_todo(&todo, &wait_sender);
        });
        Worker {
            _id: id,
            thread: Some(thread),
        }
    }
}

fn add_todo(todo: &Arc<Mutex<usize>>, wait_receiver: &mpsc::Receiver<()>) {
    let mut todo = todo.lock().unwrap();
    *todo += 1;
    if *todo == 1 {
        wait_receiver.recv().unwrap();
    }
}

fn sub_todo(todo: &Arc<Mutex<usize>>, wait_sender: &mpsc::Sender<()>) {
    let mut todo = todo.lock().unwrap();
    *todo -= 1;
    if *todo == 0 {
        wait_sender.send(()).unwrap();
    }
}
