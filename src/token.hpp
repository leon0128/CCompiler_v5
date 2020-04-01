#pragma once

#include <vector>
#include <string>

class BaseToken;

class BinaryExponentPart;
class CChar;
class CCharSequence;
class CharacterConstant;
class Constant;
class ControlLine;
class DecimalConstant;
class DecimalFloatingConstant;
class Digit;
class DigitSequence;
class ElifGroup;
class ElifGroups;
class ElseGroup;
class EndifLine;
class EncodingPrefix;
class EnumerationConstant;
class EscapeSequence;
class ExponentPart;
class FloatingConstant;
class FloatingSuffix;
class FractionalConstant;
class Group;
class GroupPart;
class HChar;
class HCharSequence;
class HeaderName;
class HexadecimalConstant;
class HexadecimalDigit;
class HexadecimalDigitSequence;
class HexadecimalEscapeSequence;
class HexadecimalFloatingConstant;
class HexadecimalFractionalConstant;
class HexadecimalPrefix;
class HexQuad;
class Identifier;
class IdentifierList;
class IdentifierNondigit;
class IfGroup;
class IfSection;
class IntegerConstant;
class IntegerSuffix;
class Keyword;
class Lparen;
class NewLine;
class Nondigit;
class NonDirective;
class NonzeroDigit;
class OctalConstant;
class OctalDigit;
class OctalEscapeSequence;
class PPNumber;
class PPTokens;
class PreprocessingFile;
class PreprocessingToken;
class Punctuator;
class QChar;
class QCharSequence;
class ReplacementList;
class SChar;
class SCharSequence;
class Sign;
class SimpleEscapeSequence;
class StringLiteral;
class TextLine;
class Token;
class UniversalCharacterName;

namespace TOKEN
{
    void error(const char* message);

    void getString(CChar*, std::string& data);
    void getString(CCharSequence*, std::string& data);
    void getString(CharacterConstant*, std::string& data);
    void getString(ControlLine*, std::string& data);
    void getString(Digit*, std::string& digit);
    void getString(ElifGroup*, std::string& data);
    void getString(ElifGroups*, std::string& data);
    void getString(ElseGroup*, std::string& data);
    void getString(EndifLine*, std::string& data);
    void getString(EncodingPrefix*, std::string& data);
    void getString(EscapeSequence*, std::string& data);
    void getString(Group*, std::string& data);
    void getString(GroupPart*, std::string& data);
    void getString(HChar*, std::string& data);
    void getString(HCharSequence*, std::string& data);
    void getString(HeaderName*, std::string& data);
    void getString(HexadecimalDigit*, std::string& data);
    void getString(HexadecimalEscapeSequence*, std::string& data);
    void getString(HexQuad*, std::string& data);
    void getString(Identifier*, std::string& data);
    void getString(IdentifierList*, std::string& data);
    void getString(IdentifierNondigit*, std::string& data);
    void getString(IfGroup*, std::string& data);
    void getString(IfSection*, std::string& data);
    void getString(Lparen*, std::string& data);
    void getString(NewLine*, std::string& data);
    void getString(Nondigit*, std::string& data);
    void getString(NonDirective*, std::string& data);
    void getString(OctalDigit*, std::string& data);
    void getString(OctalEscapeSequence*, std::string& data);
    void getString(PPNumber*, std::string& data);
    void getString(PPTokens*, std::string& data);
    void getString(PreprocessingFile*, std::string& data);
    void getString(PreprocessingToken*, std::string& data);
    void getString(Punctuator*, std::string& data);
    void getString(QChar*, std::string& data);
    void getString(QCharSequence*, std::string& data);
    void getString(ReplacementList*, std::string& data);
    void getString(SChar*, std::string& data);
    void getString(SCharSequence*, std::string& data);
    void getString(Sign*, std::string& data);
    void getString(SimpleEscapeSequence*, std::string& data);
    void getString(StringLiteral*, std::string& data);
    void getString(TextLine*, std::string& data);
    void getString(UniversalCharacterName*, std::string& data);
};

class BaseToken
{
protected:
    BaseToken(){}
    virtual ~BaseToken(){}

public:
    static void destroy(){}
};

class BinaryExponentPart : public BaseToken
{
public:
    enum EBinaryExponentPart
    {
        NONE,
        p_SIGN_DIGIT_SEQUENCE,
        P_SIGN_DIGIT_SEQUENCE
    } eBinaryExponentPart;

