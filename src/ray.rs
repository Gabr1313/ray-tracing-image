use crate::utils::{Float3, Float2};

#[derive(Debug, Clone, Copy)]
pub struct Ray {
    pub orig: Float3,
    pub dir: Float3,
}

impl Ray {
    pub fn new(orig: Float3, dir: Float3) -> Self {
        return Ray { orig, dir };
    }
    pub fn new_norm(orig: Float3, mut dir: Float3) -> Self {
        dir.normalize();
        return Ray { orig, dir };
    }
    pub fn move_along(&mut self, t: f32) {
        self.orig = self.orig + self.dir * t;
    }
}

#[derive(Debug, Clone, Copy)]
pub struct Ray2 {
    pub orig: Float2,
    pub dir: Float2,
}

impl Ray2 {
    pub fn new(orig: Float2, dir: Float2) -> Self {
        return Ray2 { orig, dir };
    }
}
