#pragma once

#include "token.hpp"
#include <vector>
#include <string>

class Preprocessor;
class TokenConverter;
class Tokenizer;
class Translator;

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
    void translate();

    void error(const char* message) const;

    Preprocessor* mPP;
    TokenConverter* mTokenConverter;
    Tokenizer* mTokenizer;
    Translator* mTranslator;

    std::vector<Token*> mTokens;

    TranslationUnit* mTranslationUnit;

    std::string mAssemble;

    bool mIsValid;
};