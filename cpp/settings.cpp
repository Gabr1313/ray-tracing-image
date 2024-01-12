#include "settings.hpp"

#include <fstream>
#include <iostream>

#define READ_TO_U64(var, s)      \
	while (input_file >> s)      \
		if (s[0] != '_') break;  \
	std::cout << s << std::endl; \
	var = std::stoull(s);

#define READ_TO_F32(var, s)     \
	while (input_file >> s)     \
		if (s[0] != '_') break; \
	var = std::stof(s);

Settings::Settings(u64 w, u64 h, u64 nt, u64 nu, u64 mb, Vec3 bg)
		: width(w),
		  height(h),
		  n_threads(nt),
		  n_updates(nu),
		  max_bounces(mb),
		  bg_col(bg) {}

Settings read_input(std::string filename) {
	std::ifstream input_file(filename);
	if (!input_file.is_open()) {
		std::cerr << "Error opening file" << std::endl;
		exit(1);
	}
	u64 width, height, n_threads, n_updates, max_bounces;
	f32 col1, col2, col3;

	std::string s;

	READ_TO_U64(width, s);
	READ_TO_U64(height, s);
	READ_TO_U64(n_threads, s);
	READ_TO_U64(n_updates, s);
	READ_TO_U64(max_bounces, s);
	READ_TO_F32(col1, s);
	READ_TO_F32(col2, s);
	READ_TO_F32(col3, s);

	Settings settings(width, height, n_threads, n_updates, max_bounces,
					  Vec3(col1, col2, col3));
	return settings;
}
