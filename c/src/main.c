#include <stdlib.h>
#include "draw.h"
#include "scanner.h"

int main() {
	const InputData input_data = scan_input();
	shoot_and_draw(&input_data);
	free(input_data.objects.ptr);
	return 0;
}
