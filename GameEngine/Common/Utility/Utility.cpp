#include "Utility.hpp"

void My::print_fstream(std::ifstream& fstream)
{
	fstream.clear();
	fstream.seekg(0);
	std::cout << fstream.rdbuf();
}

void My::rewind_fstream(std::ifstream& fstream)
{
	fstream.clear();
	fstream.seekg(0);
}
