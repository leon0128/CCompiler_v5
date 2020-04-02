#pragma once

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    void checkArgs(int argc, char** argv);
    void preprocess(const char* file);
    void tokenize();

    void error(const char* message) const;

    class Preprocessor* mPP;
    class Tokenizer* mTokenizer;

    bool mIsValid;
};