    union UBinaryExponentPart
    {
        struct S_p_SignDigitSequence
        {
            Sign* sign;
            DigitSequence* digitSequence;
        } s_p_SignDigitSequence;
        struct S_P_SignDigitSequence
        {
            Sign* sign;
            DigitSequence* digitSequence;
        } s_P_SignDigitSequence;

        UBinaryExponentPart():
            s_p_SignDigitSequence{nullptr, nullptr}{}
    } uBinaryExponentPart;

    BinaryExponentPart():
        BaseToken(),
        eBinaryExponentPart(NONE),
        uBinaryExponentPart(){}
};

class CChar : public BaseToken
{
public:
    enum ECChar
    {
        NONE,
        ANY_MEMBER,
        ESCAPE_SEQUENCE
    } eCChar;

    union UCChar
    {
        struct SAnyMember
        {
            char element;
        } sAnyMember;
        struct SEscapeSequence
        {
            EscapeSequence* escapeSequence;
        } sEscapeSequence;

        UCChar():
            sAnyMember{0}{}
    } uCChar;

    CChar():
        BaseToken(),
        eCChar(NONE),
        uCChar(){}
};

class CCharSequence : public BaseToken
{
public:
    enum ECCharSequence
    {
        NONE,
        C_CHAR,
        C_CHAR_SEQUENCE_C_CHAR
    } eCCharSequence;

    union UCCharSequence
    {
        struct SCChar
        {
            CChar* cChar;
        } sCChar;
        struct SCCharSequenceCChar
        {
            CCharSequence* cCharSequence;
            CChar* cChar;
        } sCCharSequenceCChar;

        UCCharSequence():
            sCChar{nullptr}{}
    } uCCharSequence;

    CCharSequence():
        BaseToken(),
        eCCharSequence(NONE),
        uCCharSequence(){}
};

class CharacterConstant : public BaseToken
{
public:
    enum ECharacterConstant
    {
        NONE,
        C_CHAR_SEQUENCE,
        L_C_CHAR_SEQUENCE,
        u_C_CHAR_SEQUENCE,
        U_C_CHAR_SEQUENCE
    } eCharacterConstant;

    union UCharacterConstant
    {
        struct SCCharSequence
        {
            CCharSequence* cCharSequence;
        } sCCharSequence;
        struct S_L_CCharSequence
        {
            CCharSequence* cCharSequence;
        } s_L_CCharSequence;
        struct S_u_CCharSequence
        {
            CCharSequence* cCharSequence;
        } s_u_CCharSequence;
        struct S_U_CCharSequence
        {
            CCharSequence* cCharSequence;
        } s_U_CCharSequence;

        UCharacterConstant():
            sCCharSequence{nullptr}{}
    } uCharacterConstant;

    CharacterConstant():
        BaseToken(),
        eCharacterConstant(NONE),
        uCharacterConstant(){}
};

class Constant : public BaseToken
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
        BaseToken(),
        eConstant(NONE),
        uConstant(){}
};

class ControlLine : public BaseToken
{
public:
    enum EControlLine
    {
        NONE,
        INCLUDE_PP_TOKENS_NEW_LINE,
        DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE,
        DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE,
        DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE,
        DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE,
        UNDEF_IDENTIFIER_NEW_LINE,
        LINE_PP_TOKENS_NEW_LINE,
        ERROR_PP_TOKENS_NEW_LINE,
        PRAGMA_PP_TOKENS_NEW_LINE,
        NEW_LINE
    } eControlLine;

    union UControlLine
    {
        struct SIncludePPTokensNewLine
        {
            PPTokens* ppTokens;
            NewLine* newLine;
        } sIncludePPTokensNewLine;
        struct SDefineIdentifierReplacementListNewLine
        {
            Identifier* identifier;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierReplacementListNewLine;
        struct SDefineIdentifierLparenIdentifierListRparenReplacementListNewLine
        {
            Identifier* identifier;
            Lparen* lparen;
            IdentifierList* identifierList;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine;
        struct SDefineIdentifierLparenDotdotdotRparenReplacementListNewLine
        {
            Identifier* identifier;
            Lparen* lparen;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine;
        struct SDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine
        {
            Identifier* identifier;
            Lparen* lparen;
            IdentifierList* identifierList;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine;
        struct SUndefIdentifierNewLine
        {
            Identifier* identifier;
            NewLine* newLine;
        } sUndefIdentifierNewLine;
        struct SLinePPTokensNewLine
        {
            PPTokens* ppTokens;
            NewLine* newLine;
        } sLinePPTokensNewLine;
        struct SErrorPpTokensNewLine
        {
            PPTokens* ppTokens;
            NewLine* newLine;
        } sErrorPPtokensNewLine;
        struct SPragmaPPTokensNewLine
        {
            PPTokens* ppTokens;
            NewLine* newLine;
        } sPragmaPPTokensNewLine;
        struct SNewLine
        {
            NewLine* newLine;
        } sNewLine;

