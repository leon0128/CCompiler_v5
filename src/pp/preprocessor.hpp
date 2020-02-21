#pragma once

#include "token.hpp"
#include "macro.hpp"
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>

class Preprocessor
{
private:
    // preprocessing directive
    enum EDirective
    {
        INCLUDE,
        DEFINE
    };

    static const std::unordered_map<std::string, char> TRIGRAPH_MAP;
    static const std::unordered_map<std::string, std::string> DIGRAPH_MAP;
    static const std::unordered_map<std::string, EDirective> DIRECTIVE_MAP;
    static const std::vector<std::string> PUNCTUATOR_VEC; // sort lengest first

    static std::unordered_map<std::string, Macro> MACRO_MAP;

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
    void tokenization();
    // process a preprocessing language
    void processPreprocessingLanguage();
        void includeFile(std::size_t index);
        void defineMacro(std::size_t index);
            bool isValidDefined(std::deque<Token>::iterator& iter,
                                Macro& macro);
        bool expandMacro(std::size_t index);

    bool isEquality(std::size_t index, const Token& token) const;
    bool isEquality(std::size_t index, Token&& token) const;

    std::string mFilename;
    std::string mSource;
    std::deque<Token> mTokens;
    ESearch mESearch;

    bool mIsValid;
};