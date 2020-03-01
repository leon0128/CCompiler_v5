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

Initializer::Initializer():
    mFile(),
    mDir(),
    mSrc(),
    mIsValid(true)
{
}

bool Initializer::execute(std::string& ppfile,
                          std::string& ppdir,
                          Preprocessor::ESearch eSearch)
{
    mFile = ppfile;
    mDir = ppdir;

    openFile(eSearch);

    ppfile = mFile;
    ppdir = mDir;

    if(mIsValid)
    {
        replaceTrigraph();
        joinLine();
    }

    return mIsValid;
}

void Initializer::openFile(Preprocessor::ESearch eSearch)
{
    std::string result;
    bool isFound = false;

    switch(eSearch)
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
            mDir = result.substr(0, pos + 1);
            mFile = result.substr(pos + 1, result.size() - pos - 1);
        }
        else
            mFile = result;

        FileManager::read(result.c_str(), mSrc);
        mSrc.push_back('\n');
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

    if(mDir.empty())
        result = mFile;
    else
        result = mDir + mFile;

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
        result = path + mFile;
        
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

void Initializer::replaceTrigraph()
{
    if(config().pp_is_replaced_trigraph)
    {
        for(auto pos = mSrc.find("??");
            pos != std::string::npos;
            pos = mSrc.find("??", pos + 1))
        {
            if(pos + 2 >= mSrc.size())
                break;
            
            auto iter = TRIGRAPH_MAP.find(mSrc[pos + 2]);
            if(iter != TRIGRAPH_MAP.end())
            {
                char c = iter->second;
                mSrc.replace(pos, 3, 1, c);
            }
        }
    }
}

void Initializer::joinLine()
{
    for(auto pos = mSrc.find('\\');
        pos != std::string::npos;
        pos = mSrc.find('\\', pos))
    {
        auto end = pos + 1;

        if(config().pp_is_ignored_space)
        {
            for(; end < mSrc.size(); end++)
            {
                if(mSrc[end] != ' ')
                    break;
            }
        }

        if(end >= mSrc.size())
            break;

        if(mSrc[end] == '\n')
            mSrc.replace(pos, end - pos + 1, "");
        else
            pos++;
    }

    if(mSrc.back() != '\n')
        mSrc.push_back('\n');
}

void Initializer::error(const char* message) const
{
    std::cerr << "initializer-class-error:\n    "
              << message
              << std::endl;
}