        UControlLine():
            sIncludePPTokensNewLine{nullptr, nullptr}{}
    } uControlLine;

    ControlLine():
        BaseToken(),
        eControlLine(NONE),
        uControlLine(){}
};

class DecimalConstant : public BaseToken
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
        BaseToken(),
        eDecimalConstant(NONE),
        uDecimalConstant(){}
};

class DecimalFloatingConstant : public BaseToken
{
public:
    enum EDecimalFloatingConstant
    {
        NONE,
        FRACTIONAL_CONSTANT_EXPONENT_PART_FLOATING_SUFFIX,
        DIGIT_SEQUENCE_EXPONENT_PART_FLOATING_SUFFIX
    } eDecimalFloatingConstant;

    union UDecimalFloatingConstant
    {
        struct SFractionalConstantExponentPartFloatingSuffix
        {
            FractionalConstant* fractionalConstant;
            ExponentPart* exponentPart;
            FloatingSuffix* floatingSuffix;
        } sFractionalConstantExponentPartFloatingSuffix;
        struct SDigitSequenceExponentPartFloatingSuffix
        {
            DigitSequence* digitSequence;
            ExponentPart* exponentPart;
            FloatingSuffix* floatingSuffix;
        } sDigitSequenceExponentPartFloatingSuffix;

        UDecimalFloatingConstant():
            sFractionalConstantExponentPartFloatingSuffix{nullptr, nullptr, nullptr}{}
    } uDecimalFloatingConstant;

    DecimalFloatingConstant():
        BaseToken(),
        eDecimalFloatingConstant(NONE),
        uDecimalFloatingConstant(){}
};

class Digit : public BaseToken
{
public:
    char element;
    
    Digit():
        BaseToken(),
        element(0){}
};

class DigitSequence : public BaseToken
{
public:
    enum EDigitSequence
    {
        NONE,
        DIGIT,
        DIGIT_SEQUENCE_DIGIT
    } eDigitSequence;

    union UDigitSequence
    {
        struct SDigit
        {
            Digit* digit;
        } sDigit;
        struct SDigitSequenceDigit
        {
            DigitSequence* digitSequence;
            Digit* digit;
        } sDigitSequenceDigit;
    
        UDigitSequence():
            sDigit{nullptr}{}
    } uDigitSequence;

    DigitSequence():
        BaseToken(),
        eDigitSequence(NONE),
        uDigitSequence(){}
};

class ElifGroup : public BaseToken
{
public:
    PPTokens* ppTokens;
    NewLine* newLine;
    Group* group;

    ElifGroup():
        BaseToken(),
        ppTokens(nullptr),
        newLine(nullptr),
        group(nullptr){}
};

class ElifGroups : public BaseToken
{
public:
    enum EElifGroups
    {
        NONE,
        ELIF_GROUP,
        ELIF_GROUPS_ELIF_GROUP
    } eElifGroups;

    union UElifGroups
    {
        struct SElifGroup
        {
            ElifGroup* elifGroup;
        } sElifGroup;
        struct SElifGroupsElifGroup
        {
            ElifGroups* elifGroups;
            ElifGroup* elifGroup;
        } sElifGroupsElifGroup;

        UElifGroups():
            sElifGroup{nullptr}{}
    } uElifGroups;

    ElifGroups():
        BaseToken(),
        eElifGroups(NONE),
        uElifGroups(){}
};

class ElseGroup : public BaseToken
{
public:
    NewLine* newLine;
    Group* group;

    ElseGroup():
        BaseToken(),
        newLine(nullptr),
        group(nullptr){}
};

class EndifLine : public BaseToken
{
public:
    NewLine* newLine;
    
    EndifLine():
        BaseToken(),
        newLine(nullptr){}
};

class EncodingPrefix : public BaseToken
{
public:
    std::string element;

    EncodingPrefix():
        BaseToken(),
        element(){}
};

class EnumerationConstant : public BaseToken
{
public:
    Identifier* identifier;

    EnumerationConstant():
        BaseToken(),
        identifier(nullptr){}
};

class EscapeSequence : public BaseToken
{
public:
    enum EEscapeSequence
    {
        NONE,
        SIMPLE_ESCAPE_SEQUENCE,
        OCTAL_ESCAPE_SEQUENCE,
        HEXADECIMAL_ESCAPE_SEQUENCE,
        UNIVERSAL_CHARACTER_NAME
    } eEscapeSequence;

