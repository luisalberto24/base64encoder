#include "operate_bits.h"
#include "b64encoder.h"

namespace base64
{
	std::string b64encoder::encode(std::string text)
	{
		if (text.length() > 0)
		{
			size_t baseindex = 0;
			size_t cpos = 0;
			size_t data = 0;
			size_t size = text.size();
			int count = size / 3;
			size_t allocate_len = get_base64_encoding_buffer_size(size);
			std::string dest(allocate_len, null_literal);
			while (count > 0)
			{
				data  = bits_operation<0x00FF0000>::op_and(shift_bits<0x10>::left(text[baseindex    ]));
				data |= bits_operation<0x0000FF00>::op_and(shift_bits<0x08>::left(text[baseindex + 1]));
				data |= bits_operation<0x000000FF>::op_and(text[baseindex + 2]);

				dest[cpos++] = base64_encoding_matrix[shift_bits<0x12>::right(bits_operation<0x00FC0000>::op_and(data))];
				dest[cpos++] = base64_encoding_matrix[shift_bits<0x0C>::right(bits_operation<0x0003F000>::op_and(data))];
				dest[cpos++] = base64_encoding_matrix[shift_bits<0x06>::right(bits_operation<0x00000FC0>::op_and(data))];
				dest[cpos++] = base64_encoding_matrix[bits_operation<0x0000003F>::op_and(data)];

				baseindex += 3;
				count--;
			}

			count = size % 3;
			switch (count)
			{
				case 1:
					data = text[size - 1];
					dest[cpos++] = base64_encoding_matrix[shift_bits<0x02>::right(bits_operation<0xFC>::op_and(data))];
					dest[cpos++] = base64_encoding_matrix[shift_bits<0x04>::left(bits_operation<0x03>::op_and(data))];
					dest[cpos++] = equal_sign_literal;
					dest[cpos] = equal_sign_literal;
					break;
				case 2:
					data =	bits_operation<0x0000FF00>::op_and(shift_bits<8>::left(text[size - 2])) | 
							bits_operation<0x000000FF>::op_and(text[size - 1]);
					dest[cpos++] = base64_encoding_matrix[shift_bits<0x0A>::right(bits_operation<0x0000FC00>::op_and(data))];
					dest[cpos++] = base64_encoding_matrix[shift_bits<0x04>::right(bits_operation<0x000003F0>::op_and(data))];
					data = data & 0x0000000F;
					dest[cpos++] = data > 0 ? base64_encoding_matrix[(uint8_t)(data << 2)] : equal_sign_literal;
					dest[cpos] = equal_sign_literal;
					break;
			}

			return dest;
		}

		return "";
	}

	std::string b64encoder::decode(std::string text)
	{
		int count = text.length();
		if (count > 0 && is_valid_base64_string(text))
		{
			size_t baseindex = 0;
			uint32_t data = 0;
			count = (count / 4);
			std::string result(count * 3, null_literal);
			size_t cpos = 0;
			while (count > 1)
			{
				data =	bits_operation<0x00FC0000>::op_and(shift_bits<0x12>::left(get_base64_decoded_index(text[baseindex	 ])));
				data |= bits_operation<0x0003F000>::op_and(shift_bits<0x0C>::left(get_base64_decoded_index(text[baseindex + 1])));
				data |= bits_operation<0x00000FC0>::op_and(shift_bits<0x06>::left(get_base64_decoded_index(text[baseindex + 2])));
				data |= bits_operation<0x0000003F>::op_and(get_base64_decoded_index(text[baseindex + 3]));

				result[cpos++] = shift_bits<0x10>::right(bits_operation<0x00FF0000>::op_and(data)) & 0xFF;
				result[cpos++] = shift_bits<0x08>::right(bits_operation<0x0000FF00>::op_and(data)) & 0xFF;
				result[cpos++] = bits_operation<0x000000FF>::op_and(data) & 0xFF;

				baseindex += 4;
				count--;
			}

			data =	bits_operation<0x00FC0000>::op_and(shift_bits<0x12>::left(get_base64_decoded_index(text[baseindex	 ])));
			data |= bits_operation<0x0003F000>::op_and(shift_bits<0x0C>::left(get_base64_decoded_index(text[baseindex + 1])));
			result[cpos++] = shift_bits<0x10>::right(bits_operation<0x00FF0000>::op_and(data)) & 0xFF;
			size_t pop_back = 0;
			if (text[baseindex + 2] != equal_sign_literal)
			{
				data |= bits_operation<0x00000FC0>::op_and(shift_bits<0x06>::left(get_base64_decoded_index(text[baseindex + 2])));
				result[cpos++] = shift_bits<0x08>::right(bits_operation<0x0000FF00>::op_and(data)) & 0xFF;
			}
			else pop_back++;

			if (text[baseindex + 3] != equal_sign_literal)
			{
				data |= bits_operation<0x0000003F>::op_and(get_base64_decoded_index(text[baseindex + 3]));
				result[cpos] = bits_operation<0x000000FF>::op_and(data) & 0xFF;
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
		if (text_length > 0 && (text_length % 4) == 0)
		{
			size_t substract = text[text_length - 1] == equal_sign_literal ? 1 : 0;
			if (substract > 0 && text[text_length - 2] == equal_sign_literal)
			{
				substract++;
			}
			
			size_t length = text_length - substract;
			if (length < 1)
			{
				return false;
			}
			
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

	uint8_t b64encoder::get_base64_decoded_index(uint8_t value)
	{
		if (value >= 'A' && value <= 'Z') return value - 65;
		if (value >= 'a' && value <= 'z') return (value - 97) + 26;
		if (value >= '0' && value <= '9') return (value - 48) + 52;
		if (value == '+') return 62;
		
		return 63;
	}
}