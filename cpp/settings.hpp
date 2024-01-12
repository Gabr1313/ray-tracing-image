#pragma once
#include <string>

#include "types.hpp"
#include "algebra.hpp"

class Settings {
   public:
	u64 width, height, n_threads, n_updates, max_bounces;
	Vec3 bg_col;
	// camera
	// objes
	Settings(u64 w, u64 h, u64 nt, u64 nu, u64 mb, Vec3 bg);
};

Settings read_input(std::string);