    union UEscapeSequence
    {
        struct SSimpleEscapeSequence
        {
            SimpleEscapeSequence* simpleEscapeSequence;
        } sSimpleEscapeSequence;
        struct SOctalEscapeSequence
        {
            OctalEscapeSequence* octalEscapeSequence;
        } sOctalEscapeSequence;
        struct SHexadecimalEscapeSequence
        {
            HexadecimalEscapeSequence* hexadecimalEscapeSequence;
        } sHexadecimalEscapeSequence;
        struct SUniversalCharacterName
        {
            UniversalCharacterName* universalCharacterName;
        } sUniversalCharacterName;

        UEscapeSequence():
            sSimpleEscapeSequence{nullptr}{}
    } uEscapeSequence;

    EscapeSequence():
        BaseToken(),
        eEscapeSequence(NONE),
        uEscapeSequence(){}
};

class ExponentPart : public BaseToken
{
public:
    enum EExponentPart
    {
        NONE,
        e_SIGN_DIGIT_SEQUENCE,
        E_SIGN_DIGIT_SEQUENCE
    } eExponentPart;

    union UExponentPart
    {
        struct S_e_SignDigitSequence
        {
            Sign* sign;
            DigitSequence* digitSequence;
        } s_e_SignDigitSequence;
        struct S_E_SignDigitSequence
        {
            Sign* sign;
            DigitSequence* digitSequence;
        } s_E_SignDigitSequence;
    
        UExponentPart():
            s_e_SignDigitSequence{nullptr, nullptr}{}
    } uExponentPart;

    ExponentPart():
        BaseToken(),
        eExponentPart(NONE),
        uExponentPart(){}
};

class FloatingConstant : public BaseToken
{
public:
    enum EFloatingConstant
    {
        NONE,
        DECIMAL_FLOATING_CONSTANT,
        HEXADECIMAL_FLOATING_CONSTANT
    } eFloatingConstant;

    union UFloatingConstant
    {
        struct SDecimalFloatingConstant
        {
            DecimalFloatingConstant* decimalFloatingConstant;
        } sDecimalFloatingConstant;
        struct SHexadecimalFloatingConstant
        {
            HexadecimalFloatingConstant* hexadecimalFloatingConstant;
        } sHexadecimalFloatingConstant;

        UFloatingConstant():
            sDecimalFloatingConstant{nullptr}{}
    } uFloatingConstant;

    FloatingConstant():
        BaseToken(),
        eFloatingConstant(NONE),
        uFloatingConstant(){}
};

class FloatingSuffix : public BaseToken
{
public:
    char element;

    FloatingSuffix():
        BaseToken(),
        element(0){}
};

class FractionalConstant : public BaseToken
{
public:
    enum EFractionalConstant
    {
        NONE,
        DIGIT_SEQUENCE_DIGIT_SEQUENCE,
        DIGIT_SEQUENCE
    } eFractionalConstant;

    union UFractionalConstant
    {
        struct SDigitSequenceDigitSequence
        {
            DigitSequence* digitSequence;
            DigitSequence* digitSequence_1;
        } sDigitSequenceDigitSequence;
        struct SDigitSequence
        {
            DigitSequence* digitSequence;
        } sDigitSequence;

        UFractionalConstant():
            sDigitSequenceDigitSequence{nullptr, nullptr}{}
    } uFractionalConstant;

    FractionalConstant():
        BaseToken(),
        eFractionalConstant(NONE),
        uFractionalConstant(){}
};

class Group : public BaseToken
{
public:
    enum EGroup
    {
        NONE,
        GROUP_PART,
        GROUP_GROUP_PART
    } eGroup;

    union UGroup
    {
        struct SGroupPart
        {
            GroupPart* groupPart;
        } sGroupPart;
        struct SGroupGroupPart
        {
            Group* group;
            GroupPart* groupPart;
        } sGroupGroupPart;

        UGroup():
            sGroupGroupPart{nullptr}{}
    } uGroup;

    Group():
        BaseToken(),
        eGroup(NONE),
        uGroup(){}
};

class GroupPart : public BaseToken
{
public:
    enum EGroupPart
    {
        NONE,
        IF_SECTION,
        CONTROL_LINE,
        TEXT_LINE,
        NON_DIRECTIVE
    } eGroupPart;

    union UGroupPart
    {
        struct SIfSection
        {
            IfSection* ifSection;
        } sIfSection;
        struct SControlLine
        {
            ControlLine* controlLine;
        } sControlLine;
        struct STextLine
        {
            TextLine* textLine;
        } sTextLine;
        struct SNonDirective
        {
            NonDirective* nonDirective;
        } sNonDirective;

