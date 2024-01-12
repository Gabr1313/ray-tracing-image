#include "draw.h"
#include "scanner.h"

int main(void) {
	InputData input_data = scan_input("input.txt");
	shoot_and_draw(&input_data, "draw.ppm");
	return 0;
}
