#pragma once

#include <string>

class FileManager
{
public:
    FileManager() = delete;

    static bool isExisted(const char* filename);

    static bool write(const char* filename,
                      const std::string& data);

    static bool read(const char* filename,
                     std::string& data);
};