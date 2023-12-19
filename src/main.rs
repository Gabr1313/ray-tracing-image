use ray_tracing_image::input::read_input;
use ray_tracing_image::shoot_rays;
use ray_tracing_image::Result;
use std::fs::File;
use std::io::Write;

fn main() -> Result<()> {
    let settings = read_input("input.txt")?;
    let (width, height) = (settings.width, settings.height);
    let pixels = shoot_rays(settings);
    let header = format!("P6\n{} {}\n{}\n", width, height, 0xff);

    let mut file = File::create("draw.ppm")?;
    file.write_all(header.as_bytes())?;
    file.write_all(&pixels)?;

    Ok(())
}
