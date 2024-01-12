#include "settings.hpp"
#include <iostream>

int main() {
	Settings settings = read_input("input.txt");
	std::cout << settings.width << std::endl;
	return 0;
}
