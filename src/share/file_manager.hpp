#pragma once

#include <string>

class FileManager
{
public:
    FileManager() = delete;

    static bool read(const char* filename,
                     std::string& data);
};