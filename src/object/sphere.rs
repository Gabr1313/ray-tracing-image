use crate::object::Intersectable;
use crate::ray::Ray;
use crate::utils::Float3;

pub struct Sphere {
    pub center: Float3,
    pub radius: f32,
}

impl Sphere {
    pub fn new(center: Float3, radius: f32) -> Self {
        return Sphere { center, radius };
    }
}

impl Intersectable for Sphere {
    fn intersect(&self, ray: &Ray) -> Option<f32> {
        let oc = Float3 {
            x: ray.orig.x - self.center.x,
            y: ray.orig.y - self.center.y,
            z: ray.orig.z - self.center.z,
        };
        let a = ray.dir.x * ray.dir.x + ray.dir.y * ray.dir.y + ray.dir.z * ray.dir.z;
        let b = 2.0 * (oc.x * ray.dir.x + oc.y * ray.dir.y + oc.z * ray.dir.z);
        let c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - self.radius * self.radius;
        let discriminant = b * b - 4.0 * a * c;
        if discriminant < 0.0 {
            return None;
        }
        let sqrt_discriminant = discriminant.sqrt();
        if -b + sqrt_discriminant < 0.0 {
            return None;
        }
        let t = if -b - sqrt_discriminant > 0.0 {
            -b - sqrt_discriminant
        } else {
            -b + sqrt_discriminant
        } / (2.0 * a);
        return Some(t);
    }

    fn normal(&self, ray: &Ray) -> Ray {
        let mut ray = Ray::new_norm(ray.orig, ray.orig - &self.center);
        if ray.dir.dot(&ray.orig) > 0.0 {
            ray.orig.invert();
        }
        return ray;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ray_intersect_sphere_inside() {
        let sphere = Sphere::new(Float3::new(1.0, 2.0, 3.0), 4.0);
        let ray = Ray::new_norm(Float3::new(2.0, 3.0, 4.0), Float3::new(1.0, 1.0, 1.0));
        let t = sphere.intersect(&ray).unwrap();
        let point = ray.orig + &(ray.dir * t);
        assert!(1.30 < point.x && point.x < 3.32);
        assert!(4.30 < point.y && point.y < 4.32);
        assert!(5.30 < point.z && point.z < 5.32);
    }

    #[test]
    fn ray_intersect_sphere_outside() {
        let sphere = Sphere::new(Float3::new(1.0, 2.0, 3.0), 4.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 0.0), Float3::new(1.0, 2.0, 3.0));
        let t = sphere.intersect(&ray).unwrap();
        let point = ray.orig + &(ray.dir * t);
        assert!(-0.82 < point.x && point.x < -0.80);
        assert!(-0.63 < point.y && point.y < -0.61);
        assert!(0.56 < point.z && point.z < 0.58);
    }

    #[test]
    fn ray_intersect_sphere_opposite_direction() {
        let sphere = Sphere::new(Float3::new(1.0, 2.0, 3.0), 4.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 0.0), Float3::new(-1.0, -2.0, -3.0));
        let intersection = sphere.intersect(&ray);
        assert!(intersection.is_none());
    }

    #[test]
    fn ray_intersect_sphere_false() {
        let sphere = Sphere::new(Float3::new(1.0, 2.0, 3.0), 4.0);
        let ray = Ray::new_norm(Float3::new(-1.0, -1.0, 5.0), Float3::new(1.0, 2.0, 3.0));
        let intersection = sphere.intersect(&ray);
        assert!(intersection.is_none());
    }
}
