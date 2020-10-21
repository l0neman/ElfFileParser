// ElfFileParser.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "ElfParser.h"

using namespace std;

int main()
{
	cout << "Hello World!" << endl;
	ElfParser elf_parser(R"(..\..\..\file\armeabi-v7a\libfoo.so)");
	elf_parser.parse();
	return 0;
}
