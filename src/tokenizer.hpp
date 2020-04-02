#pragma once

#include "token.hpp"
#include <vector>

class Tokenizer
{
public:
    Tokenizer();

    bool execute(const std::vector<PreprocessingToken*>&);

private:
    bool externalDeclaration(); 
    bool translationUnit();

    std::vector<PreprocessingToken*> mPPTokens;
    std::size_t mIdx;

    bool mIsValid;
};