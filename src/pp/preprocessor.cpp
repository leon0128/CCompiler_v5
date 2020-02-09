#include "preprocessor.hpp"
#include "data.hpp"
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
    DATA::filename() = argv[1];
    if(!FileManager::read(DATA::filename().c_str(), DATA::source()))
    {
        std::cerr << "error: cannot open file; \""
                  << DATA::filename()
                  << "\""
                  << std::endl;
        mIsValid = false;
    }
}

void Preprocessor::replaceTrigraphs()
{
    if(!config().pp_is_replaced_trigraph)
        return;

    for(auto pos = DATA::source().find("??");
        pos != std::string::npos;
        pos = DATA::source().find("??", pos + 1))
    {
        if(pos + 2 >= DATA::source().size())
            break;

        auto iter = TRIGRAPH_MAP.find(DATA::source().substr(pos, 3));
        if(iter != TRIGRAPH_MAP.end())
            DATA::source().replace(pos, 3, 1, iter->second);
    }
}

void Preprocessor::joinBackslash()
{
    for(auto pos = DATA::source().find('\\');
        pos != std::string::npos;
        pos = DATA::source().find('\\', pos))
    {
        auto endPos = pos + 1;
        bool isValid = false;

        if(config().pp_is_ignored_space)
        {
            for(; endPos < DATA::source().size(); endPos++)
            {
                if(DATA::source().at(endPos) == ' ')
                    continue;
                else if(DATA::source().at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }
        else
        {
            for(; endPos < DATA::source().size(); endPos++)
            {
                if(DATA::source().at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }

        if(isValid)
            DATA::source().replace(pos, endPos - pos + 1, "");
        else
            pos++;
    }
}

void Preprocessor::deleteComment()
{
    for(auto pos = DATA::source().find('/');
        pos != std::string::npos;
        pos = DATA::source().find('/', pos + 1))
    {
        if(pos + 1 >= DATA::source().size())
            break;

        if(DATA::source().at(pos + 1) == '/')
        {
            auto p = DATA::source().find('\n', pos + 2);
            if(p != std::string::npos)
                DATA::source().replace(pos, p - pos + 1, " ");
            else
                DATA::source().replace(pos, DATA::source().size() - pos, " ");
        }
        else if(DATA::source().at(pos + 1) == '*')
        {
            auto p = DATA::source().find("*/", pos + 2);
            if(p != std::string::npos)
                DATA::source().replace(pos, p - pos + 2, " ");
        }
    }
}

void Preprocessor::writeResult()
{
    std::string outputFilename(config().gen_result_pathname +
                               "/" +
                               config().pp_result_filename);

    if(!FileManager::write(outputFilename.c_str(), DATA::source()))
    {
        std::cerr << "error: cannot open file; \""
                  << outputFilename
                  << "\""
                  << std::endl;
    }
}