#pragma once

#include <string>

class Compiler
{
public:
    Compiler();
    ~Compiler();

    bool execute(int argc, char** argv);

private:
    bool mIsValid;
};