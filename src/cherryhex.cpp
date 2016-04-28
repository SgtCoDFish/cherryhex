/*
 * Copyright (c) 2016 See AUTHORS file.
 * All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	
	std::cout << text << std::endl;

	if(text.size() % 2 != 0) {
		std::cerr << "File " << fileName << " contains an invalid amount of characters." << std::endl;
		return EXIT_FAILURE;
	}
	
	const size_t bufferSize = text.size() / 2;
	auto outBuffer = std::unique_ptr<uint8_t[]>(new uint8_t[bufferSize]);

	for(size_t i = 0; i < text.size(); i += 2) {
		const uint8_t iVal = std::stoul(text.substr(i, 2), nullptr, 16);
		//std::cout << iVal << "\n";
		outBuffer[i/2] = iVal;
	}

	std::cout << "Writing " << bufferSize << " bytes to " << outFileName << std::endl;

	std::ofstream of(outFileName, std::ios::out | std::ios::binary | std::ios::trunc);
	
	// write little endian
	for(size_t i = 0; i < bufferSize; i += 4) {
		for(int j = 3; j >= 0; --j) {
			of.write(reinterpret_cast<char*>(outBuffer.get() + (i+j)), 1);
		}
	}
	
	// of.write(reinterpret_cast<char*>(outBuffer.get()), bufferSize); // big endian

	return EXIT_SUCCESS;
}

void remove_comments(std::string &str) {
	while(true) {
		const size_t pos = str.find_first_of('#');

		if(pos == str.npos) {
			break;
		}

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
