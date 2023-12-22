pub mod algebra;
pub mod camera;
pub mod input;
pub mod object;
pub mod ray;
pub mod utils;
use std::error::Error;
pub type Result<T> = std::result::Result<T, Box<dyn Error>>;
use input::Settings;
use ray::Ray;
use utils::{Float3, EPS};

pub fn shoot_rays(settings: &Settings) -> Vec<f32> {
    let camera = &settings.camera;
    let objs = &settings.objs;
    let bounces = settings.max_bounces;
    let ray_per_pixel = settings.ray_per_pixel;

    let mut pixels = Vec::with_capacity(camera.rays.len() * 3);
    for ray in &camera.rays {
        let mut pix = Float3::new(0.0, 0.0, 0.0);
        for _ in 0..ray_per_pixel {
            let light = trace_ray(ray.clone(), &objs, bounces);
            pix.sum(&light);
        }
        pixels.push(pix.x / ray_per_pixel as f32);
        pixels.push(pix.y / ray_per_pixel as f32);
        pixels.push(pix.z / ray_per_pixel as f32);
    }

    return pixels;
}

fn trace_ray(mut ray: Ray, objs: &[object::Object], bounces: usize) -> Float3 {
    let mut color = Float3::new(1.0, 1.0, 1.0);
    let mut light = Float3::new(0.0, 0.0, 0.0);
    let mut b: usize = 0;
    while b <= bounces {
        b += 1;
        let mut nearest: Option<(f32, &object::Object)> = None;
        for obj in objs {
            if let Some(t) = obj.shape.intersect(&ray) {
                if (nearest.is_none() || t < nearest.unwrap().0) && t > EPS {
                    nearest = Some((t, obj));
                }
            }
        }
        if let Some((t, obj)) = nearest {
            if obj.reflect(&mut ray, t) == false {
                b -= 1;
                continue;
            }
            light.sum(&(obj.color * obj.emit_intensity * color));
            color.mul(&obj.color);
        } else {
            break;
        }
    }
    return light;
}
