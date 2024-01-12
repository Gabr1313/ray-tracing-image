#include "draw.h"
#include "scanner.h"

int main(int argc, char *argv[]) {
	const InputData input_data = scan_input();
	shoot_and_draw(&input_data);
	return 0;
}
