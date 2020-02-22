#include "initializer.hpp"
#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>

Initializer::Initializer(Preprocessor* pp):
    mPP(pp)
{
}

void Initializer::execute() const
{

}

void Initializer::openSource() const
{

}

bool Initializer::isFindedCurrentPath(std::string& result) const
{
    std::string searchFilename;
    if(!mPP->mDir.empty())
        searchFilename = mPP->mDir + "/" + mPP->mFile;
    else
        searchFilename = mPP->mFile;

    if(FileManager::isExisted(searchFilename.c_str()))
    {
        result = std::move(searchFilename);
        return true;
    }
    else
    {
        return false;
    }
}

bool Initializer::isFindedSystemPath(std::string& result) const
{
    std::string searchFilename;

    bool isExisted = false;
    for(auto&& path : config().sys_include_pathname_vec)
    {
        searchFilename = path + "/" + mPP->mFile;
        if(FileManager::isExisted(searchFilename.c_str()))
        {
            result = std::move(searchFilename);
            isExisted = true;
            break;
        }
    }

    if(isExisted)
        return true;
    else
        return false;
}