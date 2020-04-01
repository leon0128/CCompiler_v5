#include "token_converter.hpp"

TokenConverter::TokenConverter():
    mIsValid(true)
{
}

bool TokenConverter::execute()
{
    return mIsValid;
}