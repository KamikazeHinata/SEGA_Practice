#include "Tool.h"
#include <iostream>
#include <fstream>

namespace GameLib
{
	unsigned GetUnsigned(char* p)
	{
		unsigned char* up = reinterpret_cast<unsigned char*>(p);
		unsigned res = 0;

		res |= *up;
		res |= (*(up + 1) << 8);
		res |= (*(up + 2) << 16);
		res |= (*(up + 3) << 24);

		return res;
	}

	char* ReadFile(const char* filename)
	{
		std::cout << "file: " << filename << std::endl;
		std::ifstream fs(filename, std::ifstream::binary);

		fs.seekg(0, std::fstream::end);
		int fsize = static_cast<int>(fs.tellg());
		char* content = new char[fsize + 1];
		fs.seekg(0, std::fstream::beg);
		fs.read(content, fsize);
		content[fsize] = '\0';

		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!fsize: " << fsize;

		return content;
	}
}