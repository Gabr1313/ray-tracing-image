#include <OpenImageDenoise/oidn.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int width, height;
	if (fscanf(stdin, "%*s %d %d %*f", &width, &height) != 2) {
		fprintf(stderr, "Error: can't read width and height\n");
		exit(-1);
	}
	fgetc(stdin);  // \n
	const int size = width * height * 3;
	float* buffer_float = malloc(size * sizeof(float));
	if (buffer_float == NULL) {
		fprintf(stderr, "Error: can't allocate buffer\n");
		exit(-1);
	}
	if (fread(buffer_float, sizeof(float), size, stdin) != (size_t)size) {
		fprintf(stderr, "Error: can't read buffer\n");
		exit(-1);
	}

	const OIDNDevice device = oidnNewDevice(OIDN_DEVICE_TYPE_DEFAULT);
	oidnCommitDevice(device);

	const OIDNBuffer colorBuf = oidnNewBuffer(device, size * sizeof(float));
	const OIDNBuffer outputBuf = oidnNewBuffer(device, size * sizeof(float));
	const OIDNFilter filter = oidnNewFilter(device, "RT");

	oidnSetFilterImage(filter, "color", colorBuf, OIDN_FORMAT_FLOAT3, width,
					   height, 0, 0, 0);
	oidnSetFilterImage(filter, "output", outputBuf, OIDN_FORMAT_FLOAT3, width,
					   height, 0, 0, 0);
	oidnSetFilterBool(filter, "hdr", true);
	oidnCommitFilter(filter);

	oidnWriteBuffer(colorBuf, 0, size * sizeof(float), buffer_float);
	oidnExecuteFilter(filter);
	const char* errorMessage;
	if (oidnGetDeviceError(device, &errorMessage) != OIDN_ERROR_NONE) {
		fprintf(stderr, "Error: %s\n", errorMessage);
		exit(1);
	}
	oidnReadBuffer(outputBuf, 0, size * sizeof(float), buffer_float);

	oidnReleaseBuffer(colorBuf);
	oidnReleaseFilter(filter);
	oidnReleaseDevice(device);

	fprintf(stdout, "P6\n%d %d\n255\n", width, height);
	unsigned char* buffer_char = malloc(size * sizeof(unsigned char));
	if (buffer_char == NULL) {
		fprintf(stderr, "Error: can't allocate buffer\n");
		exit(-1);
	}
	for (int i = 0; i < size; i++) {
		buffer_char[i] = (buffer_float[i] > 1) ? 255 : buffer_float[i] * 255;
	}
	fwrite(buffer_char, sizeof(unsigned char), size, stdout);
	fclose(stdout);

	return 0;
}
