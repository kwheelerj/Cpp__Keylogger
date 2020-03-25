#include <iostream>
#include <string>
#include <cassert>
#include <cmath>

const std::string &BASE64_CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
std::string base64_encode(const std::string &s);

void print_bits(int);

int main()
{
	// print_bits(65);
	// return 0;
	// std::string s = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
	std::string s = "ABC";
	std::string ret = base64_encode(s);
	std::cout << ret << std::endl;

	// assert(ret == "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlzIHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2YgdGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGludWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRoZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=");
	assert(ret == "QUJD");
	std::cout << (sizeof(int)) << std::endl;

	return 0;
}

std::string base64_encode(const std::string &s)
{
	std::cout << "base64_encode(...)" << std::endl;
	std::string ret;
	int val = 0;
	int bits = -6;
	const unsigned int b63 = 0x3F;

	for (const auto &c : s)
	{
		std::cout << c << std::endl;

		std::cout << "\t";
		std::cout << (val << 8) << std::endl;
		print_bits( (val << 8 ) );

		val = (val << 8) + c;
		std::cout << "\t";
		std::cout << val << std::endl;
		print_bits(val);

		bits += 8;
		std::cout << "\t";
		std::cout << bits << std::endl;

		while (bits >= 0)
		{
			std::cout << "here 0" << std::endl;
			std::cout << "\t";
			std::cout << (val >> bits) << std::endl;
			print_bits( (val >> bits) );
			std::cout << "\t";
			std::cout << ( (val >> bits) & b63 ) << std::endl;
			print_bits( ( (val >> bits) & b63 ) );
			ret.push_back(BASE64_CODES[(val >> bits) & b63]);
			bits -= 6;
		}
	}
	if (bits > -6)
	{
		std::cout << "here 1" << std::endl;
		std::cout << "\t";
		std::cout << (val << 8) << std::endl;
			std::cout << "\t";
		std::cout << ( (val << 8) >> (bits + 8) ) << std::endl;
			std::cout << "\t";
		std::cout << ( ( (val << 8) >> (bits + 8) ) & b63 ) << std::endl;
		ret.push_back(BASE64_CODES[( (val << 8) >> (bits + 8) ) & b63]);
	}

	while (ret.size() % 4)
	{
		std::cout << "here 2" << std::endl;
		ret.push_back('=');
	}

	return ret;
}


void print_bits(int i)
{
	std::cout << "print_bits(" << i << ")" << std::endl;
	int int_size = sizeof(int) * 8;
	int bits_array[int_size];
	for (int i = 0; i < int_size; ++i)
		bits_array[i] = 0;
	auto bit_index = int_size;

	while ( i > 0 )
	{
		while ( pow(2, bit_index) > i)
			--bit_index;
		i -= pow(2, bit_index);
		bits_array[int_size - 1 - bit_index] = 1;
	}

	for (int i : bits_array)
		std::cout << i;
	std::cout << std::endl;
}