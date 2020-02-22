#pragma once

#include <string>

class Preprocessor
{
public:
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
    
    void execute();

private:
    void openSource();
        bool isFindedCurrentPath(std::string& filename) const;
        bool isFindedSystemPath(std::string& filename) const;

    std::string mFilename;
    std::string mDirectory;
    std::string mSource;
    ESearch mESearch;
};