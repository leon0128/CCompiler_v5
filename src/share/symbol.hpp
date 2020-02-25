#pragma once

#include <vector>
#include <array>

class Symbol;
class PreprocessingToken_Symbol;

class CharacterConstant;
class Constant;
class DecimalConstant;
class Digit;
class EnumerationConstant;
class FloatingConstant;
class HChar;
class HCharSequence;
class HeaderName;
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
class Other;
class PpNumber;
class Punctuator;
class QChar;
class QCharSequence;
class Sign;
class StringLiteral;
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

class PreprocessingToken_Symbol : public Symbol
{
public:
    enum EPreprocessingToken
    {
        NONE,
        HEADER_NAME,
        IDENTIFIER,
        PP_NUMBER,
        CHARACTER_CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR,
        OTHER
    } ePreprocessingToken;

    union UPreprocessingToken
    {
        struct SHeaderName
        {
            HeaderName* headerName;
        } sHeaderName;
        struct SIdentifier
        {
            Identifier* identifier;
        } sIdentifier;
        struct SPp_number
        {
            PpNumber* ppNumber;
        } sPpNumber;
        struct SCharacterConstant
        {
            CharacterConstant* characterConstant;
        } sCharacterConstant;
        struct SStringLiteral
        {
            StringLiteral* stringLitera;
        } sStringLiteral;
        struct SPunctuator
        {
            Punctuator* punctuator;
        } sPunctuator;
        struct SOther
        {
            Other* other;
        } sOther;

        UPreprocessingToken():
            sHeaderName{nullptr}{}
    } uPreprocessingToken;

    PreprocessingToken_Symbol():
        ePreprocessingToken(NONE),
        uPreprocessingToken(){}
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

class HChar : public Symbol
{
public:
    char element;

    HChar():
        Symbol(),
        element(0){}
};

class HCharSequence : public Symbol
{
public:
    enum EHCharSequence
    {
        NONE,
        H_CHAR,
        H_CHAR_SEQUENCE_H_CHAR
    } eHCharSequence;

    union UHCharSequence
    {
        struct SHChar
        {
            HChar* hChar;
        } sHChar;
        struct SHCharSequenceHChar
        {
            HCharSequence* hCharSequence;
            HChar* hChar;
        } sHCharSequenceHChar;

        UHCharSequence():
            sHChar{nullptr}{}
    } uHCharSequence;

    HCharSequence():
        Symbol(),
        eHCharSequence(NONE),
        uHCharSequence(){}
};

class HeaderName : public Symbol
{
public:
    enum EHeaderName
    {
        NONE,
        H_CHAR_SEQUENCE,
        Q_CHAR_SEQUENCE
    } eHeaderName;

    union UHeaderName
    {
        struct SHCharSequence
        {
            HCharSequence* hCharSequence;
        } sHCharSequence;
        struct SQCharSequence
        {
            QCharSequence* qCharSequence;
        } sQCharSequence;

        UHeaderName():
            sHCharSequence{nullptr}{}
    } uHeaderName;

    HeaderName():
        Symbol(),
        eHeaderName(NONE),
        uHeaderName(){}
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

class Other : public Symbol
{
public:
    char element;

    Other():
        Symbol(),
        element(0){}
};

class PpNumber : public Symbol
{
public:
    enum EPpNumber
    {
        NONE,
        DIGIT,
        DOT_DIGIT,
        PP_NUMBER_DIGIT,
        PP_NUMBER_IDENTIFIER_NONDIGIT,
        PP_NUMBER_e_SIGN,
        PP_NUMBER_E_SIGN,
        PP_NUMBER_p_SIGN,
        PP_NUMBER_P_SIGN,
        PP_NUMBER_DOT
    } ePpNumber;

    union UPpNumber
    {
        struct SDigit
        {
            Digit* digit;
        } sDigit;
        struct SDotDigit
        {
            Digit* digit;
        } sDotDigit;
        struct SPpNumberDigit
        {
            PpNumber* ppNumber;
            Digit* digit;
        } sPpNumberDigit;
        struct SPpNumberIdentifierNondigit
        {
            PpNumber* ppNumber;
            IdentifierNondigit* identifierNondigit;
        } sPpNumberIdentifierNondigit;
        struct SPpNumber_e_Sign
        {
            PpNumber* ppNumber;
            Sign* sign;
        } sPpNumber_e_Sign;
        struct SPpNumber_E_Sign
        {
            PpNumber* ppNumber;
            Sign* sign;
        } sPpNumber_E_Sign;
        struct SPpNumber_p_Sign
        {
            PpNumber* ppNumber;
            Sign* sign;
        } sPpNumber_p_Sign;
        struct SPpNumber_P_Sign
        {
            PpNumber* ppNumber;
            Sign* sign;
        } sPpNumber_P_Sign;
        struct SPpNumberDot
        {
            PpNumber* ppNumber;
        } sPpNumberDot;

        UPpNumber():
            sDigit(){}
    } uPpNumber;

    PpNumber():
        Symbol(),
        ePpNumber(NONE),
        uPpNumber(){}
};

class QChar : public Symbol
{
public:
    char element;

    QChar():
        Symbol(),
        element(0){}
};

class QCharSequence : public Symbol
{
public:
    enum EQCharSequence
    {
        NONE,
        Q_CHAR,
        Q_CHAR_SEQUENCE_Q_CHAR
    } eQCharSequence;

    union UQCharSequence
    {
        struct SQChar
        {
            QChar* qChar;
        } sQChar;
        struct SQCharSequenceQChar
        {
            QCharSequence* qCharSequence;
            QChar* qChar;
        } sQCharSequenceQChar;

        UQCharSequence():
            sQChar{nullptr}{}
    } uQCharSequence;

    QCharSequence():
        Symbol(),
        eQCharSequence(NONE),
        uQCharSequence(){}
};

class Sign : public Symbol
{
public:
    char element;

    Sign():
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
            sHexQuad{nullptr}{}
    } uUniversalCharacterName;

    UniversalCharacterName():
        Symbol(),
        eUniversalCharacterName(NONE),
        uUniversalCharacterName(){}
};