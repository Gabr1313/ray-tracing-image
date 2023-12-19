use std::ops::{Add, Div, Mul, Sub};

#[derive(Copy, Clone, Debug, PartialEq, Default)]
pub struct Float3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

#[derive(Debug)]
pub struct Mat3 {
    pub m: [[f32; 3]; 3],
}

impl Float3 {
    pub fn new(x: f32, y: f32, z: f32) -> Self {
        return Float3 { x, y, z };
    }
    pub fn dot(&self, rhs: &Self) -> f32 {
        return self.x * rhs.x + self.y * rhs.y + self.z * rhs.z;
    }
    pub fn norm(&self) -> f32 {
        return self.dot(self).sqrt();
    }
    pub fn normalize(&mut self) {
        let norm = self.norm();
        self.x /= norm;
        self.y /= norm;
        self.z /= norm;
    }
    pub fn maximize(&mut self, &rhs: &Self) {
        self.x = self.x.max(rhs.x);
        self.y = self.y.max(rhs.y);
        self.z = self.z.max(rhs.z);
    }
    pub fn cross(&self, rhs: &Self) -> Self {
        return Float3 {
            x: self.y * rhs.z - self.z * rhs.y,
            y: self.z * rhs.x - self.x * rhs.z,
            z: self.x * rhs.y - self.y * rhs.x,
        };
    }
    pub fn sum(&mut self, rhs: &Self) {
        self.x += rhs.x;
        self.y += rhs.y;
        self.z += rhs.z;
    }
    pub fn sub(&mut self, rhs: &Self) {
        self.x -= rhs.x;
        self.y -= rhs.y;
        self.z -= rhs.z;
    }
    pub fn mul(&mut self, rhs: &Self) {
        self.x *= rhs.x;
        self.y *= rhs.y;
        self.z *= rhs.z;
    }
    pub fn div(&mut self, rhs: &Self) {
        self.x /= rhs.x;
        self.y /= rhs.y;
        self.z /= rhs.z;
    }
}

impl Add for Float3 {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        return Float3 {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
            z: self.z + rhs.z,
        };
    }
}

impl Sub for Float3 {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        return Float3 {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
            z: self.z - rhs.z,
        };
    }
}

impl Mul<f32> for Float3 {
    type Output = Self;
    fn mul(self, rhs: f32) -> Self {
        return Float3 {
            x: self.x * rhs,
            y: self.y * rhs,
            z: self.z * rhs,
        };
    }
}

impl Mul<Float3> for Float3 {
    type Output = Self;
    fn mul(self, rhs: Float3) -> Self {
        return Float3 {
            x: self.x * rhs.x,
            y: self.y * rhs.y,
            z: self.z * rhs.z,
        };
    }
}

impl Div<f32> for Float3 {
    type Output = Self;
    fn div(self, rhs: f32) -> Self {
        return Float3 {
            x: self.x / rhs,
            y: self.y / rhs,
            z: self.z / rhs,
        };
    }
}

impl Mat3 {
    pub fn new(m: [[f32; 3]; 3]) -> Self {
        return Mat3 { m };
    }
    pub fn yaw(theta: f32) -> Self {
        return Mat3 {
            m: [
                [theta.cos(), 0.0, theta.sin()],
                [0.0, 1.0, 0.0],
                [-theta.sin(), 0.0, theta.cos()],
            ],
        };
    }
    pub fn pitch(theta: f32) -> Self {
        return Mat3 {
            m: [
                [1.0, 0.0, 0.0],
                [0.0, theta.cos(), -theta.sin()],
                [0.0, theta.sin(), theta.cos()],
            ],
        };
    }
    pub fn roll(theta: f32) -> Self {
        return Mat3 {
            m: [
                [theta.cos(), -theta.sin(), 0.0],
                [theta.sin(), theta.cos(), 0.0],
                [0.0, 0.0, 1.0],
            ],
        };
    }
    pub fn mul_vec(&self, rhs: &Float3) -> Float3 {
        return Float3 {
            x: self.m[0][0] * rhs.x + self.m[0][1] * rhs.y + self.m[0][2] * rhs.z,
            y: self.m[1][0] * rhs.x + self.m[1][1] * rhs.y + self.m[1][2] * rhs.z,
            z: self.m[2][0] * rhs.x + self.m[2][1] * rhs.y + self.m[2][2] * rhs.z,
        };
    }
}
