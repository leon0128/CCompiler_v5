#pragma once

#include "token.hpp"
#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer(const std::string& src);

    void execute();

private:
    class Digit* conDigit();
    class HexadecimalDigit* conHexadecimalDigit();
    class HexQuad* conHexQuad();
    class Identifier* conIdentifier(class Identifier* bef = nullptr);
    class IdentifierNondigit* conIdentifierNondigit();
    class Nondigit* conNondigit();
    class UniversalCharacterName* conUniversalCharacterName();

    // for each preprocessing token
    void addToken(class Identifier* identifier, std::string::size_type idx);
    
    void process(class Digit* digit, std::string& data) const;
    void process(class HexadecimalDigit* hexadecimalDigit, std::string& data) const;
    void process(class HexQuad* hexQuad, std::string& data) const;
    void process(class Identifier* identifier, std::string& data) const;
    void process(class IdentifierNondigit* identifierNondigit, std::string& data) const;
    void process(class Nondigit* nondigit, std::string& data) const;
    void process(class UniversalCharacterName* universalCharacterName, std::string& data) const;

    void processError(const char* message, const std::string& data) const;

    const std::string& mSrc;
    std::string::size_type mIdx;
    std::vector<class Token*> mTokens;
};