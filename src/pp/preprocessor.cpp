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
    if(!CONF::TRIGRAPH)
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

void Preprocessor::writeResult()
{
    std::string output(mFilename);
    for(auto pos = output.find('/');
        pos != std::string::npos;
        pos = output.find('/', pos + 2))
    {
        output.replace(pos, 1, "-");
    }

    std::string outputFilename(CONF::RESULT_PATH);
    outputFilename += "/";
    outputFilename += output;
    outputFilename += ".pp";

    if(!FileManager::write(outputFilename.c_str(), mSource))
    {
        std::cerr << "error: cannot open file; \""
                  << outputFilename
                  << "\""
                  << std::endl;
    }
}