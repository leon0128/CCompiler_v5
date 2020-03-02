#include "pp_tokenizer.hpp"
#include "preprocessor.hpp"
#include <string>
#include <utility>

PPTokenizer::PPTokenizer(Preprocessor* pp):
    mIdx(0),
    mPP(pp),
    mIsValid(true)
{
}

bool PPTokenizer::execute()
{
    while(mIdx < mPP->mSrc.size() && mIsValid)
    {
        char c = mPP->mSrc[mIdx];
        if(c != ' ' &&
           c != '\t' &&
           c != '\v' &&
           c != '\f')
        {
            PreprocessingToken* preprocessingToken = getPreprocessingToken();
            if(preprocessingToken)
                mPP->mPPTokens.push_back(preprocessingToken);
            else
                mIsValid = false;
        }
        else
            mIdx++;
    }
        
    return mIsValid;
}

bool PPTokenizer::isMatched(std::string::size_type idx, char c) const
{
    if(idx < mPP->mSrc.size())
        return mPP->mSrc[idx] == c;
    else
        return false;
}