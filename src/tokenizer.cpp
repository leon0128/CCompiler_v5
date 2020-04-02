#include "tokenizer.hpp"
#include "token_converter.hpp"
#include <iostream>

Tokenizer::Tokenizer():
    mPPTokens(),
    mIdx(0),
    mIsValid(true)
{
}

bool Tokenizer::execute(const std::vector<PreprocessingToken*>& ppTokens)
{
    mPPTokens = ppTokens;

    translationUnit();

    return mIsValid;
}

bool Tokenizer::externalDeclaration()
{
    
}

bool Tokenizer::translationUnit()
{
    while(mIdx < mPPTokens.size())
    {
        if(!externalDeclaration())
        {
            mIsValid = false;
            std::cerr << "Tokenizer error:\n"
                      << "    what: External Declaration is invalid."
                      << std::endl;
            break;
        }
    }

    return mIsValid;
}