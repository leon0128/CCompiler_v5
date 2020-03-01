#pragma once

#include <string>

class Preprocessor
{
public:
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

    void error(const char* message) const;

    class Initializer* mInitializer;

    std::string mFile;
    std::string mDir;
    ESearch mESearch;

    bool mIsValid;
};