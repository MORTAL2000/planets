#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "file.h"

#ifndef ASSETS_DIR
#define ASSETS_DIR ""
#endif

static std::string assets_directory = ASSETS_DIR;


std::string File::readString(const char *path)
{
    
    std::ifstream stream(assets_directory + path, std::ios::in);

    if (!stream.is_open())
        throw "Could not open: " + std::string(path);

    std::stringstream sstr;
    sstr << stream.rdbuf();
    stream.close();
    return sstr.str();
}

std::vector<unsigned char> File::readBinary(const char *path)
{
    std::ifstream stream(assets_directory + path, std::ios::binary);

    if (!stream.is_open())
        throw "Could not open: " + std::string(path);
        
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(stream), {});
}

void File::writeBinary(const char *path, std::vector<unsigned char> data)
{
    std::ofstream out(assets_directory + path, std::ios::out | std::ios::binary);
    out.write((const char *)&data[0], data.size());

    out.close();
}
