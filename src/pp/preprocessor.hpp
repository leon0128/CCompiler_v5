#pragma once

#include "token.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class Preprocessor
{
private:
    // preprocessing directive
    enum EDirective
    {
        INCLUDE
    };

    static const std::unordered_map<std::string, char> TRIGRAPH_MAP;
    static const std::unordered_map<std::string, std::string> DIGRAPH_MAP;
    static const std::unordered_map<std::string, EDirective> DIRECTIVE_MAP;
    static const std::vector<std::string> PUNCTUATOR_VEC; // sort lengest first

public:
    enum ESearch
    {
        CURRENT_ONLY,
        CURRENT_SYSTEM,
        SYSTEM_ONLY
    };

    Preprocessor();
    ~Preprocessor();

    bool operator()(int argc,
                    const char* const * const argv,
                    const char* directory,
                    ESearch eSearch);

private:
    void confirmArguments(int argc,
                          const char* const * const argv,
                          const char* directory,
                          ESearch eSearch);
    // initial processing
    void replaceTrigraphs();
    void joinBackslash();
    void deleteComment();
    void writeResult();
    // tokenization
    void tokenization();
    // process a preprocessing language
    void processPreprocessingLanguage();
        void processInclude(std::size_t index);

    std::string mFilename;
    std::string mSource;
    std::vector<Token> mTokens;
    ESearch mESearch;

    bool mIsValid;
};