#include <cstdlib>

#include <iostream>

#include "cherryhex_version.hpp"

int main(int argc, char *argv[]) {
	std::cout << "cherryhex version " << cherryhex::Version::versionString << std::endl;
	
	return EXIT_SUCCESS;
}