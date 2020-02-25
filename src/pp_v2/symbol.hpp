#pragma once

#include <vector>
#include <array>

class Symbol;

class CharacterConstant;
class Constant;
class DecimalConstant;
class Digit;
class EnumerationConstant;
class FloatingConstant;
class HexadecimalConstant;
class HexadecimalDigit;
class HexQuad;
class Identifier;
class IdentifierNondigit;
class IntegerConstant;
class IntegerSuffix;
class Nondigit;
class NonzeroDigit;
class OctalConstant;
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

class Constant : public Symbol
{
public:
    enum EConstant
    {
        NONE,
        INTEGER_CONSTANT,
        FLOATING_CONSTANT,
        ENUMERATION_CONSTANT,
        CHARACTER_CONSTANT
    } eConstant;

    union UConstant
    {
        struct SIntegerConstant
        {
            IntegerConstant* integerConstant;
        } sIntegerConstant;
        struct SFloatingConstant
        {
            FloatingConstant* floatingConstant;
        } sFloatingConstant;
        struct SEnumerationConstant
        {
            EnumerationConstant* enumerationConstant;
        } sEnumerationConstant;
        struct SCharacterConstant
        {
            CharacterConstant* characterConstant;
        } sCharacterConstant;

        UConstant():
            sIntegerConstant{nullptr}{}
    } uConstant;

    Constant():
        Symbol(),
        eConstant(NONE),
        uConstant(){}
};

class DecimalConstant : public Symbol
{
public:
    enum EDecimalConstant
    {
        NONE,
        NONZERO_DIGIT,
        DECIMAL_CONSTANT_DIGIT
    } eDecimalConstant;

    union UDecimalConstant
    {
        struct SNonzeroDigit
        {
            NonzeroDigit* nonzeroDigit;
        } sNonzeroDigit;
        struct SDecimalConstantDigit
        {
            DecimalConstant* decimalConstant;
            Digit* digit;
        } sDecimalConstantDigit;

        UDecimalConstant():
            sNonzeroDigit{nullptr}{}
    } uDecimalConstant;

    DecimalConstant():
        eDecimalConstant(NONE),
        uDecimalConstant(){}
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
    std::array<HexadecimalDigit*, 4> hexadecimalDigits;

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
            sIdentifierNondigit{nullptr}{}
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
            sNondigit{nullptr}{}
    } uIdentifierNondigit;

    IdentifierNondigit():
        Symbol(),
        eIdentifierNondigit(NONE),
        uIdentifierNondigit(){}
};

class IntegerConstant : public Symbol
{
public:
    enum EIntegerConstant
    {
        NONE,
        DECIMAL_CONSTANT_INTEGER_SUFFIX,
        OCTAL_CONSTANT_INTEGER_SUFFIX,
        HEXADECIMAL_CONSTANT_INTEGER_SUFFIX
    } eIntegerConstant;

    union UIntegerConstant
    {
        struct SDecimalConstantIntegerSuffix
        {
            DecimalConstant* decimalConstant;
            IntegerSuffix* integerSuffix;
        } sDecimalConstantIntegerSuffix;
        struct SOctalConstantIntegerSuffix
        {
            OctalConstant* octalConstant;
            IntegerSuffix* integerSuffix;
        } sOctalConstantIntegerSuffix;
        struct SHexadecimalConstantIntegerSuffix
        {
            HexadecimalConstant* hexadecimalConstant;
            IntegerSuffix* integerSuffix;
        } sHexadecimalConstantIntegerSuffix;

        UIntegerConstant():
            sDecimalConstantIntegerSuffix{nullptr, nullptr}{}
    } uIntegerConstant;

    IntegerConstant():
        Symbol(),
        eIntegerConstant(NONE),
        uIntegerConstant(){}
};

class Nondigit : public Symbol
{
public:
    char element;
    
    Nondigit():
        Symbol(),
        element(0){}
};

// class OctalConstant : public 

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
            sHexQuad{nullptr}{}
    } uUniversalCharacterName;

    UniversalCharacterName():
        Symbol(),
        eUniversalCharacterName(NONE),
        uUniversalCharacterName(){}
};