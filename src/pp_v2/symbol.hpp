#pragma once

#include <vector>

class Symbol;
class Identifier;
class IdentifierNondigit;
class Digit;
class Nondigit;

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

class Identifir : public Symbol
{
public:
    enum EIdentifier
    {
        NONE,
        IDENTIFIER_NODIGIT,
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

    Identifir():
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
        NONDIGIT
    } eIdentifierNondigit;

    union UIdentifierNondigit
    {
        struct SNondigit
        {
            Nondigit* nondigit;
        } sNondigit;

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

class Digit : public Symbol
{
public:
    char element;
    
    Digit():
        Symbol(),
        element(0){}
};