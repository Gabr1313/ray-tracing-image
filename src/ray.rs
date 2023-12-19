use crate::utils::Float3;

#[derive(Debug, Clone, Copy)]
pub struct Ray {
    pub orig: Float3,
    pub dir: Float3,
}

impl Ray {
    pub fn new(orig: Float3, dir: Float3) -> Self {
        return Ray { orig, dir };
    }
}
