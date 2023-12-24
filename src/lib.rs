pub mod algebra;
pub mod camera;
pub mod input;
pub mod object;
pub mod ray;
pub mod utils;
pub mod threadpool;

pub type Result<T> = std::result::Result<T, Box<dyn Error>>;

use input::Settings;
use ray::Ray;
use std::error::Error;
use std::fs::File;
use std::io::Write;
use std::io::{Seek, SeekFrom};
use utils::Float3;
use object::Object;
use std::sync::{Arc, Mutex};
use std::thread;

// TODO: use the threadpool to shoot 1 ray at a time
// after shooting all the rays, draw the image
// repeat

pub fn shoot_and_draw(settings: Settings) -> Result<()> {
    let (width, height) = (settings.width, settings.height);
    let total_pixel = width * height;
    let number_of_update = settings.number_of_update;
    let n_threads = if settings.n_threads > 0 {
        settings.n_threads
    } else {
        thread::available_parallelism().unwrap().get()
    };

    let pixel_per_thread = total_pixel / n_threads;
    let carry = total_pixel % n_threads;

    let mut rays_groups = Vec::with_capacity(n_threads);
    let mut pixels_sum_groups = Vec::with_capacity(n_threads);

    let mut rays_tail = settings.camera.rays.as_slice();
    for i in 0..n_threads {
        let extra = if i < carry { 1 } else { 0 };
        let (head, tail) = rays_tail.split_at(pixel_per_thread + extra);
        rays_groups.push(Arc::new(Mutex::new(head.to_vec())));
        rays_tail = tail;
        pixels_sum_groups.push(Arc::new(Mutex::new(vec![
            Float3::new(0.0, 0.0, 0.0);
            pixel_per_thread + extra
        ])));
    }

    let ray_per_update = settings.ray_per_update;
    let objs = Arc::new(settings.objs);
    let max_bounces = settings.max_bounces;
    let background = settings.background;

    let header = format!("P6\n{} {}\n{}\n", width, height, 0xff);
    let header_len = header.len() as u64;

    for nou in 1..=number_of_update {
        let mut handles = Vec::with_capacity(n_threads);
        for (rays, pixels_sum) in rays_groups.iter().zip(pixels_sum_groups.iter()) {
            let rays = Arc::clone(rays);
            let pixels_sum = Arc::clone(pixels_sum);
            let objs = Arc::clone(&objs);
            let handle = thread::spawn(move || {
                shoot_rays(
                    rays.lock().unwrap().as_mut(),
                    ray_per_update,
                    objs.as_ref(),
                    max_bounces,
                    background,
                    pixels_sum.lock().unwrap().as_mut(),
                );
            });
            handles.push(handle);
        }
        for handle in handles {
            handle.join().unwrap();
        }

        let mut content = Vec::with_capacity(3 * total_pixel);
        for pixels_sum in &pixels_sum_groups {
            let pixels_sum = pixels_sum.lock().unwrap();
            for pix in pixels_sum.iter() {
                content.push((pix.x / (ray_per_update * nou) as f32 * 256.0) as u8);
                content.push((pix.y / (ray_per_update * nou) as f32 * 256.0) as u8);
                content.push((pix.z / (ray_per_update * nou) as f32 * 256.0) as u8);
            }
        }

        let mut file = File::create("draw.ppm")?;
        file.write_all(header.as_bytes())?;
        file.seek(SeekFrom::Start(header_len))?;
        file.write_all(&content)?;
    }

    return Ok(());
}

pub fn shoot_rays(
    rays: &[Ray],
    ray_per_pixel: usize,
    objs: &[Object],
    bounces: usize,
    background: Float3,
    pixels: &mut [Float3],
) {
    for (i, ray) in rays.iter().enumerate() {
        for _ in 0..ray_per_pixel {
            let light = trace_ray(*ray, &objs, bounces, background);
            pixels[i].sum(&light);
        }
    }
}

fn trace_ray(mut ray: Ray, objs: &[object::Object], bounces: usize, background: Float3) -> Float3 {
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
            light.sum(&(background * &color));
            break;
        }
    }
    return light;
}
