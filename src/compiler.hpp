#pragma once

#include "token.hpp"
#include <vector>

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    void checkArgs(int argc, char** argv);
    void preprocess(const char* file);
    void convert();
    void tokenize();

    void error(const char* message) const;

    class Preprocessor* mPP;
    class TokenConverter* mTokenConverter;
    class Tokenizer* mTokenizer;

    std::vector<Token*> mTokens;

    bool mIsValid;
};