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

#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

namespace cherryhex {

std::vector<uint8_t> ascii_to_binary(const std::string &asciiHexString, bool bigEndian) {
	std::vector<uint8_t> ret;

	const size_t bufferSize = asciiHexString.size() / 2;

	ret.resize(bufferSize);

	for (size_t i = 0; i < asciiHexString.size(); i += 2) {
		const uint8_t iVal = std::stoul(asciiHexString.substr(i, 2), nullptr, 16);
		//std::cout << iVal << "\n";
		ret[i / 2] = iVal;
	}

	if (!bigEndian) {
		for (size_t i = 0; i < bufferSize; i += 4) {
			const uint8_t temp1 = ret[i];
			const uint8_t temp2 = ret[i + 1];

			ret[i + 0] = ret[i + 3];
			ret[i + 1] = ret[i + 2];

			ret[i + 2] = temp2;
			ret[i + 3] = temp1;
		}
	}

	return ret;
}

namespace util {

void remove_comments(std::string &str) {
	while (true) {
		const size_t pos = str.find_first_of('#');

		if (pos == str.npos) {
			break;
		}

		const size_t lfPos = str.find_first_of('\n', pos);

		str.erase(pos, lfPos - pos);
	}
}

void remove_spaces(std::string &str) {
	str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
}

std::string file_to_string(const std::string &fileName) {
	std::ifstream fin(fileName, std::ios::in);

	if (!fin) {
		return "";
	}

	std::stringstream ss;
	ss << fin.rdbuf();

	return ss.str();
}

}
}

