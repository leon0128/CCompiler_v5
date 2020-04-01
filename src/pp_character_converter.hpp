#pragma once

#include "token.hpp"
#include <string>

class Preprocessor;

class PPCharacterConverter
{
public:
    PPCharacterConverter(Preprocessor* pp);

    bool execute();

private:
    void convert(PreprocessingToken*);

    bool simpleEscapeSequence(std::string& data, std::size_t idx);
    bool octalEscapeSequence(std::string& data, std::size_t idx);
    bool hexadecimalEscapeSequence(std::string& data, std::size_t idx);
    bool universalCharacterName(std::string& data, std::size_t idx);

    bool isMatched(const std::string& data,
                   std::size_t idx,
                   char c);

    Preprocessor* mPP;
    bool mIsValid;
};