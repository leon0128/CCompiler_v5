#include "file_manager.hpp"
#include <fstream>
#include <sstream>

bool FileManager::isExisted(const char* filename)
{
    std::ifstream file(filename);
    return file.is_open();
}

bool FileManager::write(const char* filename,
                        const std::string& data)
{
    std::ofstream file(filename, std::ios::out);
    if(!file.is_open())
        return false;
    
    file << data;
    file.close();

    return true;
}

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