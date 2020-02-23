#pragma once

#include <string>

class Preprocessor
{
public:
    friend class Initializer;

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

    std::string mFile;
    std::string mDir;
    std::string mSrc;
    ESearch mESearch;
};