// Ascii Base64 Ecoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include "b64encoder.h"

using namespace std;
using namespace base64;

typedef char*	char_ptr;
typedef char**	char_ptr2;

void create_test_case_data(char_ptr2&, const char_ptr2&);
void release_test_case_data(char_ptr2&);
string read_from_file(const string);
void write_to_file(const string, const string&);

int main(int argc, char_ptr2 argv)
{
	char_ptr2 source_data = NULL;

#ifndef _DEBUG
	if (argc < 2)
	{
		cout << "No text to convert was specified.";
		getchar();
		return -1;
	}
	source_data = argv;
#else
	create_test_case_data(source_data, argv);
	argc = 3;
#endif

	b64encoder::enum_container_type source_type = b64encoder::enum_container_type::text;
	b64encoder::enum_container_type target_type = b64encoder::enum_container_type::text;

	string source = "";
	string target = "";
	bool is_encoding = true;

	for (int i = 0; i < argc; i++)
	{
		if (!strncmp(source_data[i], "-sT", 3) && source.length() == 0)
		{
			source_type = b64encoder::enum_container_type::text;
			if ((i + 1) < argc)
			{
				source = source_data[i + 1];
			}
		}
		else if (!strncmp(source_data[i], "-sF", 3) && source.length() == 0)
		{
			source_type = b64encoder::enum_container_type::file;
			if ((i + 1) < argc)
			{
				source = source_data[i + 1];
			}
		}
		else if (!strncmp(source_data[i], "-dF", 3) && target.length() == 0)
		{
			target_type = b64encoder::enum_container_type::file;
			if ((i + 1) < argc)
			{
				target = source_data[i + 1];
			}
			else
			{
				std::cout << "No target file was specified." << std::endl;
				return -1;
			}
		}
		else if (strnlen_s(source_data[i], 3) == 2 && !strncmp(source_data[i], "-d", 2))
		{
			is_encoding = false;
		}
	}

	if (source.length() == 0)
	{
		std::cout << "No source data was specified." << std::endl;
		return -1;
	}

	string source_text = "";
	if (is_encoding)
	{
		source_text = source_type == b64encoder::enum_container_type::text ?
			b64encoder::encode(source) :
			b64encoder::encode(read_from_file(source));
	}
	else
	{
		source_text = source_type == b64encoder::enum_container_type::text ?
			b64encoder::decode(source) :
			b64encoder::decode(read_from_file(source));
	}

	switch (target_type)
	{
		case b64encoder::enum_container_type::text:
			std::cout << "Result for " << (is_encoding ? "encoding" : "decoding: ") << std::endl;
			std::cout << source_text.c_str() << std::endl;
			break;
		case b64encoder::enum_container_type::file: 
			std::cout << "Result for " << (is_encoding ? "encoding" : "decoding") << " to file: " << target.c_str() << std::endl;
			write_to_file(target, source_text);
			break;
	}
	
	std::cout << std::endl << "Press any key to end..." << std::endl;

#if _DEBUG
	release_test_case_data(source_data);
#endif

	getchar();

	return 0;
}


void create_test_case_data(char_ptr2& data_pointer, const char_ptr2& args)
{
	data_pointer = new char*[3];
	data_pointer[0] = args[0];
	data_pointer[1] = new char[4];
	data_pointer[1][3] = NULL;
	memcpy_s(data_pointer[1], 3, "-sT", 3);
	data_pointer[2] = new char[30];
	data_pointer[2][29] = NULL;
	memcpy_s(data_pointer[2], 29, "Data to convert for testing.", 29);
}

void release_test_case_data(char_ptr2& data_pointer)
{
	if (data_pointer)
	{
		delete data_pointer[2];
		delete data_pointer[1];
		delete data_pointer;
	}
}

string read_from_file(const string file_path)
{
	ifstream file(file_path, ios::binary | ios::ate);
	streamsize file_size = file.tellg();
	file.seekg(0, ios::beg);

	string file_content((unsigned int)file_size, '\x0');
	if (file.read((char*)file_content.c_str(), file_size))
	{
		return file_content;
	}

	return "";
}

void write_to_file(const string file_path, const string& file_content)
{
	ofstream file(file_path, ios::binary | ios::trunc);
	file.write(file_content.c_str(), file_content.size());
	file.close();
}
