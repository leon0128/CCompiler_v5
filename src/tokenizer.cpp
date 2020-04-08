#include "tokenizer.hpp"

Tokenizer::Tokenizer():
    mPPTokens(),
    mIdx(0),
    mIsValid(true)
{
}

bool Tokenizer::execute(const std::vector<PreprocessingToken*>&)
{
    return mIsValid;
}