        UGroupPart():
            sIfSection{nullptr}{}
    } uGroupPart;

    GroupPart():
        BaseToken(),
        eGroupPart(NONE),
        uGroupPart(){}
};

class HChar : public BaseToken
{
public:
    char element;

    HChar():
        BaseToken(),
        element(0){}
};

class HCharSequence : public BaseToken
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
        BaseToken(),
        eHCharSequence(NONE),
        uHCharSequence(){}
};

class HeaderName : public BaseToken
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
        BaseToken(),
        eHeaderName(NONE),
        uHeaderName(){}
};

class HexadecimalDigit : public BaseToken
{
public:
    char element;

    HexadecimalDigit():
        BaseToken(),
        element(0){}
};

class HexadecimalDigitSequence : public BaseToken
{
public:
    enum EHexadecimalDigitSequence
    {
        NONE,
        HEXADECIMAL_DIGIT,
        HEXADECIMAL_DIGIT_SEQUENCE_HEXADECIMAL_DIGIT
    } eHexadecimalDigitSequence;

    union UHexadecimalDigitSequence
    {
        struct SHexadecimalDigit
        {
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalDigit;
        struct SHexadecimalDigitSequenceHexadecimalDigit
        {
            HexadecimalDigitSequence* hexadecimalDigitSequence;
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalDigitSequenceHexadecimalDigit;

        UHexadecimalDigitSequence():
            sHexadecimalDigit{nullptr}{}
    } uHexadecimalDigitSequence;

    HexadecimalDigitSequence():
        BaseToken(),
        eHexadecimalDigitSequence(NONE),
        uHexadecimalDigitSequence(){}
};

class HexadecimalEscapeSequence : public BaseToken
{
public:
    enum EHexadecimalEscapeSequence
    {
        NONE,
        HEXADECIMAL_DIGIT,
        HEXADECIMAL_ESCAPE_SEQUENCE_HEXADECIMAL_DIGIT
    } eHexadecimalEscapeSequence;

    union UHexadecimalEscapeSequence
    {
        struct SHexadecimalDigit
        {
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalDigit;
        struct SHexadecimalEscapeSequenceHexadecimalDigit
        {
            HexadecimalEscapeSequence* hexadecimalEscapeSequence;
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalEscapeSequenceHexadecimalDigit;

        UHexadecimalEscapeSequence():
            sHexadecimalDigit{nullptr}{}
    } uHexadecimalEscapeSequence;

    HexadecimalEscapeSequence():
        BaseToken(),
        eHexadecimalEscapeSequence(NONE),
        uHexadecimalEscapeSequence(){}
};

class HexadecimalFloatingConstant : public BaseToken
{
public:
    enum EHexadecimalFloatingConstant
    {
        NONE,
        HEXADECIMAL_PREFIX_HEXADECIMAL_FRACTIONAL_CONSTANT_BINARY_EXPONENT_PART_FLOATING_SUFFIX,
        HEXADECIMAL_PREFIX_HEXADECIMAL_DIGIT_SEQUENCE_BINARY_EXPONENT_PART_FLOATING_SUFFIX
    } eHexadecimalFloatingConstant;

    union UHexadecimalFloatingConstant
    {
        struct SHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix
        {
            HexadecimalPrefix* hexadecimalPrefix;
            HexadecimalFractionalConstant* hexadecimalFractionalConstant;
            BinaryExponentPart* binaryExponentPart;
            FloatingSuffix* floatingSuffix;
        } sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix;
        struct SHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix
        {
            HexadecimalPrefix* hexadecimalPrefix;
            HexadecimalDigitSequence* hexadecimalDigitSequence;
            BinaryExponentPart* binaryExponentPart;
            FloatingSuffix* floatingSuffix;
        } sHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix;
    
        UHexadecimalFloatingConstant():
            sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix{nullptr, nullptr, nullptr, nullptr}{}
    } uHexadecimalFloatingConstant;

    HexadecimalFloatingConstant():
        BaseToken(),
        eHexadecimalFloatingConstant(NONE),
        uHexadecimalFloatingConstant(){}
};

class HexadecimalFractionalConstant : public BaseToken
{
public:
    enum EHexadecimalFractionalConstant
    {
        NONE,
        HEXADECIMAL_DIGIT_SEQUENCE_HEXADECIMAL_DIGIT_SEQUENCE,
        HEXADECIMAL_DIGIT_SEQUENCE
    } eHexadecimalFractionalConstant;

