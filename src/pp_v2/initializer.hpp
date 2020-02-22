#pragma once

#include <string>

class Initializer
{
public:
    Initializer(class Preprocessor* pp);

    void execute() const;

private:
    void openSource() const;
        bool isFindedCurrentPath(std::string& result) const;
        bool isFindedSystemPath(std::string& result) const;

    Preprocessor* mPP;
};