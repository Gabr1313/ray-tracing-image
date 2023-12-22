pub mod plane;
pub mod sphere;
pub mod triangle;

pub use plane::*;
pub use sphere::*;
pub use triangle::*;

use crate::ray::Ray;
use crate::utils::{Float3, EPS};
use rand::rngs::ThreadRng;
use rand::Rng;
use std::f32::consts::PI;

pub trait Intersectable {
    fn intersect(&self, ray: &Ray) -> Option<f32>;
    fn normal(&self, point: &Float3, start_point: &Float3) -> Ray;
}

pub struct Object {
    pub shape: Box<dyn Intersectable>,
    pub color: Float3,
    pub emit_intensity: f32,
    pub reflection: f32,
}

impl Object {
    pub fn new(
        shape: Box<dyn Intersectable>,
        color: Float3,
        emit_intensity: f32,
        reflection: f32,
    ) -> Self {
        return Object {
            shape,
            color,
            emit_intensity,
            reflection,
        };
    }

    pub fn reflect(&self, ray: &mut Ray, mut t: f32) -> bool {
        // avoid infinte loop... you never know
        let start_point = ray.orig;
        for _ in 0..10 {
            debug_assert!(t >= 0.0);
            ray.move_along(t * (1.0 - EPS));
            if t < EPS {
                break;
            }
            if let Some(t_new) = self.shape.intersect(&ray) {
                t = t_new;
            } else {
                // ray tangent not intersecting the object anymore
                return false;
            }
        }

        let norm = self.shape.normal(&ray.orig, &start_point);
        let mut rng = rand::thread_rng();
        let flip = rng.gen_range(0.0..=1.0);
        if flip < self.reflection {
            ray.dir = ray.dir.reflect(&norm.dir);
        } else {
            ray.dir = half_sphere_rng(norm, &mut rng);
        }
        return true;
    }
}

fn half_sphere_rng(norm: Ray, rng: &mut ThreadRng) -> Float3 {
    let phi = rng.gen_range(0.0..2.0 * PI);
    let theta = rng.gen_range(0.0..PI);
    let sin_theta = theta.sin();
    let x = sin_theta * phi.cos();
    let y = sin_theta * phi.sin();
    let z = theta.cos();
    let mut dir = Float3::new(x, y, z);
    if dir.dot(&norm.dir) < 0.0 {
        dir.invert();
    }
    return dir;
}
