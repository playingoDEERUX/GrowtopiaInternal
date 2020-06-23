#pragma once
#include <string>

namespace utils {
	static std::string CreateRandomIdentifier() 
	{
		std::string pattern = "ABCDEF0123456789";
		std::string rid;
		for (int i = 0; i < 32; i++) 
		{
			rid += (char)(pattern[rand() % 16]);
		}
		rid[0] = '0';
		return rid;
	}
	static std::string RTHash(unsigned char* str, int len)
	{
		if (!str) return 0;

		unsigned char* n = (unsigned char*)str;
		uint32_t acc = 0x55555555;

		if (len == 0)
		{
			while (*n)
				acc = (acc >> 27) + (acc << 5) + *n++;
		}
		else
		{
			for (int i = 0; i < len; i++)
			{
				acc = (acc >> 27) + (acc << 5) + *n++;
			}
		}
		return std::to_string(acc);
	}
}