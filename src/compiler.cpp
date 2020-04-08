#include "compiler.hpp"
#include "preprocessor.hpp"
#include "token.hpp"
#include "token_converter.hpp"
#include "tokenizer.hpp"
#include <iostream>

Compiler::Compiler():
    mPP(nullptr),
    mTokenConverter(nullptr),
    mTokenizer(nullptr),
    mTokens(),
    mIsValid(true)
{
    mPP = new Preprocessor();
    mTokenConverter = new TokenConverter();
    mTokenizer = new Tokenizer();
}

Compiler::~Compiler()
{
    delete mPP;
    delete mTokenConverter;
    delete mTokenizer;

    BaseToken::destroy();
}

bool Compiler::execute(int argc, char** argv)
{
    checkArgs(argc, argv);

    if(mIsValid)
        Preprocessor::includePredefinedMacro();
          
    if(mIsValid)
        preprocess(argv[1]);

    if(mIsValid)
        convert();

    // if(mIsValid)
    //     tokenize();

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
    mIsValid = mPP->execute(file);

    if(mIsValid)
        mIsValid = mPP->characterConvert();

    if(!mIsValid)
        error("failed to preprocess.");
}

void Compiler::convert()
{
    mIsValid = mTokenConverter->execute(mPP->mProcessedPPTokens, mTokens);

    if(!mIsValid)
        error("falied to convert from preprocessing-token to token.");
}

void Compiler::tokenize()
{
    mIsValid = mTokenizer->execute(mPP->getPPTokens());

    if(!mIsValid)
        error("failed to tokenize.");
}

void Compiler::error(const char* message) const
{
    std::cerr << "Compiler error:\n"
              << "    what: " << message
              << std::endl;
}