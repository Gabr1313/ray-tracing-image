#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	int width, height;
	char* input_file;
	char* output_file;
	int to_0_1 = 0;
	if (argc < 3 || argc > 4) {
		fprintf(stderr, "Usage: %s <input_file> <output_file> [0|1]\n",
				argv[0]);
		return 0;
	}
	if (argc == 4) to_0_1 = argv[3][0] == '1';
	input_file = argv[1];
	output_file = argv[2];

	FILE* file = fopen(input_file, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: can't open %s\n", input_file);
		exit(-1);
	}
	if (fscanf(file, "%*s %d %d %*f", &width, &height) != 2) {
		fprintf(stderr, "Error: can't read width and height\n");
		exit(-1);
	} else fgetc(file);

	const int size = width * height * 3;
	float* utility_buffer = malloc(size * sizeof(float));
	if (utility_buffer == NULL) {
		fprintf(stderr, "Error: can't allocate utility_buffer\n");
		exit(-1);
	}
	fread(utility_buffer, sizeof(float), size, file);
	fclose(file);

	unsigned char* buffer_char = malloc(size * sizeof(unsigned char));
	if (buffer_char == NULL) {
		fprintf(stderr, "Error: can't allocate buffer_char\n");
		exit(-1);
	}

	if (!to_0_1)
		for (int i = 0; i < size; i++)
			buffer_char[i] =
				(utility_buffer[i] > 1) ? 255 : utility_buffer[i] * 255;
	else
		for (int i = 0; i < size; i++)
			buffer_char[i] =
				((fabs(utility_buffer[i]) > 1) ? 127.5
											   : utility_buffer[i] * 127.5) +
				127.5;

	file = fopen(output_file, "wb");
	fprintf(file, "P6\n%d %d\n255\n", width, height);
	fwrite(buffer_char, sizeof(unsigned char), size, file);
	fclose(file);
	free(buffer_char);

	return 0;
}
