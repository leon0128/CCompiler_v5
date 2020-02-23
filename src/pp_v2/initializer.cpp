#include "initializer.hpp"
#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>
#include <iostream>

const std::unordered_map<std::string, char> Initializer::TRIGRAPH_MAP
    = {{"?\?(", '['},
       {"?\?)", ']'},
       {"?\?<", '{'},
       {"?\?>", '}'},
       {"?\?=", '#'},
       {"?\?/", '\\'},
       {"?\?\'", '^'},
       {"?\?!", '|'},
       {"?\?-", '~'}};

Initializer::Initializer(Preprocessor* pp):
    mPP(pp)
{
}

void Initializer::execute() const
{
    openSource();
    replaceTrigraph();
    joinLine();
    deleteComment();
}

void Initializer::openSource() const
{
    std::string result;
    bool isFound = false;
    switch(mPP->mESearch)
    {
        case(Preprocessor::CURRENT_ONLY):
        {
            if(isFoundCurrentPath(result))
                isFound = true;
            break;
        }
        case(Preprocessor::CURRENT_AND_SYSTEM):
        {
            if(isFoundCurrentPath(result))
                isFound = true;
            else if(isFoundSystemPath(result))
                isFound = true;
            break;
        }
        case(Preprocessor::SYSTEM_ONLY):
        {
            if(isFoundSystemPath(result))
                isFound = true;
            break;
        }
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
        {
            mPP->mFile = result;
        }

        FileManager::read(result.c_str(), mPP->mSrc);
        mPP->mSrc.push_back('\n');
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

bool Initializer::isFoundCurrentPath(std::string& result) const
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

bool Initializer::isFoundSystemPath(std::string& result) const
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

void Initializer::replaceTrigraph() const
{
    if(config().pp_is_replaced_trigraph)
    {
        for(auto pos = mPP->mSrc.find("??");
            pos != std::string::npos;
            pos = mPP->mSrc.find("??", pos + 1))
        {
            auto iter = TRIGRAPH_MAP.find(mPP->mSrc.substr(pos, 3));
            if(iter != TRIGRAPH_MAP.end())
                mPP->mSrc.replace(pos, 3, 1, iter->second);
        }
    }
}

void Initializer::joinLine() const
{
    for(auto pos = mPP->mSrc.find('\\');
        pos != std::string::npos;
        pos = mPP->mSrc.find('\\', pos))
    {
        auto endPos = pos + 1;

        if(config().pp_is_ignored_space)
        {
            for(; endPos < mPP->mSrc.size(); endPos++)
            {
                if(mPP->mSrc.at(endPos) != ' ')
                    break;
            }
        }

        if(mPP->mSrc.at(endPos) == '\n')
            mPP->mSrc.erase(mPP->mSrc.begin() + pos,
                            mPP->mSrc.begin() + endPos + 1);
        else
            pos++;
    }

    if(mPP->mSrc.back() != '\n')
        mPP->mSrc.push_back('\n');
}

void Initializer::deleteComment() const
{
    std::string line;
    std::string::size_type pos = 0;
    while(getLine(line, mPP->mSrc, pos))
    {
        for(std::string::size_type p = 0; p < line.size(); p++)
        {
            char c = line.at(p);
            if(c == '"' ||
               c == '\'')
            {
                char punctuator = c;
                for(p++; p < line.size(); p++)
                {
                    c = line.at(p);
                    if(c == '\\')
                        p++;
                    else if(c == punctuator)
                        break;
                }
            }
            else if(c == '/' &&
                    p + 1 < line.size() &&
                    line[p + 1] == '/')
            {
                mPP->mSrc.erase(mPP->mSrc.begin() + pos + p,
                                mPP->mSrc.begin() + pos + line.size());
                pos += p + 1;
                break;
            }
            else if(c == '/' &&
                    p + 1 < line.size() &&
                    line[p + 1] == '*')
            {
                auto end = mPP->mSrc.find("*/", pos + 2);
                if(end != std::string::npos)
                {
                    mPP->mSrc.replace(p, end - p + 2, " ");
                    break;
                }
                else
                {
                    std::cerr << "warning: block-comment end token not found.\n"
                              << "    line contents: "
                              << line
                              << std::endl;
                }
            }
            
            if(p + 1 == line.size())
                pos += line.size() + 1;
        }

        if(line.empty())
            pos++;
    }
}

bool Initializer::getLine(std::string& line,
                          const std::string& src,
                          std::string::size_type pos) const
{
    line.clear();

    if(pos < src.size())
    {
        for(; pos < src.size(); pos++)
        {
            char c = src.at(pos);
            if(c != '\n')
                line.push_back(c);
            else
                break;
        }

        return true;
    }
    else
    {
        return false;
    }
}