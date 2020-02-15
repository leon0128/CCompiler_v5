#pragma once

#include "token.hpp"
#include <string>
#include <deque>

class Macro
{
public:
    enum EKind
    {
        OBJECT,
        FUNCTION
    };

    Macro(EKind inEKind,
          std::deque<Token>::iterator first,
          std::deque<Token>::iterator last,
          std::size_t inNumArgs = 0):
        eKind(OBJECT),
        tokens(first, last),
        numArgs(inNumArgs){}
        
    EKind eKind;
    std::deque<Token> tokens;
    std::size_t numArgs;
};