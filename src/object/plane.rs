use crate::object::Intersectable;
use crate::ray::Ray;
use crate::utils::Float3;

// ax + by + cz = d
#[derive(Debug)]
pub struct Plane {
    pub a: f32,
    pub b: f32,
    pub c: f32,
    pub d: f32,
}

impl Plane {
    pub fn new(a: f32, b: f32, c: f32, d: f32) -> Self {
        return Self { a, b, c, d };
    }
}

impl Intersectable for Plane {
    fn intersect(&self, ray: &Ray) -> Option<f32> {
        let a = self.a * ray.dir.x + self.b * ray.dir.y + self.c * ray.dir.z;
        if a.abs() < 1e-10 {
            return None;
        }
        let b = self.a * ray.orig.x + self.b * ray.orig.y + self.c * ray.orig.z - self.d;
        let t = -b / a;
        if t < 0.0 {
            return None;
        }
        return Some(t);
    }
    fn normal(&self, point: &Float3) -> Ray {
        // assert!(
        //     (self.a * point.x + self.b * point.y + self.c * point.z - self.d).abs() < 1e-10
        // );
        return Ray::new(*point, Float3::new(self.a, self.b, self.c));
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::utils::Float3;
    #[test]
    fn ray_intersect_plane_1() {
        let plane = Plane::new(1.0, 1.0, 1.0, 1.0);
        let ray = Ray::new(Float3::new(-1.0, -1.0, 5.0), Float3::new(-1.0, -2.0, -3.0));
        let intersection = plane.intersect(&ray);
        assert!(intersection.is_some());
        let t = intersection.unwrap();
        let point = ray.orig + ray.dir * t;
        assert!(-1.34 < point.x && point.x < -1.32);
        assert!(-1.68 < point.y && point.y < -1.66);
        assert!(3.99 < point.z && point.z < 4.01);
    }

    #[test]
    fn ray_intersect_plane_opposite_direction() {
        let plane = Plane::new(1.0, 1.0, 1.0, 1.0);
        let ray = Ray::new(Float3::new(-1.0, -1.0, 5.0), Float3::new(1.0, 2.0, 3.0));
        let intersection = plane.intersect(&ray);
        assert!(intersection.is_none());
    }

    #[test]
    fn ray_intersect_plane_parallel() {
        let plane = Plane::new(1.0, 1.0, 1.0, 1.0);
        let ray = Ray::new(Float3::new(-1.0, -1.0, 5.0), Float3::new(1.0, -2.0, 1.0));
        let intersection = plane.intersect(&ray);
        assert!(intersection.is_none());
    }
}
