#include "initializer.hpp"
#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>
#include <iostream>

Initializer::Initializer(Preprocessor* pp):
    mPP(pp)
{
}

void Initializer::execute() const
{
    openSource();
}

void Initializer::openSource() const
{
    std::string result;
    bool isFinded = false;
    switch(mPP->mESearch)
    {
        case(Preprocessor::CURRENT_ONLY):
        {
            if(isFindedCurrentPath(result))
                isFinded = true;
            break;
        }
        case(Preprocessor::CURRENT_AND_SYSTEM):
        {
            if(isFindedCurrentPath(result))
                isFinded = true;
            else if(isFindedSystemPath(result))
                isFinded = true;
            break;
        }
        case(Preprocessor::SYSTEM_ONLY):
        {
            if(isFindedSystemPath(result))
                isFinded = true;
            break;
        }
    }

    if(isFinded)
    {
        auto pos = result.rfind('/');
        if(pos != std::string::npos)
        {
            mPP->mDir = result.substr(0, pos + 1);
            mPP->mFile = result.substr(pos + 1, result.size() - pos - 1);
        }
        else
        {
            mPP->mFile = result;
        }

        FileManager::read(result.c_str(), mPP->mSrc);
    }
    else
    {
        std::cerr << "warning: PP cannot open file.\n"
                  << "    filename: "
                  << mPP->mDir
                  << mPP->mFile
                  << std::endl;
    }
}

bool Initializer::isFindedCurrentPath(std::string& result) const
{
    std::string searchFilename;
    if(!mPP->mDir.empty())
        searchFilename = mPP->mDir + mPP->mFile;
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
        searchFilename = path + mPP->mFile;
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