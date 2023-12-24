use crate::camera::Camera;
use crate::object::{triangle, Object, Plane, Sphere};
use crate::ray::Ray;
use crate::utils::Float3;
use crate::Result;
use std::{io, str};

pub struct Scanner<R> {
    reader: R,
    buf_str: Vec<u8>,
    buf_iter: str::SplitAsciiWhitespace<'static>,
}

impl<R: io::BufRead> Scanner<R> {
    fn new(reader: R) -> Self {
        Self {
            reader,
            buf_str: vec![],
            buf_iter: "".split_ascii_whitespace(),
        }
    }
    fn tok<T: str::FromStr>(&mut self) -> T {
        loop {
            if let Some(token) = self.buf_iter.next() {
                if token.starts_with('_') {
                    continue;
                }
                return token.parse().ok().expect("Failed parse");
            }
            self.buf_str.clear();
            self.reader
                .read_until(b'\n', &mut self.buf_str)
                .expect("Failed read");
            self.buf_iter = unsafe {
                let slice = str::from_utf8_unchecked(&self.buf_str);
                std::mem::transmute(slice.split_ascii_whitespace())
            }
        }
    }
}

fn file_reader(filename: &str) -> io::Result<Scanner<io::BufReader<std::fs::File>>> {
    let file = std::fs::File::open(filename)?;
    Ok(Scanner::new(io::BufReader::new(file)))
}

pub struct Settings {
    pub width: usize,
    pub height: usize,
    pub n_threads: usize,
    pub number_of_updates: usize,
    pub camera: Camera,
    pub background: Float3,
    pub max_bounces: usize,
    pub objs: Vec<Object>,
}

pub fn read_input(filename: &str) -> Result<Settings> {
    let mut scan = file_reader(filename)?;

    let width = scan.tok();
    let height = scan.tok();

    let n_threads = scan.tok();
    let sqrt_ray_per_pixel = scan.tok();
    let number_of_update = scan.tok();

    let camera_pos = Float3::new(scan.tok(), scan.tok(), scan.tok());
    let camera_dir = Float3::new(scan.tok(), scan.tok(), scan.tok());
    let ray = Ray::new_norm(camera_pos, camera_dir);
    let camera_angle: f32 = scan.tok();
    let camera = Camera::new_rectangle(&ray, camera_angle, width, height, sqrt_ray_per_pixel);

    let max_bounces: usize = scan.tok();
    let background = Float3::new(scan.tok(), scan.tok(), scan.tok());

    let n: usize = scan.tok();

    let mut objs = Vec::with_capacity(n);
    for i in 0..n {
        match scan.tok::<String>().to_lowercase().as_str() {
            "sphere" => {
                let sphere =
                    Sphere::new(Float3::new(scan.tok(), scan.tok(), scan.tok()), scan.tok());
                let col = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let reflect = scan.tok();
                let intensity: f32 = scan.tok();
                objs.push(Object::new(Box::new(sphere), col, intensity, reflect));
            }
            "plane" => {
                let plane = Plane::new(scan.tok(), scan.tok(), scan.tok(), scan.tok());
                let col = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let reflect = scan.tok();
                let intensity: f32 = scan.tok();
                objs.push(Object::new(Box::new(plane), col, intensity, reflect));
            }
            "triangle" => {
                let p1 = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let p2 = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let p3 = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let triangle = triangle::Triangle::new(p1, p2, p3);
                let col = Float3::new(scan.tok(), scan.tok(), scan.tok());
                let reflect = scan.tok();
                let intensity: f32 = scan.tok();
                objs.push(Object::new(Box::new(triangle), col, intensity, reflect));
            }
            _ => {
                return Err(Box::new(io::Error::new(
                    io::ErrorKind::InvalidInput,
                    format!("Unknown object type at object {}", i),
                )));
            }
        }
    }
    return Ok(Settings {
        width,
        height,
        n_threads,
        camera,
        background,
        number_of_updates: number_of_update,
        max_bounces,
        objs,
    });
}
