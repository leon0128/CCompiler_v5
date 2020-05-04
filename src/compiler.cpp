#include "compiler.hpp"
#include "preprocessor.hpp"
#include "token.hpp"
#include "token_converter.hpp"
#include "tokenizer.hpp"
#include "translator.hpp"
#include <iostream>

Compiler::Compiler():
    mPP(nullptr),
    mTokenConverter(nullptr),
    mTokenizer(nullptr),
    mTranslator(nullptr),
    mTokens(),
    mTranslationUnit(nullptr),
    mAssemble(),
    mIsValid(true)
{
    mPP = new Preprocessor();
    mTokenConverter = new TokenConverter();
    mTokenizer = new Tokenizer();
    mTranslator = new Translator();
}

Compiler::~Compiler()
{
    delete mPP;
    delete mTokenConverter;
    delete mTokenizer;
    delete mTranslator;

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

    if(mIsValid)
        tokenize();

    if(mIsValid)
        translate();

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
    if(mTranslationUnit = mTokenizer->execute(mTokens))
        ;
    else
        mIsValid = false;

    if(!mIsValid)
        error("failed to tokenize.");
}

void Compiler::translate()
{
    if(!(mIsValid = mTranslator->execute(mTranslationUnit, mAssemble)))
        error("failed to translate.");
}

void Compiler::error(const char* message) const
{
    std::cerr << "Compiler error:\n"
              << "    what: " << message
              << std::endl;
}