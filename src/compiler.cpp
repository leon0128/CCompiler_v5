#include "compiler.hpp"
#include "preprocessor.hpp"
#include <iostream>

Compiler::Compiler():
    mPP(nullptr),
    mIsValid(true)
{
    mPP = new Preprocessor();
}

Compiler::~Compiler()
{
    delete mPP;
}

bool Compiler::execute(int argc, char** argv)
{
    checkArgs(argc, argv);
    
    if(mIsValid)
        preprocess(argv[1]);

    return mIsValid;
}

void Compiler::checkArgs(int argc, char** argv)
{
    if(argc < 2)
    {
        mIsValid = false;    
        error("number of command-line arguments is invalid.");
    }
}

void Compiler::preprocess(const char* file)
{
    if(!mPP->execute(file))
    {
        mIsValid = false;
        error("failed to preprocess.");
    }
}

void Compiler::error(const char* message) const
{
    std::cerr << "compiler-class-error:\n    "
              << message
              << std::endl;
}