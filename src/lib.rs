pub mod camera;
pub mod input;
pub mod object;
pub mod ray;
pub mod utils;
use std::error::Error;
pub type Result<T> = std::result::Result<T, Box<dyn Error>>;

use input::Settings;
use ray::Ray;
use utils::Float3;

pub fn shoot_rays(settings: Settings) -> Vec<u8> {
    let camera = settings.camera;
    let objs = settings.objs;
    let bounces = settings.max_bounces;
    let ray_per_pixel = settings.ray_per_pixel;

    let mut pixels = Vec::with_capacity(camera.rays.len() * 3);
    for ray in camera.rays {
        let mut pix = Float3::new(0.0, 0.0, 0.0);
        for _ in 0..ray_per_pixel {
            let light = trace_ray(ray.clone(), &objs, bounces);
            pix.sum(&light);
        }
        pixels.push((pix.x * 256.0 / ray_per_pixel as f32) as u8);
        pixels.push((pix.y * 256.0 / ray_per_pixel as f32) as u8);
        pixels.push((pix.z * 256.0 / ray_per_pixel as f32) as u8);
    }

    return pixels;
}

fn trace_ray(mut ray: Ray, objs: &[object::Object], bounces: usize) -> Float3 {
    let mut color = Float3::new(1.0, 1.0, 1.0);
    let mut light = Float3::new(0.0, 0.0, 0.0);
    let mut nearest: Option<(f32, &object::Object, usize)> = None;
    let mut prec = usize::MAX;
    for _ in 0..=bounces {
        for (i, obj) in objs.iter().enumerate() {
            if let Some(t) = obj.shape.intersect(&ray) {
                if (nearest.is_none() || t < nearest.unwrap().0) && i != prec {
                    nearest = Some((t, obj, i));
                }
            }
        }
        if let Some((t, obj, i)) = nearest {
            prec = i;
            let norm = obj.shape.normal(&(ray.orig + ray.dir * t));
            ray = half_sphere_rng(norm);
            light.sum(&(obj.color * obj.emit_intensity * color));
            color.mul(&obj.color);
        } else {
            break;
        }
    }
    return light;
}

use rand_distr::{Distribution, Normal};
pub fn half_sphere_rng(norm: Ray) -> Ray {
    let mut rng = rand::thread_rng();
    let normal = Normal::new(0.0, 1.0).unwrap();
    let x = normal.sample(&mut rng);
    let y = normal.sample(&mut rng);
    let z = normal.sample(&mut rng);
    let mut dir = Float3::new(x, y, z);
    if dir.dot(&norm.dir) < 0.0 {
        dir.x = -dir.x;
        dir.y = -dir.y;
        dir.z = -dir.z;
    }
    return Ray::new(norm.orig, dir);
}
