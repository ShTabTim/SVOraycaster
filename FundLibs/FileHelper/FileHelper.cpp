#include "FileHelper.hpp"
#include <Windows.h>

std::string rdFile(const char* path) {
    std::ifstream fil;
    fil.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream fStream;
        fil.open(path);
        fStream << fil.rdbuf();
        fil.close();

        return fStream.str();
    }
    catch (std::ifstream::failure e) {
        MessageBox(nullptr, L"ERROR::FILE::FILE_NOT_SUCCESFULLY_READ",L"", MB_ICONERROR);
    }
    return 0;
}