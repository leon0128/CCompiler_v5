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

    template<typename Iter>
    Macro(EKind inEKind,
          Iter first,
          Iter last):
        eKind(inEKind),
        seq(first, last),
        args(){}

    EKind eKind;
    std::vector<Token> seq;
    std::vector<Token> args;
};