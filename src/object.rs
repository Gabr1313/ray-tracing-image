pub mod plane;
pub mod sphere;

pub use plane::*;
pub use sphere::*;

use crate::ray::Ray;
use crate::utils::Float3;

pub trait Intersectable {
    fn intersect(&self, ray: &Ray) -> Option<f32>;
    fn normal(&self, point: &Float3) -> Ray;
}

pub struct Object {
    pub shape: Box<dyn Intersectable>,
    pub color: Float3,
    pub emit_intensity: f32,
}

impl Object {
    pub fn new(shape: Box<dyn Intersectable>, color: Float3, emit_intensity: f32) -> Self {
        return Object {
            shape,
            color,
            emit_intensity,
        };
    }
}
