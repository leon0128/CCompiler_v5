#pragma once

#include <unordered_map>
#include <string>

class Initializer
{
private:
    static const std::unordered_map<std::string, char> TRIGRAPH_MAP;

public:
    Initializer(class Preprocessor* pp);

    void execute() const;

private:
    void openSource() const;
        bool isFoundCurrentPath(std::string& result) const;
        bool isFoundSystemPath(std::string& result) const;
    void replaceTrigraph() const;
    void joinLine() const;

    Preprocessor* mPP;
};