    union UHexadecimalFractionalConstant
    {
        struct SHexadecimalDigitSequenceHexadecimalDigitSequence
        {
            HexadecimalDigitSequence* hexadecimalDigitSequence;
            HexadecimalDigitSequence* hexadecimalDigitSequence_1;
        } sHexadecimalDigitSequenceHexadecimalDigitSequence;
        struct SHexadecimalDigitSequence
        {
            HexadecimalDigitSequence* hexadecimalDigitSequence;
        } sHexadecimalDigitSequence;

        UHexadecimalFractionalConstant():
            sHexadecimalDigitSequenceHexadecimalDigitSequence{nullptr, nullptr}{}
    } uHexadecimalFractionalConstant;

    HexadecimalFractionalConstant():
        BaseToken(),
        eHexadecimalFractionalConstant(NONE),
        uHexadecimalFractionalConstant(){}
};

class HexadecimalPrefix : public BaseToken
{
public:
    std::string element;

    HexadecimalPrefix():
        BaseToken(),
        element(){}
};

class HexQuad : public BaseToken
{
public:
    HexadecimalDigit* hexadecimalDigit;
    HexadecimalDigit* hexadecimalDigit_1;
    HexadecimalDigit* hexadecimalDigit_2;
    HexadecimalDigit* hexadecimalDigit_3;

    HexQuad():
        BaseToken(),
        hexadecimalDigit(nullptr),
        hexadecimalDigit_1(nullptr),
        hexadecimalDigit_2(nullptr),
        hexadecimalDigit_3(nullptr){}
};

class Identifier : public BaseToken
{
public:
    enum EIdentifier
    {
        NONE,
        IDENTIFIER_NONDIGIT,
        IDENTIFIER_IDENTIFIER_NONDIGIT,
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
        BaseToken(),
        eIdentifier(NONE),
        uIdentifier(){}
};

class IdentifierList : public BaseToken
{
public:
    enum EIdentifierList
    {
        NONE,
        IDENTIFIER,
        IDENTIFIER_LIST_IDENTIFIER
    } eIdentifierList;

    union UIdentifierList
    {
        struct SIdentifier
        {
            Identifier* identifier;
        } sIdentifier;
        struct SIdentifierListIdentifier
        {
            IdentifierList* identifierList;
            Identifier* identifier;
        } sIdentifierListIdentifier;

        UIdentifierList():
            sIdentifier{nullptr}{}
    } uIdentifierList;

    IdentifierList():
        BaseToken(),
        eIdentifierList(NONE),
        uIdentifierList(){}
};

class IdentifierNondigit : public BaseToken
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
        BaseToken(),
        eIdentifierNondigit(NONE),
        uIdentifierNondigit(){}
};

class IfGroup : public BaseToken
{
public:
    enum EIfGroup
    {
        NONE,
        IF_PP_TOKENS_NEW_LINE_GROUP,
        IFDEF_IDENTIFIER_NEW_LINE_GROUP,
        IFNDEF_IDENTIFIER_NEW_LINE_GROUP
    } eIfGroup;

    union UIfGroup
    {
        struct SIfPPTokensNewLineGroup
        {
            PPTokens* ppTokens;
            NewLine* newLine;
            Group* group;
        } sIfPPTokensNewLineGroup;
        struct SIfdefIdentifierNewLineGroup
        {
            Identifier* identifier;
            NewLine* newLine;
            Group* group;
        } sIfdefIdentifierNewLineGroup;
        struct SIfndefIdentifierNewLineGroup
        {
            Identifier* identifier;
            NewLine* newLine;
            Group* group;
        } sIfndefIdentifierNewLineGroup;

        UIfGroup():
            sIfPPTokensNewLineGroup{nullptr, nullptr, nullptr}{}
    } uIfGroup;

    IfGroup():
        BaseToken(),
        eIfGroup(NONE),
        uIfGroup(){}
};

class IfSection : public BaseToken
{
public:
    IfGroup* ifGroup;
    ElifGroups* elifGroups;
    ElseGroup* elseGroup;
    EndifLine* endifLine;

    IfSection():
        BaseToken(),
        ifGroup(nullptr),
        elifGroups(nullptr),
        elseGroup(nullptr),
        endifLine(nullptr){}
};

class IntegerConstant : public BaseToken
{
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
        BaseToken(),
        eIntegerConstant(NONE),
        uIntegerConstant(){}
};

class Keyword : public BaseToken
{
public:
    std::string element;

    Keyword():
        BaseToken(),
        element(){}
};

class Lparen : public BaseToken
{
public:
    char element;

