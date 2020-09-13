#include "stdint.h"

#pragma once

template<uint8_t bits>
class shift_bits
{
	public:
		static uint32_t right(uint32_t value)
		{
			return  value >> bits;
		}
		
		static uint32_t left(uint32_t value)
		{
			return value << bits;
		}
};

template<uint32_t mask>
class bits_operation
{
	public:
		static uint32_t op_and(uint32_t value)
		{ 
			return (value & mask);  
		}

		static uint32_t op_or(uint32_t value)
		{ 
			return (value | mask); 
		}
};