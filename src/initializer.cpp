#include "initializer.hpp"
#include "preprocessor.hpp"
#include "file_manager.hpp"
#include "config.hpp"
#include <iostream>

const std::unordered_map<char, char> Initializer::TRIGRAPH_MAP
    = {{'(', '['},
       {')', ']'},
       {'<', '{'},
       {'>', '}'},
       {'=', '#'},
       {'/', '\\'},
       {'\'', '^'},
       {'!', '|'},
       {'-', '~'}};

Initializer::Initializer(Preprocessor* pp):
    mPP(pp),
    mIsValid(true)
{
}

bool Initializer::execute()
{
    openFile();

    if(mIsValid)
    {
        replaceTrigraph();
        joinLine();
    }

    return mIsValid;
}

void Initializer::openFile()
{
    std::string result;
    bool isFound = false;

    switch(mPP->mESearch)
    {
        case(Preprocessor::CURRENT_ONLY):
            if(isFoundCurrentPath(result))
                isFound = true;
            break;

        case(Preprocessor::CURRENT_AND_SYSTEM):
            if(isFoundCurrentPath(result))
                isFound = true;
            else if(isFoundSystemPath(result))
                isFound = true;
            break;
        
        case(Preprocessor::SYSTEM_ONLY):
            if(isFoundSystemPath(result))
                isFound = true;
            break;
    }

    if(isFound)
    {
        auto pos = result.rfind('/');
        if(pos != std::string::npos)
        {
            mPP->mDir = result.substr(0, pos + 1);
            mPP->mFile = result.substr(pos + 1, result.size() - pos - 1);
        }
        else
            mPP->mFile = result;

        FileManager::read(result.c_str(), mPP->mSrc);
        mPP->mSrc.push_back('\n');
    }
    else
    {
        mIsValid = false;
        error("failed to open file.");
    }
}

bool Initializer::isFoundCurrentPath(std::string& result) const
{
    result.clear();

    if(mPP->mDir.empty())
        result = mPP->mFile;
    else
        result = mPP->mDir + mPP->mFile;

    if(FileManager::isExisted(result.c_str()))
        return true;
    else
        return false;
}

bool Initializer::isFoundSystemPath(std::string& result) const
{
    result.clear();

    bool isExisted = false;
    for(auto&& path : config().sys_include_pathname_vec)
    {
        result = path + mPP->mFile;
        
        if(FileManager::isExisted(result.c_str()))
        {
            isExisted = true;
            break;
        }
    }

    if(isExisted)
        return true;
    else
        return false;
}

void Initializer::replaceTrigraph() const
{
    if(config().pp_is_replaced_trigraph)
    {
        for(auto pos = mPP->mSrc.find("??");
            pos != std::string::npos;
            pos = mPP->mSrc.find("??", pos + 1))
        {
            if(pos + 2 >= mPP->mSrc.size())
                break;
            
            auto iter = TRIGRAPH_MAP.find(mPP->mSrc[pos + 2]);
            if(iter != TRIGRAPH_MAP.end())
            {
                char c = iter->second;
                mPP->mSrc.replace(pos, 3, 1, c);
            }
        }
    }
}

void Initializer::joinLine() const
{
    for(auto pos = mPP->mSrc.find('\\');
        pos != std::string::npos;
        pos = mPP->mSrc.find('\\', pos))
    {
        auto end = pos + 1;

        if(config().pp_is_ignored_space)
        {
            for(; end < mPP->mSrc.size(); end++)
            {
                if(mPP->mSrc[end] != ' ')
                    break;
            }
        }

        if(end >= mPP->mSrc.size())
            break;

        if(mPP->mSrc[end] == '\n')
            mPP->mSrc.replace(pos, end - pos + 1, "");
        else
            pos++;
    }

    if(mPP->mSrc.back() != '\n')
        mPP->mSrc.push_back('\n');
}

void Initializer::error(const char* message) const
{
    std::cerr << "initializer-class-error:\n    "
              << message
              << std::endl;
}