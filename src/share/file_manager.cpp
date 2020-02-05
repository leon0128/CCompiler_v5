#include "file_manager.hpp"
#include <fstream>
#include <sstream>

bool FileManager::read(const char* filename,
                       std::string& data)
{
    data.clear();

    std::ifstream file(filename, std::ios::in);
    if(!file.is_open())
        return false;

    std::stringstream stream;
    stream << file.rdbuf();

    data = stream.str();
    file.close();

    return true;
}