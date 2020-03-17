#pragma once

#include "token.hpp"
#include <vector>

class Preprocessor;

class PPDirectiveProcessor
{
public:
    PPDirectiveProcessor(Preprocessor* pp);

    bool execute();

private:
    void process(PreprocessingFile*);

    Preprocessor* mPP;
    bool mIsValid;
};