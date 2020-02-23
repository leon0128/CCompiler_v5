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
        bool isFindedCurrentPath(std::string& result) const;
        bool isFindedSystemPath(std::string& result) const;
    void replaceTrigraph() const;
    void joinLine() const;
    void deleteComment() const;
        bool getLine(std::string& line,
                     const std::string& src,
                     std::string::size_type pos = 0) const;

    Preprocessor* mPP;
};