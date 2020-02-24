#pragma once

#include <vector>

class Symbol;

class Digit;
class HexadecimalDigit;
class HexQuad;
class Identifier;
class IdentifierNondigit;
class Nondigit;
class UniversalCharacterName;

class Symbol
{
private:
    static std::vector<Symbol*> SYMBOLS;

public:
    static void destroy();

    Symbol()
        {SYMBOLS.push_back(this);}
    virtual ~Symbol(){}
};

class Digit : public Symbol
{
public:
    char element;
    
    Digit():
        Symbol(),
        element(0){}
};

class HexadecimalDigit : public Symbol
{
public:
    char element;

    HexadecimalDigit():
        Symbol(),
        element(0){}
};

class HexQuad : public Symbol
{
public:
    HexadecimalDigit* hexadecimalDigits[4];

    HexQuad():
        Symbol(),
        hexadecimalDigits{nullptr, nullptr, nullptr, nullptr}{}
};

class Identifier : public Symbol
{
public:
    enum EIdentifier
    {
        NONE,
        IDENTIFIER_NONDIGIT,
        IDENTIFIER_IDENTIFIER_NODIGIT,
        IDENTIFIER_DIGIT
    } eIdentifier;

    union UIdentifier
    {
        struct SIdentifierNondigit
        {
            IdentifierNondigit* identifierNondigit;
        } sIdentifierNondigit;
        struct SIdentifierIdentifierNondigit
        {
            Identifier* identifier;
            IdentifierNondigit* identifierNondigit;
        } sIdentifierIdentifierNondigit;
        struct SIdentifierDigit
        {
            Identifier* identifier;
            Digit* digit;
        } sIdentifierDigit;

        UIdentifier():
            sIdentifierNondigit(){}
    } uIdentifier;

    Identifier():
        Symbol(),
        eIdentifier(NONE),
        uIdentifier(){}
};

class IdentifierNondigit : public Symbol
{
public:
    enum EIdentifierNondigit
    {
        NONE,
        NONDIGIT,
        UNIVERSAL_CHARACTER_NAME
    } eIdentifierNondigit;

    union UIdentifierNondigit
    {
        struct SNondigit
        {
            Nondigit* nondigit;
        } sNondigit;
        struct SUniversalCharacterName
        {
            UniversalCharacterName* universalCharacterName;
        } sUniversalCharacterName;

        UIdentifierNondigit():
            sNondigit(){}
    } uIdentifierNondigit;

    IdentifierNondigit():
        Symbol(),
        eIdentifierNondigit(NONE),
        uIdentifierNondigit(){}
};

class Nondigit : public Symbol
{
public:
    char element;
    
    Nondigit():
        Symbol(),
        element(0){}
};

class UniversalCharacterName : public Symbol
{
public:
    enum EUniversalCharacterName
    {
        NONE,
        HEX_QUAD,
        HEX_QUAD_HEX_QUAD
    } eUniversalCharacterName;

    union UUniversalCharacterName
    {
        struct SHexQuad
        {
            HexQuad* hexQuad;
        } sHexQuad;
        struct SHexQuadHexQuad
        {
            HexQuad* hexQuad;
            HexQuad* hexQuad_1;
        } sHexQuadHexQuad;

        UUniversalCharacterName():
            sHexQuad(){}
    } uUniversalCharacterName;

    UniversalCharacterName():
        Symbol(),
        eUniversalCharacterName(NONE),
        uUniversalCharacterName(){}
};