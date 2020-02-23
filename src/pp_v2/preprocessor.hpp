#pragma once

#include <string>
#include <vector>

class Preprocessor
{
public:
    friend class Initializer;
    friend class Tokenizer;

    // kind of search
    enum ESearch
    {
        CURRENT_ONLY,
        CURRENT_AND_SYSTEM,
        SYSTEM_ONLY
    };

    Preprocessor(const std::string& filename,
                 const std::string& directory,
                 ESearch eSearch = CURRENT_ONLY);
    ~Preprocessor();
    
    void execute();

private:

    class Initializer* mInitializer;
    class Tokenizer* mTokenizer;

    // for initializer
    std::string mFile;
    std::string mDir;
    std::string mSrc;
    ESearch mESearch;
    // for tokenizer
    std::vector<class Token*> mTokens;
};