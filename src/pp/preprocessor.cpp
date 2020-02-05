#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include <iostream>
#include <string>

Preprocessor::Preprocessor():
    mFilename(nullptr),
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

    std::cout << "source: \n"
              << mSource
              << std::endl; 

    return mIsValid;
}

bool Preprocessor::confirmArguments(int argc, char** argv)
{
    // confirm argc
    if(argc < 2)
    {
        std::cout << "error: the number of command-line arguments must be greater than 2."
                  << std::endl;
        return mIsValid = false;
    }

    // confirm argv
    mFilename = argv[1];
    if(!FileManager::read(mFilename, mSource))
    {
        std::cout << "error: cannot open file; \""
                  << mFilename
                  << "\""
                  << std::endl;
        return mIsValid = false;
    }

    return true;
}