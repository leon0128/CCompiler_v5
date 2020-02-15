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

    Macro(EKind inEKind,
          std::vector<Token>::iterator first,
          std::vector<Token>::iterator last):
        eKind(inEKind),
        seq(first, last),
        args(){}

    EKind eKind;
    std::vector<Token> seq;
    std::vector<Token> args;
};