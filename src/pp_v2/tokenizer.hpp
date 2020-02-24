#pragma once

#include <string>

class Tokenizer
{
public:
    Tokenizer(class Preprocessor* pp);

    void execute() const;

private:
    class Digit* conDigit();
    class HexadecimalDigit* conHexadecimalDigit();
    class HexQuad* conHexQuad();
    class Identifier* conIdentifier(class Identifier* bef = nullptr);
    class IdentifierNondigit* conIdentifierNondigit();
    class Nondigit* conNondigit();
    class UniversalCharacterName* conUniversalCharacterName();

    class Preprocessor* mPP;
    std::size_t mIdx;
};