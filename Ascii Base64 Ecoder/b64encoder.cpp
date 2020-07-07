#include "b64encoder.h"

namespace base64
{
	std::string b64encoder::encode(std::string text)
	{
		if (text.length() > 0)
		{
			size_t level = 0;
			size_t baseindex = 0;
			size_t cpos = 0;
			size_t data = 0;
			size_t size = text.size();
			size_t count = size;
			size_t allocate_len = get_base64_encoding_buffer_size(count);
			std::string dest(allocate_len, null_literal);
			while ((count / 3) > 0)
			{
				data = ((text[baseindex] << 16) & 0x00FF0000) |
					((text[baseindex + 1] << 8) & 0x0000FF00) |
					(text[baseindex + 2] & 0x000000FF);
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x00FC0000) >> 18)];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x0003F000) >> 12)];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x00000FC0) >> 6)];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x0000003F))];
				level += 1;
				baseindex = level * 3;
				count -= 3;
			}

			switch (count)
			{
			case 1:
				data = text[size - count];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0xFC) >> 2)];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)(data & 0x03) << 4];
				dest[cpos++] = equal_sign_literal;
				dest[cpos++] = equal_sign_literal;
				break;
			case 2:
				data = ((text[size - count] << 8) & 0x0000FF00) | (text[(size - count) + 1] & 0x000000FF);
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x0000FC00) >> 10)];
				dest[cpos++] = base64_encoding_matrix[(UCHAR_T)((data & 0x000003F0) >> 4)];
				data = data & 0x0000000F;
				dest[cpos++] = data > 0 ? base64_encoding_matrix[(UCHAR_T)(data << 2)] : equal_sign_literal;
				dest[cpos++] = equal_sign_literal;
				break;
			}

			return dest;
		}

		return "";
	}

	std::string b64encoder::decode(std::string text)
	{
		size_t count = text.length();
		if (count > 0 && is_valid_base64_string(text))
		{
			size_t level = 0;
			size_t baseindex = 0;
			uint32_t data = 0;
			std::string result((count / 4) * 3, null_literal);
			size_t cpos = 0;
			count -= 4;
			while ((count / 4) > 0)
			{
				data = ((get_base64_decoded_index(text[baseindex]) << 18) & 0x00FC0000);
				data |= ((get_base64_decoded_index(text[baseindex + 1]) << 12) & 0x0003F000);
				data |= ((get_base64_decoded_index(text[baseindex + 2]) << 6) & 0x00000FC0);
				data |= (get_base64_decoded_index(text[baseindex + 3]) & 0x0000003F);

				result[cpos++] = ((((data & 0x00FF0000) >> 16) & 0xFF) - null_literal);
				result[cpos++] = ((((data & 0x0000FF00) >> 8) & 0xFF) - null_literal);
				result[cpos++] = (((data & 0x000000FF) & 0xFF) - null_literal);

				level += 1;
				baseindex = level * 4;
				count -= 4;
			}

			data = ((get_base64_decoded_index(text[baseindex]) << 18) & 0x00FC0000);
			data |= ((get_base64_decoded_index(text[baseindex + 1]) << 12) & 0x0003F000);
			result[cpos++] = ((((data & 0x00FF0000) >> 16) & 0xFF) - null_literal);
			size_t pop_back = 0;
			if (text[baseindex + 2] != equal_sign_literal)
			{
				data |= ((get_base64_decoded_index(text[baseindex + 2]) << 6) & 0x00000FC0);
				result[cpos++] = ((((data & 0x0000FF00) >> 8) & 0xFF) - null_literal);
			}
			else pop_back++;

			if (text[baseindex + 3] != equal_sign_literal)
			{
				data |= (get_base64_decoded_index(text[baseindex + 3]) & 0x0000003F);
				result[cpos++] = (((data & 0x000000FF) & 0xFF) - null_literal);
			}
			else pop_back++;

			if (pop_back > 0)
			{
				result.resize(result.size() - pop_back);
			}

			return result;
		}

		return "";
	}

	size_t b64encoder::get_base64_encoding_buffer_size(size_t size)
	{
		size_t slots = size / 3;
		size_t remainder = size % 3;
		return (slots * 4) + (remainder > 0 ? 4 : 0);
	}

	bool b64encoder::is_valid_base64_string(std::string text)
	{
		size_t text_length = text.length();
		if ((text_length % 4) == 0)
		{
			size_t substract = text[text_length - 1] == equal_sign_literal ? 1 : 0;
			if (substract > 0 && text[text_length - 2] == equal_sign_literal)
			{
				substract++;
			}
			size_t length = text_length - substract;
			for (size_t i = 0; i < length - 1; i++)
			{
				if (!(isalpha(text[i]) || isdigit(text[i])) && text[i] != '+' && text[i] != '/')
				{
					return false;
				}
			}

			return true;
		}

		return false;
	}

	UCHAR_T b64encoder::get_base64_decoded_index(UCHAR_T value)
	{
		if (value >= 'A' && value <= 'Z') return value - 65;
		if (value >= 'a' && value <= 'z') return (value - 97) + 26;
		if (value >= '0' && value <= '9') return (value - 48) + 52;
		if (value == '/') return 62;
		
		return 63;
	}
}