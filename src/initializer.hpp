#pragma once

#include "preprocessor.hpp"
#include <string>
#include <unordered_map>

class Initializer
{
private:
    static const std::unordered_map<char, char> TRIGRAPH_MAP;

public:
    Initializer();

    bool execute(std::string& ppfile,
                 std::string& ppdir,
                 Preprocessor::ESearch eSearch);

private:
    void openFile(Preprocessor::ESearch eSearch);
        bool isFoundCurrentPath(std::string& result) const;
        bool isFoundSystemPath(std::string& result) const;
    void replaceTrigraph();
    void joinLine();

    void error(const char* message) const;

    std::string mFile;
    std::string mDir;
    std::string mSrc;

    bool mIsValid;
};