    Lparen():
        BaseToken(),
        element(0){}
};

class NewLine : public BaseToken
{
public:
    char element;

    NewLine():
        BaseToken(),
        element(0){}
};

class Nondigit : public BaseToken
{
public:
    char element;
    
    Nondigit():
        BaseToken(),
        element(0){}
};

class NonDirective : public BaseToken
{
public:
    PPTokens* ppTokens;
    NewLine* newLine;

    NonDirective():
        BaseToken(),
        ppTokens(nullptr),
        newLine(nullptr){}
};

class NonzeroDigit : public BaseToken
{
public:
    char element;

    NonzeroDigit():
        BaseToken(),
        element(0){}
};

class OctalConstant : public BaseToken
{
    enum EOctalConstant
    {
        NONE,
        ZERO,
        OCTAL_CONSTANT_OCTAL_DIGIT
    } eOctalConstant;

    union UOctalConstant
    {
        struct SZero
        {
            char element;
        } sZero;
        struct SOctalConstantOctalDigit
        {
            OctalConstant* octalConstant;
            OctalDigit* octalDigit;
        } sOctalConstantOctalDigit;

        UOctalConstant():
            sZero{0}{}
    } uOctalConstant;

    OctalConstant():
        BaseToken(),
        eOctalConstant(NONE),
        uOctalConstant(){}
};

class OctalDigit : public BaseToken
{
public:
    char element;

    OctalDigit():
        BaseToken(),
        element(0){}
};

class OctalEscapeSequence : public BaseToken
{
public:
    enum EOctalEscapeSequence
    {
        NONE,
        OCTAL_DIGIT,
        OCTAL_DIGIT_OCTAL_DIGIT,
        OCTAL_DIGIT_OCTAL_DIGIT_OCTAL_DIGIT
    } eOctalEscapeSequence;

    union UOctalEscapeSequence
    {
        struct SOctalDigit
        {
            OctalDigit* octalDigit;
        } sOctalDigit;
        struct SOctalDigitOctalDigit
        {
            OctalDigit* octalDigit;
            OctalDigit* octalDigit_1;
        } sOctalDigitOctalDigit;
        struct SOctalDigitOctalDigitOctalDigit
        {
            OctalDigit* octalDigit;
            OctalDigit* octalDigit_1;
            OctalDigit* octalDigit_2;
        } sOctalDigitOctalDigitOctalDigit;

        UOctalEscapeSequence():
            sOctalDigit{nullptr}{}
    } uOctalEscapeSequence;

    OctalEscapeSequence():
        BaseToken(),
        eOctalEscapeSequence(NONE),
        uOctalEscapeSequence(){}
};

class PPNumber : public BaseToken
{
public:
    enum EPPNumber
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
    } ePPNumber;

    union UPPNumber
    {
        struct SDigit
        {
            Digit* digit;
        } sDigit;
        struct SDotDigit
        {
            Digit* digit;
        } sDotDigit;
        struct SPPNumberDigit
        {
            PPNumber* ppNumber;
            Digit* digit;
        } sPPNumberDigit;
        struct SPPNumberIdentifierNondigit
        {
            PPNumber* ppNumber;
            IdentifierNondigit* identifierNondigit;
        } sPPNumberIdentifierNondigit;
        struct SPPNumber_e_Sign
        {
            PPNumber* ppNumber;
            Sign* sign;
        } sPPNumber_e_Sign;
        struct SPPNumber_E_Sign
        {
            PPNumber* ppNumber;
            Sign* sign;
        } sPPNumber_E_Sign;
        struct SPPNumber_p_Sign
        {
            PPNumber* ppNumber;
            Sign* sign;
        } sPPNumber_p_Sign;
        struct SPPNumber_P_Sign
        {
            PPNumber* ppNumber;
            Sign* sign;
        } sPPNumber_P_Sign;
        struct SPPNumberDot
        {
            PPNumber* ppNumber;
        } sPPNumberDot;

        UPPNumber():
            sDigit(){}
    } uPPNumber;

    PPNumber():
        BaseToken(),
        ePPNumber(NONE),
        uPPNumber(){}
};

class PPTokens : public BaseToken
{
public:
    enum EPPTokens
    {
        NONE,
        PREPROCESSING_TOKEN,
        PP_TOKENS_PREPROCESSING_TOKEN
    } ePPTokens;

    union UPPTokens
    {
        struct SPreprocessingToken
        {
            PreprocessingToken* preprocessingToken;
        } sPreprocessingToken;
        struct SPPTokensPreprocessingToken
        {
            PPTokens* ppTokens;
            PreprocessingToken* preprocessingToken;
        } sPPTokensPreprocessingToken;

