#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>

Preprocessor::Preprocessor(const std::string& filename,
                           const std::string& directory,
                           ESearch eSearch):
    mFilename(filename),
    mDirectory(directory),
    mSource(),
    mESearch(eSearch)
{
}

void Preprocessor::execute()
{

}

void Preprocessor::openSource()
{

}

bool Preprocessor::isFindedCurrentPath(std::string& filename) const
{
    std::string searchFilename;
    if(!mDirectory.empty())
        searchFilename = mDirectory + "/" + mFilename;
    else
        searchFilename = mFilename;
    
    if(FileManager::isExisted(searchFilename.c_str()))
    {
        filename = std::move(searchFilename);
        return true;
    }
    else
    {
        return false;
    }
}