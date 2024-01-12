use ray_tracing_image::input::read_input;
use ray_tracing_image::shoot_and_draw;
use ray_tracing_image::Result;

fn main() -> Result<()> {
    let settings = read_input("c/input.txt")?;
    shoot_and_draw(settings)?;
    Ok(())
}
