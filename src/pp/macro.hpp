#pragma once

#include "token.hpp"
#include <string>
#include <vector>

class Macro
{
public:
    enum EKind
    {
        OBJECT,
        FUNCTION
    };

    Macro():
        eKind(OBJECT),
        seq(),
        args(),
        isVariadic(false){}
    template<typename Iter>
    Macro(EKind inEKind,
          Iter first,
          Iter last):
        eKind(inEKind),
        seq(first, last),
        args(),
        isVariadic(false){}

    EKind eKind;
    std::vector<Token> seq;
    std::vector<Token> args;
    bool isVariadic;
};