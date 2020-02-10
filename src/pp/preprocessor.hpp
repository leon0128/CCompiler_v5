#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class Preprocessor
{
private:
    static const std::unordered_map<std::string, char> TRIGRAPH_MAP;
    static const std::unordered_map<std::string, std::string> DIGRAPH_MAP;
    static const std::vector<std::string> PUNCTUATOR_VEC; // sort lengest first

public:
    Preprocessor();
    ~Preprocessor();

    bool operator()(int argc, char** argv);

private:
    void confirmArguments(int argc, char** argv);
    // initial processing
    void replaceTrigraphs();
    void joinBackslash();
    void deleteComment();
    void writeResult();
    // tokenization
    void tokenization();
    
    bool mIsValid;
};