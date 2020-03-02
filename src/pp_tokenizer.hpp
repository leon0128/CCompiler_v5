#pragma once

#include "token.hpp"
#include <string>

class Preprocessor;

class PPTokenizer
{
public:
    PPTokenizer(Preprocessor* pp);

    bool execute();

private:
    bool isMatched(std::string::size_type idx, char c) const;

    std::string::size_type mIdx;

    Preprocessor* mPP;
    bool mIsValid;
};