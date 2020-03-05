#pragma once

#include <string>
#include <vector>
#include <utility>

class Initializer;
class PPTokenizer;
class PPDirectiveTokenizer;

class Preprocessor
{
public:
    friend Initializer;
    friend PPTokenizer;
    friend PPDirectiveTokenizer;

    enum ESearch
    {
        CURRENT_ONLY,
        CURRENT_AND_SYSTEM,
        SYSTEM_ONLY
    };

    Preprocessor();
    ~Preprocessor();

    bool execute(const std::string& file,
                 const std::string& dir,
                 ESearch eSearch = CURRENT_ONLY);
    bool execute(std::string&& file,
                 std::string&& dir = std::string(),
                 ESearch eSearch = CURRENT_ONLY);

private:
    void initialize();
    void ppTokenize();
    void ppDirectiveTokenize();

    void error(const char* message) const;

    Initializer* mInitializer;
    PPTokenizer* mPPTokenizer; 
    PPDirectiveTokenizer* mPPDirectiveTokenizer;

    std::string mFile;
    std::string mDir;
    std::string mSrc;
    ESearch mESearch;

    std::vector<std::pair<class PreprocessingToken*, std::string::size_type>> mPPTokens;

    class PreprocessingFile* mPreprocessingFile;

    bool mIsValid;
};