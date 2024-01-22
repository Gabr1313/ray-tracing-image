#include "draw.h"
#include "scanner.h"

int main() {
	InputData input_data = scan_input();
	shoot_and_draw(&input_data);
	free_input_data(&input_data);
	return 0;
}
