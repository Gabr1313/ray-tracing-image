use crate::object::Intersectable;
use crate::ray::Ray;
use crate::utils::Float3;

// ax + by + cz + d = 0
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
    pub fn from_points(p1: Float3, p2: Float3, p3: Float3) -> Self {
        let a1 = p2.x - p1.x;
        let b1 = p2.y - p1.y;
        let c1 = p2.z - p1.z;
        let a2 = p3.x - p1.x;
        let b2 = p3.y - p1.y;
        let c2 = p3.z - p1.z;
        let a = b1 * c2 - b2 * c1;
        let b = a2 * c1 - a1 * c2;
        let c = a1 * b2 - b1 * a2;
        let d = -a * p1.x - b * p1.y - c * p1.z;
        return Self { a, b, c, d };
    }
}

impl Intersectable for Plane {
    fn intersect(&self, ray: &Ray) -> Option<f32> {
        let a = self.a * ray.dir.x + self.b * ray.dir.y + self.c * ray.dir.z;
        if a.abs() < 1e-6 {
            return None;
        }
        let b = self.a * ray.orig.x + self.b * ray.orig.y + self.c * ray.orig.z + self.d;
        let t = -b / a;
        if t < 0.0 {
            return None;
        }
        return Some(t);
    }
    fn normal(&self, ray: &Ray) -> Ray {
        let mut ray = Ray::new_norm(ray.orig, Float3::new(self.a, self.b, self.c));
        if ray.dir.dot(&ray.orig) > 0.0 {
            ray.dir.invert();
        }
        return ray;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::utils::Float3;
    #[test]
    fn ray_intersect_plane_1() {
        let plane = Plane::new(1.0, 1.0, 1.0, -1.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 5.0), Float3::new(-1.0, -2.0, -3.0));
        let t = plane.intersect(&ray).unwrap();
        let point = ray.orig + &(ray.dir * t);
        assert!(-1.34 < point.x && point.x < -1.32);
        assert!(-1.68 < point.y && point.y < -1.66);
        assert!(3.99 < point.z && point.z < 4.01);
    }

    #[test]
    fn ray_intersect_plane_opposite_direction() {
        let plane = Plane::new(1.0, 1.0, 1.0, -1.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 5.0), Float3::new(1.0, 2.0, 3.0));
        let intersection = plane.intersect(&ray);
        assert!(intersection.is_none());
    }

    #[test]
    fn ray_intersect_plane_parallel() {
        let plane = Plane::new(1.0, 1.0, 1.0, -1.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 5.0), Float3::new(1.0, -2.0, 1.0));
        let intersection = plane.intersect(&ray);
        assert!(intersection.is_none());
    }
}
