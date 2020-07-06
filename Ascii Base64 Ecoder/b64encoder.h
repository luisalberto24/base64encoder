#include "stdint.h"
#include <iostream>
#include <string>

#ifndef _B64_ENCODER_
#define _B64_ENCODER_

#pragma once

#define UCHAR_T	unsigned char

using namespace std;

namespace base64
{
	static const char base64_encoding_matrix[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	class b64encoder
	{
		public:
			static std::string encode(std::string text);
			static std::string decode(std::string text);
		private:
			static const char equal_sign_literal = '=';
			static const char null_literal = '\x0';
		private:
			static size_t get_base64_encoding_buffer_size(size_t size);
			static bool is_valid_base64_string(std::string text);
			static UCHAR_T get_base64_decoded_index(UCHAR_T value);
	};
}

#endif 


