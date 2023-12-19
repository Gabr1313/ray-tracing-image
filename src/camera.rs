use std::f32::consts::PI;

use crate::object::{Intersectable, Plane};
use crate::ray::Ray;
use crate::utils::Float3;
use crate::utils::Mat3;

#[derive(Debug)]
pub struct Camera {
    pub rays: Vec<Ray>,
}

impl Camera {
    pub fn new_rectangle(ray: &Ray, alpha: f32, n_rays_x: usize, n_rays_y: usize) -> Self {
        assert!(0.0 < alpha && alpha < PI);
        assert!(n_rays_x > 1 && n_rays_y > 1);

        let ratio = n_rays_x as f32 / n_rays_y as f32;
        let (ul, ur, _, dl) = screen_corners(&ray, alpha, ratio);

        let mut rays = Vec::with_capacity(n_rays_y * n_rays_x);
        let dy = (dl - ul) / (n_rays_y - 1) as f32;
        let dx = (ur - ul) / (n_rays_x - 1) as f32;
        for i in 0..n_rays_y {
            for j in 0..n_rays_x {
                let point = ul + dy * i as f32 + dx * j as f32;
                rays.push(Ray::new(ray.orig, point));
            }
        }

        return Camera { rays };
    }
}

fn screen_corners(ray: &Ray, alpha: f32, ratio: f32) -> (Float3, Float3, Float3, Float3) {
    assert!(ratio > 0.0);

    let alpha_2 = alpha / 2.0;
    let base = Float3::new(1.0, 0.0, 0.0);

    let (x, y, z);
    if ratio < 1.0 {
        let yawed = Mat3::yaw(alpha_2).mul_vec(&base);
        x = yawed.x;
        z = yawed.z;
        y = z * ratio;
    } else {
        let rolled = Mat3::roll(alpha_2).mul_vec(&base);
        x = rolled.x;
        y = rolled.y;
        z = y / ratio;
    }

    let base_ul = Float3::new(x, -y, z);
    let base_ur = Float3::new(x, y, z);
    let base_dl = Float3::new(x, -y, -z);
    let base_dr = Float3::new(x, y, -z);

    // dbg!(base_ul, base_ur, base_dl, base_dr);
    let dir = ray.dir;
    let pos = ray.orig;

    let mut dir1 = dir;
    dir1.normalize();
    let mut dir2 = Float3::new(0.0, 0.0, 1.0).cross(&dir);
    dir2.normalize();
    let mut dir3 = dir1.cross(&dir2);
    dir3.normalize();

    // dbg!(dir, dir2, dir3);

    let change = Mat3::new([
        [dir1.x, dir2.x, dir3.x],
        [dir1.y, dir2.y, dir3.y],
        [dir1.z, dir2.z, dir3.z],
    ]);

    let ray_ul = Ray::new(pos, change.mul_vec(&base_ul));
    let ray_ur = Ray::new(pos, change.mul_vec(&base_ur));
    let ray_dr = Ray::new(pos, change.mul_vec(&base_dr));
    let ray_dl = Ray::new(pos, change.mul_vec(&base_dl));

    // dbg!(&ray_ul, &ray_ur, &ray_dl, &ray_dr);

    let plane = Plane::new(dir.x, dir.y, dir.z, (pos + dir).dot(&dir));

    let ul = ray_ul.orig + ray_ul.dir * plane.intersect(&ray_ul).unwrap();
    let ur = ray_ur.orig + ray_ur.dir * plane.intersect(&ray_ur).unwrap();
    let dr = ray_dr.orig + ray_dr.dir * plane.intersect(&ray_dr).unwrap();
    let dl = ray_dl.orig + ray_dl.dir * plane.intersect(&ray_dl).unwrap();

    return (ul, ur, dr, dl);
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_screen_corners() {
        let ray = Ray::new(Float3::new(0.0, 0.0, 0.0), Float3::new(1.0, 1.0, 1.0));
        let alpha = 90f32.to_radians();
        let ratio = 2.0;

        let (ul, ur, dr, dl) = screen_corners(&ray, alpha, ratio);

        dbg!(ul, ur, dr, dl);

        assert!((ul.x - 1.87).abs() < 1e-2);
        assert!((ul.y + 0.58).abs() < 1e-2);
        assert!((ul.z - 1.71).abs() < 1e-2);

        assert!((ur.x + 0.58).abs() < 1e-2);
        assert!((ur.y - 1.87).abs() < 1e-2);
        assert!((ur.z - 1.71).abs() < 1e-2);

        assert!((dr.x - 0.13).abs() < 1e-2);
        assert!((dr.y - 2.58).abs() < 1e-2);
        assert!((dr.z - 0.29).abs() < 1e-2);

        assert!((dl.x - 2.58).abs() < 1e-2);
        assert!((dl.y - 0.13).abs() < 1e-2);
        assert!((dl.z - 0.29).abs() < 1e-2);
    }
}
