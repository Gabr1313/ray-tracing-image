use crate::object::Intersectable;
use crate::object::Plane;
use crate::ray::{Ray, Ray2};
use crate::utils::{Float2, Float3};

#[derive(Debug)]
enum Projection {
    XY,
    YZ,
    ZX,
}

// ax + by + cz = d
#[derive(Debug)]
pub struct Triangle {
    plane: Plane,
    projection: Projection,
    r1: Ray2,
    r2: Ray2,
    r3: Ray2,
}

impl Triangle {
    pub fn new(p1: Float3, mut p2: Float3, mut p3: Float3) -> Self {
        let plane = Plane::from_points(p1, p2, p3);
        let (projection, r1, r2, r3);
        if plane.a.abs() > plane.b.abs() && plane.a.abs() > plane.c.abs() {
            projection = Projection::YZ;
            if Float2::new(p3.y - p1.y, p3.z - p1.z).cross(&Float2::new(p2.y - p1.y, p2.z - p1.z))
                < 0.0
            {
                std::mem::swap(&mut p2, &mut p3);
            }
            r1 = Ray2::new(
                Float2::new(p1.y, p1.z),
                Float2::new(p2.y - p1.y, p2.z - p1.z),
            );
            r2 = Ray2::new(
                Float2::new(p2.y, p2.z),
                Float2::new(p3.y - p2.y, p3.z - p2.z),
            );
            r3 = Ray2::new(
                Float2::new(p3.y, p3.z),
                Float2::new(p1.y - p3.y, p1.z - p3.z),
            );
        } else if plane.b.abs() > plane.a.abs() && plane.b.abs() > plane.c.abs() {
            projection = Projection::ZX;
            if Float2::new(p3.z - p1.z, p3.x - p1.x).cross(&Float2::new(p2.z - p1.z, p2.x - p1.x))
                < 0.0
            {
                std::mem::swap(&mut p2, &mut p3);
            }
            r1 = Ray2::new(
                Float2::new(p1.z, p1.x),
                Float2::new(p2.z - p1.z, p2.x - p1.x),
            );
            r2 = Ray2::new(
                Float2::new(p2.z, p2.x),
                Float2::new(p3.z - p2.z, p3.x - p2.x),
            );
            r3 = Ray2::new(
                Float2::new(p3.z, p3.x),
                Float2::new(p1.z - p3.z, p1.x - p3.x),
            );
        } else {
            projection = Projection::XY;
            if Float2::new(p3.x - p1.x, p3.y - p1.y).cross(&Float2::new(p2.x - p1.x, p2.y - p1.y))
                < 0.0
            {
                std::mem::swap(&mut p2, &mut p3);
            }
            r1 = Ray2::new(
                Float2::new(p1.x, p1.y),
                Float2::new(p2.x - p1.x, p2.y - p1.y),
            );
            r2 = Ray2::new(
                Float2::new(p2.x, p2.y),
                Float2::new(p3.x - p2.x, p3.y - p2.y),
            );
            r3 = Ray2::new(
                Float2::new(p3.x, p3.y),
                Float2::new(p1.x - p3.x, p1.y - p3.y),
            );
        };
        return Self {
            plane,
            projection,
            r1,
            r2,
            r3,
        };
    }
}

