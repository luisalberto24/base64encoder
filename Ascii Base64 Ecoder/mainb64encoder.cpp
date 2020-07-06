// Ascii Base64 Ecoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "b64encoder.h"

using namespace std;
using namespace base64;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "No text to convert was specified.";
		getchar();
		return -1;
	}

	std::string result = base64::b64encoder::encode(std::string(argv[1]));
	cout << result << std::endl;

	getchar();

	return 0;
}