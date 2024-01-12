pub mod plane;
pub mod sphere;
pub mod triangle;

pub use plane::*;
pub use sphere::*;
pub use triangle::*;

use crate::ray::Ray;
use crate::utils::Float3;
use rand::rngs::ThreadRng;
use rand::Rng;
use std::f32::consts::PI;

pub trait Intersectable {
    fn intersect(&self, ray: &Ray) -> Option<f32>;
    fn normal(&self, ray: &Ray) -> Ray;
}

pub struct Object {
    pub shape: Box<dyn Intersectable + Sync + Send>,
    pub color: Float3,
    pub emit_intensity: f32,
    pub reflection: f32,
    pub id: usize,
}

impl Object {
    pub fn new(
        shape: Box<dyn Intersectable + Sync + Send>,
        color: Float3,
        emit_intensity: f32,
        reflection: f32,
    ) -> Self {
        static mut ID: usize = 0;
        let id = unsafe {
            ID += 1;
            ID
        };
        return Object {
            shape,
            color,
            emit_intensity,
            reflection,
            id,
        };
    }

    pub fn reflect(&self, ray: &mut Ray, t: f32) {
        ray.move_along(t);
        let norm = self.shape.normal(&ray);
        let mut rng = rand::thread_rng();
        let flip = rng.gen_range(0.0..=1.0);
        if flip < self.reflection {
            ray.dir = ray.dir.reflect(&norm.dir);
        } else {
            ray.dir = half_sphere_random(norm, &mut rng);
        }
    }
}

fn half_sphere_random(norm: Ray, rng: &mut ThreadRng) -> Float3 {
    let phi = rng.gen_range(0.0..2.0 * PI);
    let theta = rng.gen_range(0.0..PI);
    let sin_theta = theta.sin();
    let mut dir = Float3::new(sin_theta * phi.cos(), sin_theta * phi.sin(), theta.cos());
    debug_assert!(norm.dir.norm() - 1.0 < 0.0001);
    debug_assert!(dir.norm() - 1.0 < 0.0001);
    if norm.dir.dot(&dir) < 0.0 {
        dir = dir.reflect(&norm.dir);
    }
    return dir;
}
