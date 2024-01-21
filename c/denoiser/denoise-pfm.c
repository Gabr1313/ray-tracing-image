#include <OpenImageDenoise/oidn.h>
#include <stdio.h>
#include <stdlib.h>

void set_filter_image(const OIDNFilter* filter, const OIDNDevice* device,
							const char* name, const char* filename,
							float* utility_buffer, const int size) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: can't open %s\n", filename);
		exit(-1);
	}
	int width, height;
	if (fscanf(file, "%*s %d %d %*f", &width, &height) != 2) {
		fprintf(stderr, "Error: can't read width and height\n");
		exit(-1);
	} else fgetc(file);
	if (width * height * 3 != size) {
		fprintf(stderr, "Error: size mismatch\n");
		exit(-1);
	}
	OIDNBuffer oidn_buf = oidnNewBuffer(*device, size * sizeof(float));
	if (oidn_buf == NULL) {
		fprintf(stderr, "Error: can't allocate color_buf\n");
		exit(-1);
	}
	fread(utility_buffer, sizeof(float), size, file);
	oidnWriteBuffer(oidn_buf, 0, size * sizeof(float), utility_buffer);
	fclose(file);
	oidnSetFilterImage(*filter, name, oidn_buf, OIDN_FORMAT_FLOAT3, width,
					   height, 0, 0, 0);
	oidnReleaseBuffer(oidn_buf);
}

void get_width_height(const char* filename, int* width, int* height) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: can't open %s\n", filename);
		exit(-1);
	}
	if (fscanf(file, "%*s %d %d", width, height) != 2) {
		fprintf(stderr, "Error: can't read width and height\n");
		exit(-1);
	}
	fclose(file);
}

int main() {
	int width, height;
	char* filename_color = "color.pfm";
	char* filename_albedo = "albedo.pfm";
	char* filename_normal = "normal.pfm";

	const OIDNDevice device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
	oidnCommitDevice(device);
	const OIDNFilter filter = oidnNewFilter(device, "RT");

	get_width_height(filename_color, &width, &height);
	const int size = width * height * 3;
	float* utility_buffer = malloc(size * sizeof(float));

	set_filter_image(&filter, &device, "color", filename_color, utility_buffer,
					 size);
	set_filter_image(&filter, &device, "albedo", filename_albedo,
					 utility_buffer, size);
	set_filter_image(&filter, &device, "normal", filename_normal,
					 utility_buffer, size);
	oidnNewBuffer(device, size * sizeof(float));

	OIDNBuffer output_buf = oidnNewBuffer(device, size * sizeof(float));
	if (output_buf == NULL) {
		fprintf(stderr, "Error: can't allocate output_buf\n");
		exit(-1);
	}
	oidnSetFilterImage(filter, "output", output_buf, OIDN_FORMAT_FLOAT3, width,
					   height, 0, 0, 0);
	oidnSetFilterBool(filter, "hdr", false); // i don't know
	oidnCommitFilter(filter);
	oidnExecuteFilter(filter);
	const char* errorMessage;
	if (oidnGetDeviceError(device, &errorMessage) != OIDN_ERROR_NONE) {
		fprintf(stderr, "Error: %s\n", errorMessage);
		exit(1);
	}
	oidnReleaseDevice(device);

	oidnReadBuffer(output_buf, 0, size * sizeof(float), utility_buffer);
	oidnReleaseBuffer(output_buf);

	unsigned char* buffer_char = malloc(size * sizeof(unsigned char));
	if (buffer_char == NULL) {
		fprintf(stderr, "Error: can't allocate buffer_char\n");
		exit(-1);
	}
	for (int i = 0; i < size; i++) {
		buffer_char[i] =
			(utility_buffer[i] > 1) ? 255 : utility_buffer[i] * 255;
	}

	fprintf(stdout, "P6\n%d %d\n255\n", width, height);
	fwrite(buffer_char, sizeof(unsigned char), size, stdout);
	free(buffer_char);

	return 0;
}
