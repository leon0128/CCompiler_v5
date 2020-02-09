#pragma once
#include <string>
#include <unordered_map>

class Preprocessor
{
private:
    static const std::unordered_map<std::string, char> TRIGRAPH_MAP;

public:
    Preprocessor();
    ~Preprocessor();

    bool operator()(int argc, char** argv);

private:
    void confirmArguments(int argc, char** argv);
    void replaceTrigraphs();
    void joinBackslash();
    void deleteComment();
    void writeResult();

    bool mIsValid;
};