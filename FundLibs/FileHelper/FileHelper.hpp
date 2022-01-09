#pragma once
#include <string>

#include <fstream>
#include <sstream>
#include <iostream>


std::string rdFile(const char* path);
template <typename T>
void wrFile(const char* path, T t) {
	std::ofstream fil;
	fil.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		fil.open(path);
		fil << t;
		fil.close();
	}
	catch (std::ifstream::failure e) {
		std::cout << "\nERROR::FILE::FILE_NOT_SUCCESFULLY_WRITE\n";
	}
}