        UPPTokens():
            sPreprocessingToken{nullptr}{}
    } uPPTokens;

    PPTokens():
        BaseToken(),
        ePPTokens(NONE),
        uPPTokens(){}
};

class PreprocessingFile : public BaseToken
{
public:
    Group* group;

    PreprocessingFile():
        BaseToken(),
        group(nullptr){}
};

class PreprocessingToken : public BaseToken
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
        struct SPP_number
        {
            PPNumber* ppNumber;
        } sPPNumber;
        struct SCharacterConstant
        {
            CharacterConstant* characterConstant;
        } sCharacterConstant;
        struct SStringLiteral
        {
            StringLiteral* stringLiteral;
        } sStringLiteral;
        struct SPunctuator
        {
            Punctuator* punctuator;
        } sPunctuator;
        struct SOther
        {
            char element;
        } sOther;

        UPreprocessingToken():
            sHeaderName{nullptr}{}
    } uPreprocessingToken;

    PreprocessingToken():
        BaseToken(),
        ePreprocessingToken(NONE),
        uPreprocessingToken(){}
};

class Punctuator : public BaseToken
{
public:
    std::string element;

    Punctuator():
        BaseToken(),
        element(){}
};

class QChar : public BaseToken
{
public:
    char element;

    QChar():
        BaseToken(),
        element(0){}
};

class QCharSequence : public BaseToken
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
        BaseToken(),
        eQCharSequence(NONE),
        uQCharSequence(){}
};

class ReplacementList : public BaseToken
{
public:
    PPTokens* ppTokens;

    ReplacementList():
        BaseToken(),
        ppTokens(nullptr){}
};

class SChar : public BaseToken
{
public:
    enum ESChar
    {
        NONE,
        ANY_MEMBER,
        ESCAPE_SEQUENCE
    } eSChar;

    union USChar
    {
        struct SAnyMember
        {
            char element;
        } sAnyMember;
        struct SEscapeSequence
        {
            EscapeSequence* escapeSequence;
        } sEscapeSequence;

        USChar():
            sAnyMember{0}{}
    } uSChar;

    SChar():
        BaseToken(),
        eSChar(NONE),
        uSChar(){}
};

class SCharSequence : public BaseToken
{
public:
    enum ESCharSequence
    {
        NONE,
        S_CHAR,
        S_CHAR_SEQUENCE_S_CHAR
    } eSCharSequence;

    union USCharSequence
    {
        struct SSChar
        {
            SChar* sChar;
        } sSChar;
        struct SSCharSequenceSChar
        {
            SCharSequence* sCharSequence;
            SChar* sChar;
        } sSCharSequenceSChar;

        USCharSequence():
            sSChar{nullptr}{}
    } uSCharSequence;

    SCharSequence():
        BaseToken(),
        eSCharSequence(NONE),
        uSCharSequence(){}
};

class Sign : public BaseToken
{
public:
    char element;

    Sign():
        BaseToken(),
        element(0){}
};

class SimpleEscapeSequence : public BaseToken
{
public:
    char element;

    SimpleEscapeSequence():
        BaseToken(),
        element(0){}
};

class StringLiteral : public BaseToken
{
public:
    EncodingPrefix* encodingPrefix;
    SCharSequence* sCharSequence;

    StringLiteral():
        BaseToken(),
        encodingPrefix(nullptr),
        sCharSequence(nullptr){}
};

class TextLine : public BaseToken
{
public:
    PPTokens* ppTokens;
    NewLine* newLine;

    TextLine():
        BaseToken(),
        ppTokens(nullptr),
        newLine(nullptr){}
};

class Token : public BaseToken
{
public:
    enum EToken
    {
        NONE,
        KEYWORD,
        IDENTIFIER,
        CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR
    } eToken;

    union UToken
    {
        struct SKeyword
        {
            Keyword* keyword;
        } sKeyword;
        struct SIdentifier
        {
            Identifier* identifier;
        } sIdentifier;
        struct SConstant
        {
            Constant* constant;
        } sConstant;
        struct SStringLiteral
        {
            StringLiteral* stringLiteral;
        } sStringLiteral;
        struct SPunctuator
        {
            Punctuator* punctuator;
        } sPunctuator;

        UToken():
            sKeyword{nullptr}{}
    } uToken;

    Token():
        BaseToken(),
        eToken(NONE),
        uToken(){}
};

class UniversalCharacterName : public BaseToken
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
        BaseToken(),
        eUniversalCharacterName(NONE),
        uUniversalCharacterName(){}
};