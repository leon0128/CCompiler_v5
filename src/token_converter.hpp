#pragma once

#include "token.hpp"

class TokenConverter
{
public:
    TokenConverter() = delete;

    static Token* convert(PreprocessingToken*);
};