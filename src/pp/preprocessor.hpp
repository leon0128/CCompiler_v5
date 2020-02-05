#pragma once
#include <string>

class Preprocessor
{
public:
    Preprocessor();
    ~Preprocessor();

    bool operator()(int argc, char** argv);

private:
    bool confirmArguments(int argc, char** argv);

    char* mFilename;
    std::string mSource;
    
    bool mIsValid;
};