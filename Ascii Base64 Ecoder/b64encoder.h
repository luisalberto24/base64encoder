#include "stdint.h"
#include <iostream>
#include <string>

#pragma once

using namespace std;

namespace base64
{
	static const char base64_encoding_matrix[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	class b64encoder
	{
		public:
			enum enum_container_type
			{
				none = 0,
				text = 1,
				file = 2
			};
			static std::string encode(std::string text);
			static std::string decode(std::string text);
		private:
			static const char equal_sign_literal = '=';
			static const char null_literal = '\x0';
		private:
			static size_t get_base64_encoding_buffer_size(size_t size);
			static bool is_valid_base64_string(std::string text);
			static uint8_t get_base64_decoded_index(uint8_t value);
	};
}


