#pragma once

#include "preprocessor.hpp"
#include <string>
#include <unordered_map>

class Initializer
{
private:
    static const std::unordered_map<char, char> TRIGRAPH_MAP;

public:
    Initializer(Preprocessor* pp);

    bool execute();

private:
    void openFile();
        bool isFoundCurrentPath(std::string& result) const;
        bool isFoundSystemPath(std::string& result) const;
    void replaceTrigraph() const;
    void joinLine() const;

    void error(const char* message) const;

    Preprocessor* mPP;
    bool mIsValid;
};