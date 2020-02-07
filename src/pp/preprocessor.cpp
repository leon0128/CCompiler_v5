#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <iostream>
#include <string>

const std::unordered_map<std::string, char> Preprocessor::TRIGRAPH_MAP
    = {{"?\?(", '['},
       {"?\?)", ']'},
       {"?\?<", '{'},
       {"?\?>", '}'},
       {"?\?=", '#'},
       {"?\?/", '\\'},
       {"?\?\'", '^'},
       {"?\?!", '|'},
       {"?\?-", '~'}};

Preprocessor::Preprocessor():
    mFilename(),
    mSource(),
    mIsValid(true)
{
}

Preprocessor::~Preprocessor()
{
}

bool Preprocessor::operator()(int argc, char** argv)
{
    confirmArguments(argc, argv);
    if(mIsValid)
        replaceTrigraphs();
    if(mIsValid)
        joinBackslash();
    if(mIsValid)
        deleteComment();
    if(mIsValid)
        writeResult();

    return mIsValid;
}

void Preprocessor::confirmArguments(int argc, char** argv)
{
    // confirm argc
    if(argc < 2)
    {
        std::cerr << "error: the number of command-line arguments must be greater than 2."
                  << std::endl;
        mIsValid = false;
        return;
    }

    // confirm argv
    mFilename = argv[1];
    if(!FileManager::read(mFilename.c_str(), mSource))
    {
        std::cerr << "error: cannot open file; \""
                  << mFilename
                  << "\""
                  << std::endl;
        mIsValid = false;
    }
}

void Preprocessor::replaceTrigraphs()
{
    if(!config().pp_is_replaced_trigraph)
        return;

    for(auto pos = mSource.find("??");
        pos != std::string::npos;
        pos = mSource.find("??", pos + 1))
    {
        if(pos + 2 >= mSource.size())
            break;

        auto iter = TRIGRAPH_MAP.find(mSource.substr(pos, 3));
        if(iter != TRIGRAPH_MAP.end())
            mSource.replace(pos, 3, 1, iter->second);
    }
}

void Preprocessor::joinBackslash()
{
    for(auto pos = mSource.find('\\');
        pos != std::string::npos;
        pos = mSource.find('\\', pos))
    {
        auto endPos = pos + 1;
        bool isValid = false;

        if(config().pp_is_ignored_space)
        {
            for(; endPos < mSource.size(); endPos++)
            {
                if(mSource.at(endPos) == ' ')
                    continue;
                else if(mSource.at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }
        else
        {
            for(; endPos < mSource.size(); endPos++)
            {
                if(mSource.at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }

        if(isValid)
            mSource.replace(pos, endPos - pos + 1, "");
        else
            pos++;
    }
}

void Preprocessor::deleteComment()
{
}

void Preprocessor::writeResult()
{
    std::string outputFilename(config().gen_result_pathname +
                               "/" +
                               config().pp_result_filename);

    if(!FileManager::write(outputFilename.c_str(), mSource))
    {
        std::cerr << "error: cannot open file; \""
                  << outputFilename
                  << "\""
                  << std::endl;
    }
}