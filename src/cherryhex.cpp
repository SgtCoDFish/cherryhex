#include <cstdlib>
#include <cstring>
#include <cstdint>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include <memory>

#include "cherryhex_version.hpp"

static std::string file_to_string(std::ifstream &fin);
static void remove_comments(std::string &str);
static void print_usage();

int main(int argc, char *argv[]) {
	if(argc != 2 || (std::strcmp(argv[1], "-v") == 0 || std::strcmp(argv[1], "--help") == 0)) {
		print_usage();
		return EXIT_SUCCESS;
	}

	const std::string fileName = argv[1];
	const std::string outFileName = fileName.substr(0, fileName.find_last_of('.')) + ".bin";

	std::string text;
	{
		std::ifstream fin(fileName, std::ios::in);

		if(!fin) {
			std::cerr << "Couldn't open " << fileName << std::endl;
			return EXIT_FAILURE;
		}
		
		text = file_to_string(fin);
	}
	
	remove_comments(text);
	text.erase(std::remove_if(text.begin(), text.end(), ::isspace), text.end());

	std::cout << text << std::endl;

	if(text.size() % 2 != 0) {
		std::cerr << "File " << fileName << " contains an invalid amount of characters." << std::endl;
		return EXIT_FAILURE;
	}
	
	const size_t bufferSize = text.size() / 2;
	auto outBuffer = std::make_unique<uint8_t[]>(bufferSize);

	for(size_t i = 0; i < text.size(); i += 2) {
		const uint8_t iVal = std::stoul(text.substr(i, 2), nullptr, 16);
		//std::cout << iVal << "\n";
		outBuffer[i/2] = iVal;
	}

	std::cout << "Writing " << bufferSize << " bytes to " << outFileName << std::endl;

	std::ofstream of(outFileName, std::ios::out | std::ios::binary | std::ios::trunc);
	of.write(reinterpret_cast<char*>(outBuffer.get()), bufferSize);

	return EXIT_SUCCESS;
}

void remove_comments(std::string &str) {
	while(true) {
		const size_t pos = str.find_first_of('#');

		if(pos == str.npos) {
			break;
		}

		std::cout << "pos = " << pos << std::endl;

		const size_t lfPos = str.find_first_of('\n', pos);

		str.erase(pos, lfPos - pos);
	}
}

std::string file_to_string(std::ifstream &fin) {
	std::stringstream ss;
	ss << fin.rdbuf();
	return ss.str();
}

void print_usage() {
	std::cout << "cherryhex version " << cherryhex::Version::versionString << "\n\n";
	
	std::cout << "Usage: cherryhex <file>\n\n";

	std::cout << "Converts a file containing ASCII hex into raw hexadecimal in a file."
			  << std::endl;
}
