pub mod algebra;
pub mod camera;
pub mod input;
pub mod object;
pub mod ray;
pub mod threadpool;
pub mod utils;

pub type Result<T> = std::result::Result<T, Box<dyn Error>>;

use input::Settings;
use ray::Ray;
use std::error::Error;
use std::fs::File;
use std::io::Write;
use std::io::{Seek, SeekFrom};
use std::sync::{Arc, Mutex};
use std::thread;
use threadpool::ThreadPool;
use utils::Float3;

struct ImageStatus {
    float3s: Vec<Float3>,
    u8s: Vec<u8>,
}

impl ImageStatus {
    fn new(total_pixel: usize) -> Self {
        let float3s = vec![Float3::new(0.0, 0.0, 0.0); total_pixel];
        let u8s = vec![0; total_pixel * 3];
        Self { float3s, u8s }
    }
}

pub fn shoot_and_draw(settings: Settings) -> Result<()> {
    let number_of_updates = settings.number_of_updates;
    let camera = &settings.camera.rays;
    let background = settings.background;
    let objs = Arc::new(settings.objs);
    let width = settings.width;
    let height = settings.height;
    let ray_per_pixel = camera[0].len();
    let n_threads = if settings.n_threads > 0 {
        settings.n_threads
    } else {
        thread::available_parallelism()?.get()
    };

    let header = format!("P6\n{} {}\n{}\n", width, height, 0xff);
    let header_len = header.len() as u64;
    let mut file = File::create("draw.ppm")?;
    file.write_all(header.as_bytes())?;

    let pool = ThreadPool::new(n_threads);
    let image_status = Arc::new(Mutex::new(ImageStatus::new(width * height)));
    for nou in 1..=number_of_updates {
        let to_multiply = 256.0 / (nou * ray_per_pixel) as f32;
        for (i, ray_group) in camera.iter().enumerate() {
            let objs = Arc::clone(&objs);
            let image_status = Arc::clone(&image_status);
            let ray_group = Arc::clone(ray_group);
            pool.execute(move || {
                let mut local_pix_sum = Float3::new(0.0, 0.0, 0.0);
                for ray in ray_group.iter() {
                    let light = trace_ray(&ray, &objs, settings.max_bounces, &background);
                    local_pix_sum.sum(&light);
                }
                let mut image_status = image_status.lock().unwrap();
                image_status.float3s[i].sum(&local_pix_sum);
                let total_pix_sum = image_status.float3s[i];
                let rgb = &mut image_status.u8s[i * 3..i * 3 + 3];
                rgb[0] = (total_pix_sum.x * to_multiply) as u8;
                rgb[1] = (total_pix_sum.y * to_multiply) as u8;
                rgb[2] = (total_pix_sum.z * to_multiply) as u8; 
            });
        }
        pool.wait();
        file.seek(SeekFrom::Start(header_len))?;
        file.write_all(&image_status.lock().unwrap().u8s)?;
    }

    Ok(())
}

fn trace_ray(ray: &Ray, objs: &[object::Object], bounces: usize, background: &Float3) -> Float3 {
    let mut ray = *ray;
    let mut color = Float3::new(1.0, 1.0, 1.0);
    let mut light = Float3::new(0.0, 0.0, 0.0);
    let mut prev = usize::MAX;
    for _ in 0..bounces {
        let mut nearest: Option<(f32, &object::Object)> = None;
        for obj in objs {
            if let Some(t) = obj.shape.intersect(&ray) {
                if (nearest.is_none() || t < nearest.unwrap().0) && prev != obj.id {
                    nearest = Some((t, obj));
                }
            }
        }
        if let Some((t, obj)) = nearest {
            prev = obj.id;
            obj.reflect(&mut ray, t);
            light.sum(&(obj.color * obj.emit_intensity * &color));
            color.mul(&obj.color);
        } else {
            light.sum(&(*background * &color));
            break;
        }
    }
    return light;
}