impl Intersectable for Triangle {
    fn intersect(&self, ray: &Ray) -> Option<f32> {
        if let Some(t) = self.plane.intersect(ray) {
            let point = ray.orig + &(ray.dir * t);
            let (d1, d2, d3) = (self.r1.dir, self.r2.dir, self.r3.dir);
            let (p1, p2, p3) = (self.r1.orig, self.r2.orig, self.r3.orig);
            let (cross1, cross2, cross3);
            match self.projection {
                Projection::XY => {
                    cross1 = d1.cross(&(Float2::new(point.x - p1.x, point.y - p1.y)));
                    cross2 = d2.cross(&(Float2::new(point.x - p2.x, point.y - p2.y)));
                    cross3 = d3.cross(&(Float2::new(point.x - p3.x, point.y - p3.y)));
                }
                Projection::YZ => {
                    cross1 = d1.cross(&(Float2::new(point.y - p1.x, point.z - p1.y)));
                    cross2 = d2.cross(&(Float2::new(point.y - p2.x, point.z - p2.y)));
                    cross3 = d3.cross(&(Float2::new(point.y - p3.x, point.z - p3.y)));
                }
                Projection::ZX => {
                    cross1 = d1.cross(&(Float2::new(point.z - p1.x, point.x - p1.y)));
                    cross2 = d2.cross(&(Float2::new(point.z - p2.x, point.x - p2.y)));
                    cross3 = d3.cross(&(Float2::new(point.z - p3.x, point.x - p3.y)));
                }
            };
            if cross1 < 0.0 && cross2 < 0.0 && cross3 < 0.0 {
                return Some(t);
            }
        }
        return None;
    }
    fn normal(&self, ray: &Ray) -> Ray {
        return self.plane.normal(ray);
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn ray_intersect_triangle() {
        let p1 = Float3::new(1.0, 2.0, 3.0);
        let p2 = Float3::new(4.0, 5.0, 6.0);
        let p3 = Float3::new(1.0, 7.0, 8.0);
        let triangle = Triangle::new(p1, p2, p3);
        let pos = Float3::new(2.0, 4.0, 3.0);
        let dir = Float3::new(0.0, -1.0, 2.0);
        let mut ray = Ray::new_norm(pos, dir);
        let t = triangle.intersect(&ray).unwrap();
        ray.move_along(t);
        assert!((ray.orig.x - 2.0).abs() < 1e-2);
        assert!((ray.orig.y - 3.33).abs() < 1e-2);
        assert!((ray.orig.z - 4.33).abs() < 1e-2);
    }

    #[test]
    fn ray_intersect_plane_opposite_direction() {
        let p1 = Float3::new(1.0, 2.0, 3.0);
        let p2 = Float3::new(4.0, 5.0, 6.0);
        let p3 = Float3::new(1.0, 7.0, 8.0);
        let triangle = Triangle::new(p1, p2, p3);
        let pos = Float3::new(2.0, 4.0, 3.0);
        let dir = Float3::new(0.0, 1.0, -2.0);
        let ray = Ray::new_norm(pos, dir);
        assert!(triangle.intersect(&ray).is_none());
    }

    #[test]
    fn ray_intersect_triangle_parallel() {
        let p1 = Float3::new(1.0, 2.0, 3.0);
        let p2 = Float3::new(4.0, 5.0, 6.0);
        let p3 = Float3::new(1.0, 7.0, 8.0);
        let triangle = Triangle::new(p1, p2, p3);
        let pos = Float3::new(8.0, 4.0, 3.0);
        let dir = Float3::new(0.0, -1.0, 2.0);
        let ray = Ray::new_norm(pos, dir);
        assert!(triangle.intersect(&ray).is_none());
    }
}

/*

(-4, 20, -1)
(5, 20, -2)
(0, 20, -5)

[src/object/triangle.rs:70] &r1 = Ray2 {
    orig: Float2 {
        x: -1.0,
        y: -4.0,
    },
    dir: Float2 {
        x: 9.0,
        y: 1.0,
    },
}
[src/object/triangle.rs:70] &r2 = Ray2 {
    orig: Float2 {
        x: -2.0,
        y: 5.0,
    },
    dir: Float2 {
        x: -5.0,
        y: 3.0,
    },
}
[src/object/triangle.rs:70] &r3 = Ray2 {
    orig: Float2 {
        x: -5.0,
        y: 0.0,
    },
    dir: Float2 {
        x: -4.0,
        y: -4.0,
    },
}



        (-4.0, 0.0, -1.0)
(        1.0, 0.0, 9.0)
(        5.0, 0.0, -2.0)
(        3.0, 0.0, -5.0)
(        0.0, 0.0, -5.0)
(        -4.0, 0.0, -4.0)
*/
