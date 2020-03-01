#pragma once

#include <vector>
#include <string>
#include <array>

class PreprocessingToken;

class Symbol;
class PreprocessingToken_Symbol;

class AdditiveExpression;
class ANDExpression;
class ArgumentExpressionList;
class AssignmentExpression;
class AssignmentOperator;
class BinaryExponentPart;
class CastExpression;
class CChar;
class CCharSequence;
class CharacterConstant;
class ConditionalExpression;
class Constant;
class ConstantExpression;
class ControlLine;
class DecimalConstant;
class DecimalFloatingConstant;
class Designation;
class Designator;
class DesignatorList;
class Digit;
class DigitSequence;
class ElifGroup;
class ElifGroups;
class ElseGroup;
class EncodingPrefix;
class EndifLine;
class EnumerationConstant;
class EqualityExpression;
class EscapeSequence;
class ExclusiveORExpression;
class ExponentPart;
class Expression;
class FloatingConstant;
class FloatingSuffix;
class FractionalConstant;
class GenericAssociation;
class GenericAssocList;
class GenericSelection;
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
class InclusiveORExpression;
class Initializer;
class InitializerList;
class IntegerConstant;
class IntegerSuffix;
class LogicalANDExpression;
class LogicalORExpression;
class LongSuffix;
class LongLongSuffix;
class Lparen;
class MultiplicativeExpression;
class NewLine;
class Nondigit;
class NonDirective;
class NonzeroDigit;
class OctalConstant;
class OctalDigit;
class OctalEscapeSequence;
class Other;
class PreprocessingFile;
class PrimaryExpression;
class PostfixExpression;
class PpNumber;
class PpTokens;
class Punctuator;
class QChar;
class QCharSequence;
class RelationalExpression;
class ReplacementList;
class SChar;
class SCharSequence;
class ShiftExpression;
class Sign;
class SimpleEscapeSequence;
class StringLiteral;
class TextLine;
class TypeName;
class UnaryExpression;
class UnaryOperator;
class UniversalCharacterName;
class UnsignedSuffix;

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
            StringLiteral* stringLiteral;
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
        Symbol(),
        ePreprocessingToken(NONE),
        uPreprocessingToken(){}
};

class AdditiveExpression : public Symbol
{
public:
    enum EAdditiveExpression
    {
        NONE,
        NUMTIPLECATIVE_EXPRESSION,
        ADDITIVE_EXPRESSION_PLUS_MULTIPLICATIVE_EXPRESSION,
        ADDITIVE_EXPRESSION_MINUS_MUITIPLICATIVE_EXPRESSION
    } eAdditiveExpression;

    union UAdditiveExpression
    {
        struct SMultiplicativeExpression
        {
            MultiplicativeExpression* multiplicativeExpression;
        } sMultiplicativeExpression;
        struct SAdditiveExpressionPlusMultiplicativeExpression
        {
            AdditiveExpression* additiveExpression;
            MultiplicativeExpression* multiplicativeExpression;
        } sAdditiveExpressionPlusMultiplicativeExpression;
        struct SAdditiveExpressionMinusMultiplicativeExpression
        {
            AdditiveExpression* additiveExpression;
            MultiplicativeExpression* multiplicativeExpression;
        } sAdditiveExpressionMinusMultiplicativeExpression;

        UAdditiveExpression():
            sMultiplicativeExpression{nullptr}{}
    } uAdditiveExpression;

    AdditiveExpression():
        Symbol(),
        eAdditiveExpression(NONE),
        uAdditiveExpression(){}
};

class ANDExpression : public Symbol
{
public:
    enum EANDExpression
    {
        NONE,
        EQUALITY_EXPRESSION,
        AND_EXPRESSION_EQUALITY_EXPRESSION
    } eANDExpression;

    union UANDExpression
    {
        struct SEqualityExpression
        {
            EqualityExpression* equalityExpression;
        } sEqualityExpression;
        struct SANDExpressionEqualityExpression
        {
            ANDExpression* andExpression;
            EqualityExpression* equalityExpression;
        } sANDExpressionEqualityExpression;
    
        UANDExpression():
            sEqualityExpression{nullptr}{}
    } uANDExpression;

    ANDExpression():
        Symbol(),
        eANDExpression(NONE),
        uANDExpression(){}
};

class ArgumentExpressionList : public Symbol
{
public:
    enum EArgumentExpressionList
    {
        NONE,
        ASSIGNMENT_EXPRESSION,
        ARGUMENT_EXPRESSION_LIST_ASSIGNMENT_EXPRESSION
    } eArgumentExpressionList;

    union UArgumentExpressionList
    {
        struct SAssignmentExpression
        {
            AssignmentExpression* assignmentExpression;
        } sAssignmentExpression;
        struct SArgumentExpressionListAssignmentExpression
        {
            ArgumentExpressionList* argumentExpressionList;
            AssignmentExpression* assignmentExpression;
        } sArgumentExpressionListAssignmentExpression;

        UArgumentExpressionList():
            sAssignmentExpression{nullptr}{}
    } uArgumentExpressionList;

    ArgumentExpressionList():
        Symbol(),
        eArgumentExpressionList(NONE),
        uArgumentExpressionList(){}
};

class AssignmentExpression : public Symbol
{
public:
    enum EAssignmentExpression
    {
        NONE,
        CONDITIONAL_EXPRESSION,
        UNARY_EXPRESSION_ASSIGNMENT_OPERATOR_ASSIGNMENT_EXPRESSION
    } eAssignmentExpression;

    union UAssignmentExpression
    {
        struct SConditionalExpression
        {
            ConditionalExpression* conditionalExpression;
        } sConditionalExpression;
        struct SUnaryExpressionAssignmentOperatorAssignmentExpression
        {
            UnaryExpression* unaryExpression;
            AssignmentOperator* assignmentOperator;
            AssignmentExpression* assignmentExpression;
        } sUnaryExpressionAssignmentOperatorAssignmentExpression;

        UAssignmentExpression():
            sConditionalExpression{nullptr}{}
    } uAssignmentExpression;

    AssignmentExpression():
        Symbol(),
        eAssignmentExpression(NONE),
        uAssignmentExpression(){}
};

class AssignmentOperator : public Symbol
{
public:
    std::string element;

    AssignmentOperator():
        Symbol(),
        element(){}
};

class BinaryExponentPart : public Symbol
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
        Symbol(),
        eBinaryExponentPart(NONE),
        uBinaryExponentPart(){}
};

class CastExpression : public Symbol
{
public:
    enum ECastExpression
    {
        NONE,
        UNARY_EXPRESSION,
        TYPE_NAME_CAST_EXPRESSION
    } eCastExpression;

    union UCastExpression
    {
        struct SUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sUnaryExpression;
        struct STypeNameCastExpression
        {
            TypeName* typeName;
            CastExpression* castExpression;
        } sTypeNameCastExpression;

        UCastExpression():
            sUnaryExpression{nullptr}{}
    } uCastExpression;

    CastExpression():
        Symbol(),
        eCastExpression(NONE),
        uCastExpression(){}
};

class CChar : public Symbol
{
public:
    enum ECChar
    {
        NONE,
        ANY_MEMBER,
        ESCAPE_CEQUENCE
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
        Symbol(),
        eCChar(NONE),
        uCChar(){}
};

class CCharSequence : public Symbol
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
        Symbol(),
        eCCharSequence(NONE),
        uCCharSequence(){}
};

class CharacterConstant : public Symbol
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
        Symbol(),
        eCharacterConstant(NONE),
        uCharacterConstant(){}
};

class ConditionalExpression : public Symbol
{
public:
    enum EConditionalExpression
    {
        NONE,
        LOGICAL_OR_EXPRESSION,
        LOGICAL_OR_EXPRESSION_EXPRESSION_CONDITIONAL_EXPRESSION
    } eConditionalExpression;

    union UConditionalExpression
    {
        struct SLogicalORExpression
        {
            LogicalORExpression* logicalORExpression;
        } sLogicalORExpression;
        struct SLogicalORExpressionExpressionConditionalExpression
        {
            LogicalORExpression* logicalORExpression;
            Expression* expression;
            ConditionalExpression* conditionalExpression;
        } sLogicalORExpressionExpressionConditionalExpression;

        UConditionalExpression():
            sLogicalORExpression{nullptr}{}
    } uConditionalExpression;

    ConditionalExpression():
        Symbol(),
        eConditionalExpression(NONE),
        uConditionalExpression(){}
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

class ConstantExpression : public Symbol
{
public:
    ConditionalExpression* conditionalExpression;
    
    ConstantExpression():
        Symbol(),
        conditionalExpression(nullptr){}
};

class ControlLine : public Symbol
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
        struct SIncludePpTokensNewLine
        {
            PpTokens* ppTokens;
            NewLine* newLine;
        } sIncludePpTokensNewLine;
        struct SDefineIdentifierReplacementListNewLine
        {
            PreprocessingToken* identifier;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierReplacementListNewLine;
        struct SDefineIdentifierLparenIdentifierListRparenReplacementListNewLine
        {
            PreprocessingToken* identifier;
            Lparen* lparen;
            IdentifierList* identifierList;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine;
        struct SDefineIdentifierLparenDotdotdotRparenReplacementListNewLine
        {
            PreprocessingToken* identifier;
            Lparen* lparen;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine;
        struct SDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine
        {
            PreprocessingToken* identifier;
            Lparen* lparen;
            IdentifierList* identifierList;
            ReplacementList* replacementList;
            NewLine* newLine;
        } sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine;
        struct SUndefIdentifierNewLine
        {
            PreprocessingToken* identifier;
            NewLine* newLine;
        } sUndefIdentifierNewLine;
        struct SLinePpTokensNewLine
        {
            PpTokens* ppTokens;
            NewLine* newLine;
        } sLinePpTokensNewLine;
        struct SErrorPpTokensNewLine
        {
            PpTokens* ppTokens;
            NewLine* newLine;
        } sErrorPptokensNewLine;
        struct SPragmaPpTokensNewLine
        {
            PpTokens* ppTokens;
            NewLine* newLine;
        } sPragmaPpTokensNewLine;
        struct SNewLine
        {
            NewLine* newLine;
        } sNewLine;

        UControlLine():
            sIncludePpTokensNewLine{nullptr, nullptr}{}
    } uControlLine;

    ControlLine():
        Symbol(),
        eControlLine(NONE),
        uControlLine(){}
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
        Symbol(),
        eDecimalConstant(NONE),
        uDecimalConstant(){}
};

class DecimalFloatingConstant : public Symbol
{
public:
    enum EDecimalFloatingConstant
    {
        NONE,
        FRACTIONAL_CONSTANT_EXPONENT_PART_FLOATING_SUFFIX,
        DIIT_SEQUENCE_EXPONENT_PART_FLOATING_SUFFIX
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
        Symbol(),
        eDecimalFloatingConstant(NONE),
        uDecimalFloatingConstant(){}
};

class Designation : public Symbol
{
public:
    DesignatorList* designatorList;

    Designation():
        Symbol(),
        designatorList(nullptr){}
};

class Designator : public Symbol
{
public:
    enum EDesignator
    {
        NONE,
        CONSTANT_EXPRESSION,
        IDENTIFIER
    } eDesignator;

    union UDesignator
    {
        struct SConstantExpression
        {
            ConstantExpression* constantExpression;
        } sConstantExpression;
        struct SIdentifier
        {
            PreprocessingToken* identifier;
        } sIdentifier;

        UDesignator():
            sConstantExpression{nullptr}{}
    } uDesignator;

    Designator():
        Symbol(),
        eDesignator(NONE),
        uDesignator(){}
};

class DesignatorList : public Symbol
{
public:
    enum EDesignatorList
    {
        NONE,
        DESIGNATOR,
        DESIGNATOR_LIST_DESIGNATOR
    } eDesignatorList;

    union UDesignatorList
    {
        struct SDesignator
        {
            Designator* designator;
        } sDesignator;
        struct SDesignatorListDesignator
        {
            DesignatorList* designatorList;
            Designator* designator;
        } sDesignatorListDesignator;

        UDesignatorList():
            sDesignator{nullptr}{}
    } uDesignatorList;

    DesignatorList():
        Symbol(),
        eDesignatorList(NONE),
        uDesignatorList(){}
};

class Digit : public Symbol
{
public:
    char element;
    
    Digit():
        Symbol(),
        element(0){}
};

class DigitSequence : public Symbol
{
public:
    enum EDigitSequence
    {
        NONE,
        DIGIT,
        DIGIT_SEQUENCE_DiGIT
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
        Symbol(),
        eDigitSequence(NONE),
        uDigitSequence(){}
};

class ElifGroup : public Symbol
{
public:
    ConstantExpression* constantExpression;
    NewLine* newLine;
    Group* group;

    ElifGroup():
        Symbol(),
        constantExpression(nullptr),
        newLine(nullptr),
        group(nullptr){}
};

class ElifGroups : public Symbol
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
        Symbol(),
        eElifGroups(NONE),
        uElifGroups(){}
};

class ElseGroup : public Symbol
{
public:
    NewLine* newLine;
    Group* group;

    ElseGroup():
        Symbol(),
        newLine(nullptr),
        group(nullptr){}
};

class EncodingPrefix : public Symbol
{
public:
    std::string element;

    EncodingPrefix():
        Symbol(),
        element(){}
};

class EndifLine : public Symbol
{
public:
    NewLine* newLine;
    
    EndifLine():
        Symbol(),
        newLine(nullptr){}
};

class EnumerationConstant : public Symbol
{
public:
    PreprocessingToken* identifier;

    EnumerationConstant():
        Symbol(),
        identifier(nullptr){}
};

class EqualityExpression : public Symbol
{
public:
    enum EEqualityExpression
    {
        NONE,
        RELATIONAL_EXPRESSION,
        EQUALITY_EXPRESSION_EQUAL_RELATIONAL_EXPRESSION,
        EQUALITY_EXPRESSION_NOT_EQUAL_RELATIONAL_EXPRESSION
    } eEqualityExpression;

    union UEqualityExpression
    {
        struct SRelationalExpression
        {
            RelationalExpression* relationalExpression;
        } sRelationalExpression;
        struct SEqualityExpressionEqualRelationalExpression
        {
            EqualityExpression* equalityExpression;
            RelationalExpression* relationalExpression;
        } sEqualityExpressionEqualRelationalExpression;
        struct SEqualityExpressionNotEqualRelationalExpression
        {
            EqualityExpression* equalityExpression;
            RelationalExpression* relationalExpression;
        } sEqualityExpressionNotEqualRelationalExpression;

        UEqualityExpression():
            sRelationalExpression{nullptr}{}
    } uEqualityExpression;

    EqualityExpression():
        Symbol(),
        eEqualityExpression(NONE),
        uEqualityExpression(){}
};

class EscapeSequence : public Symbol
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
        Symbol(),
        eEscapeSequence(NONE),
        uEscapeSequence(){}
};

class ExclusiveORExpression : public Symbol
{
public:
    enum EExclusiveORExpression
    {
        NONE,
        AND_EXPRESSION,
        EXCLUSIVE_OR_EXPRESSION_AND_EXPRESSION
    } eExclusiveORExpression;

    union UExclusiveORExpression
    {
        struct SANDExpression
        {
            ANDExpression* andExpression;
        } sANDExpression;
        struct SExclusiveORExpressionANDExpression
        {
            ExclusiveORExpression* exclusiveORExpression;
            ANDExpression* andExpression;
        } sExclusiveORExpressionANDExpression;

        UExclusiveORExpression():
            sANDExpression{nullptr}{}
    } uExclusiveORExpression;

    ExclusiveORExpression():
        Symbol(),
        eExclusiveORExpression(NONE),
        uExclusiveORExpression(){}
};

class ExponentPart : public Symbol
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
        Symbol(),
        eExponentPart(NONE),
        uExponentPart(){}
};

class Expression : public Symbol
{
public:
    enum EExpression
    {
        NONE,
        ASSIGNMENT_EXPRESSION,
        EXPRESSION_ASSIGNMENT_EXPRESSION
    } eExpression;

    union UExpression
    {
        struct SAssignmentExpression
        {
            AssignmentExpression* assignmentExpression;
        } sAssignmentExpression;
        struct SExpressionAssignmentExpression
        {
            Expression* expression;
            AssignmentExpression* assignmentExpression;
        } sExpressionAssignmentExpression;

        UExpression():
            sAssignmentExpression{nullptr}{}
    } uExpression;

    Expression():
        Symbol(),
        eExpression(NONE),
        uExpression(){}
};

class FloatingConstant : public Symbol
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
        Symbol(),
        eFloatingConstant(NONE),
        uFloatingConstant(){}
};

class FloatingSuffix : public Symbol
{
public:
    char element;

    FloatingSuffix():
        Symbol(),
        element(0){}
};

class FractionalConstant : public Symbol
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
        Symbol(),
        eFractionalConstant(NONE),
        uFractionalConstant(){}
};

class GenericAssociation : public Symbol
{
public:
    enum EGenericAssociation
    {
        NONE,
        TYPE_NAME_ASSIGNMENT_EXPRESSION,
        DEFAULT_ASSIGNMENT_EXPRESSION
    } eGenericAssociation;

    union UGenericAssociation
    {
        struct STypeNameAssignmentExpression
        {
            TypeName* typeName;
            AssignmentExpression* assignmentExpression;
        } sTypeNameAssignmentExpression;
        struct SDefaultAssignmentExpression
        {
            AssignmentExpression* assignmentExpression;
        } sDefaultAssignmentExpression;

        UGenericAssociation():
            sTypeNameAssignmentExpression{nullptr, nullptr}{}
    } uGenericAssociation;

    GenericAssociation():
        Symbol(),
        eGenericAssociation(NONE),
        uGenericAssociation(){}
};

class GenericAssocList : public Symbol
{
public:
    enum EGenericAssocList
    {
        NONE,
        GENERIC_ASSOCIATION,
        GENERIC_ASSOC_LIST_GENERIC_ASSOCIATION
    } eGenericAssocList;

    union UGenericAssocList
    {
        struct SGenericAssociation
        {
            GenericAssociation* genericAssociation;
        } sGenericAssociation;
        struct SGenericAssocListGenericAssociation
        {
            GenericAssocList* genericAssocList;
            GenericAssociation* genericAssociation;
        } sGenericAssocListGenericAssociation;

        UGenericAssocList():
            sGenericAssociation{nullptr}{}
    } uGenericAssocList;

    GenericAssocList():
        Symbol(),
        eGenericAssocList(NONE),
        uGenericAssocList(){}
};

class GenericSelection : public Symbol
{
public:
    AssignmentExpression* assignmentExpression;
    GenericAssocList* genericAssocList;

    GenericSelection():
        Symbol(),
        assignmentExpression(nullptr),
        genericAssocList(nullptr){}
};

class Group : public Symbol
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
        Symbol(),
        eGroup(NONE),
        uGroup(){}
};

class GroupPart : public Symbol
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
        Symbol(),
        eGroupPart(NONE),
        uGroupPart(){}
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

class HexadecimalFloatingConstant : public Symbol
{
public:
    enum EHexadecimalFloatingConstant
    {
        NONE,
        HEXADECIMAL_PREFIX_HEXADECIMAL_FRACTIONAL_CONSTANT_BINARY_EXPONENT_PART_FLOATING_SUFFIX,
        HEXADECImAL_PREFIX_HEXADECIMAL_DIGIT_SEQUENCE_BINARY_EXPONENT_PART_FLOATING_SUFFIX
    } eHexadecimalFloatingConstant;

    struct UHexadecimalFloatingConstant
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
        Symbol(),
        eHexadecimalFloatingConstant(NONE),
        uHexadecimalFloatingConstant(){}
};

class HexadecimalConstant : public Symbol
{
public:
    enum EHexadecimalConstant
    {
        NONE,
        HEXADECIMAL_PREFIX_HEXADECIMAL_DIGIT,
        HEXADECIMAL_CONSTANT_HEXADECIMAL_DIGIT
    } eHexadecimalConstant;

    union UHexadecimalConstant
    {
        struct SHexadecimalPrefixHexadecimalDigit
        {
            HexadecimalPrefix* hexadecimalPrefix;
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalPrefixHeaxadecimalDigit;
        struct SHexadecimalConstantHexadecimalDigit
        {
            HexadecimalConstant* hexadecimalConstant;
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalConstantHexadecimalDigit;

        UHexadecimalConstant():
            sHexadecimalPrefixHeaxadecimalDigit{nullptr, nullptr}{}
    } uHexadecimalConstant;

    HexadecimalConstant():
        Symbol(),
        eHexadecimalConstant(NONE),
        uHexadecimalConstant(){}
};

class HexadecimalDigit : public Symbol
{
public:
    char element;

    HexadecimalDigit():
        Symbol(),
        element(0){}
};

class HexadecimalDigitSequence : public Symbol
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
        Symbol(),
        eHexadecimalDigitSequence(NONE),
        uHexadecimalDigitSequence(){}
};

class HexadecimalEscapeSequence : public Symbol
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
        Symbol(),
        eHexadecimalEscapeSequence(NONE),
        uHexadecimalEscapeSequence(){}
};

class HexadecimalFractionalConstant : public Symbol
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
        Symbol(),
        eHexadecimalFractionalConstant(NONE),
        uHexadecimalFractionalConstant(){}
};

class HexadecimalPrefix : public Symbol
{
public:
    std::string element;

    HexadecimalPrefix():
        Symbol(),
        element(){}
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

class IdentifierList : public Symbol
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
            PreprocessingToken* identifier;
        } sIdentifier;
        struct SIdentifierListIdentifier
        {
            IdentifierList* identifierList;
            PreprocessingToken* identifier;
        } sIdentifierListIdentifier;

        UIdentifierList():
            sIdentifier{nullptr}{}
    } uIdentifierList;

    IdentifierList():
        Symbol(),
        eIdentifierList(NONE),
        uIdentifierList(){}
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

class IntegerSuffix : public Symbol
{
public:
    enum EIntegerSuffix
    {
        NONE,
        UNSIGNED_SUFFIX_LONG_SUFFIX,
        UNSIGNED_SUFFIX_LONG_LONG_SUFFIX,
        LONG_SUFFIX_UNSIGNED_SUFFIX,
        LONG_LONG_SUFFIX_UNSIGNED_SUFFIX
    } eIntegerSuffix;

    union UIntegerSuffix
    {
        struct SUnsignedSuffixLongSuffix
        {
            UnsignedSuffix* unsignedSuffix;
            LongSuffix* longSuffix;
        } sUnsignedSuffixLongSuffix;
        struct SUnsignedSuffixLongLongSuffix
        {
            UnsignedSuffix* unsignedSuffix;
            LongLongSuffix* longLongSuffix;
        } sUnsignedSuffixLongLongSuffix;
        struct SLongSuffixUnsignedSuffix
        {
            LongSuffix* longSuffix;
            UnsignedSuffix* unsignedSuffix;
        } sLongSuffixUnsignedSuffix;
        struct SLongLongSuffixUnsignedSuffix
        {
            LongLongSuffix* longLongSuffix;
            UnsignedSuffix* unsignedSuffix;
        } sLongLongSuffixUnsignedSuffix;

        UIntegerSuffix():
            sUnsignedSuffixLongLongSuffix{nullptr, nullptr}{}
    } uIntegerSuffix;

    IntegerSuffix():
        Symbol(),
        eIntegerSuffix(NONE),
        uIntegerSuffix(){}
};

class IfGroup : public Symbol
{
public:
    enum EIfGroup
    {
        NONE,
        IF_CONSTANT_EXPRESSION_NEW_LINE_GROUP,
        IFDEF_IDENTIFIER_NEW_LINE_GROUP,
        IFNDEF_IDENTIFIER_NEW_LINE_GROUP
    } eIfGroup;

    union UIfGroup
    {
        struct SIfConstantExpressionNewLineGroup
        {
            ConstantExpression* constantExpression;
            NewLine* newLine;
            Group* group;
        } sIfConstantExpressionNewLineGroup;
        struct SIfdefIdentifierNewLineGroup
        {
            PreprocessingToken* identifier;
            NewLine* newLine;
            Group* group;
        } sIfdefIdentifierNewLineGroup;
        struct SIfndefIdentifierNewLineGroup
        {
            PreprocessingToken* identifier;
            NewLine* newLine;
            Group* group;
        } sIfndefIdentifierNewLineGroup;

        UIfGroup():
            sIfConstantExpressionNewLineGroup{nullptr, nullptr, nullptr}{}
    } uIfGroup;

    IfGroup():
        Symbol(),
        eIfGroup(NONE),
        uIfGroup(){}
};

class IfSection : public Symbol
{
public:
    IfGroup* ifGroup;
    ElifGroups* elifGroups;
    ElseGroup* elseGroup;
    EndifLine* endifLine;

    IfSection():
        Symbol(),
        ifGroup(nullptr),
        elifGroups(nullptr),
        elseGroup(nullptr),
        endifLine(nullptr){}
};

class InclusiveORExpression : public Symbol
{
public:
    enum EInclusiveORExpression
    {
        NONE,
        EXCLUSIVE_OR_EXPRESSION,
        INCLUSIVE_OR_EXPRESSION_EXCLUSIVE_OR_EXPRESSION
    } eInclusiveORExpression;

    union UInclusiveORExpression
    {
        struct SExclusiveORExpression
        {
            ExclusiveORExpression* exclusiveORExpression;
        } sExclusiveORExpression;
        struct SInclusiveORExpressionExclusiveORExpression
        {
            InclusiveORExpression* inclusiveORExpression;
            ExclusiveORExpression* exclusiveORExpression;
        } sInclusiveORExpressionExclusiveORExpression;

        UInclusiveORExpression():
            sExclusiveORExpression{nullptr}{}
    } uInclusiveORExpression;

    InclusiveORExpression():
        Symbol(),
        eInclusiveORExpression(NONE),
        uInclusiveORExpression(){}
};

class Initializer : public Symbol
{
public:
    enum EInitializer
    {
        NONE,
        ASSIGNMENT_EXPRESSION,
        INITIALIZER_LIST,
        INITIALIZER_LIST_COMMA
    } eInitializer;

    union UInitializer
    {
        struct SAssignmentExpression
        {
            AssignmentExpression* assignmentExpression;
        } sAssignmentExpression;
        struct SInitializerList
        {
            InitializerList* initializerList;
        } sInitializerList;
        struct SInitializerListComma
        {
            InitializerList* initializerList;
        } sInitializerListComma;

        UInitializer():
            sAssignmentExpression{nullptr}{}
    } uInitializer;

    Initializer():
        Symbol(),
        eInitializer(NONE),
        uInitializer(){}
};

class InitializerList : public Symbol
{
public:
    enum EInitializerList
    {
        NONE,
        DESIGNATION_INITIALIZER,
        INITIALIZER_LIST_DESIGNATION_INITIALIZER
    } eInitializerList;

    union UInitializerList
    {
        struct UDesignationInitializer
        {
            Designation* designation;
            Initializer* initializer;
        } uDesignationInitializer;
        struct UInitializerListDesignationInitializer
        {
            InitializerList* initializerList;
            Designation* designation;
            Initializer* initializer;
        } uInitializerListDesignationInitializer;

        UInitializerList():
            uDesignationInitializer{nullptr, nullptr}{}
    } uInitializerList;

    InitializerList():
        Symbol(),
        eInitializerList(NONE),
        uInitializerList(){}
};

class LogicalANDExpression : public Symbol
{
public:
    enum ELogicalANDExpression
    {
        NONE,
        INCLUSIVE_OR_EXPRESSION,
        LOGICAL_AND_EXPRESSION_INCLUSIVE_OR_EXPRESSION
    } eLogicalANDExpression;

    union ULogicalANDExpression
    {
        struct SInclusiveORExpression
        {
            InclusiveORExpression* inclusiveORExpression;
        } sInclusiveORExpression;
        struct SLogicalANDExpressionInclusiveORExpression
        {
            LogicalANDExpression* logicalANDExpression;
            InclusiveORExpression* inclusiveORExpression;
        } sLogicalANDExpressionInclusiveORExpression;

        ULogicalANDExpression():
            sInclusiveORExpression{nullptr}{}
    } uLogicalANDExpression;

    LogicalANDExpression():
        Symbol(),
        eLogicalANDExpression(NONE),
        uLogicalANDExpression(){}
};

class LogicalORExpression : public Symbol
{
public:
    enum ELogicalORExpression
    {
        NONE,
        LOGICAL_AND_EXPRESSION,
        LOGICAL_OR_EXPRESSION_LOGICAL_AND_EXPRESSION
    } eLogicalORExpression;

    union ULogicalORExpression
    {
        struct SLogicalANDExpression
        {
            LogicalANDExpression* logicalANDExpression;
        } sLogicalANDExpression;
        struct SLogicalORExpressionLogicalANDExpression
        {
            LogicalORExpression* logicalORExpression;
            LogicalANDExpression* logicalANDExpression;
        } sLogicalOrExpressionLogicalANDExpression;

        ULogicalORExpression():
            sLogicalANDExpression{nullptr}{}
    } uLogicalORExpression;

    LogicalORExpression():
        Symbol(),
        eLogicalORExpression(NONE),
        uLogicalORExpression(){}
};

class LongSuffix : public Symbol
{
public:
    char element;

    LongSuffix():
        Symbol(),
        element(0){}
};

class LongLongSuffix : public Symbol
{
    std::string element;

    LongLongSuffix():
        Symbol(),
        element(){}
};

class Lparen : public Symbol
{
public:
    char element;

    Lparen():
        Symbol(),
        element(0){}
};

class MultiplicativeExpression : public Symbol
{
public:
    enum EMultiplicativeExpression
    {
        NONE,
        CAST_EXPRESSION,
        MULTIPLICATIVE_EXPRESSION_ASTERISK_CAST_EXPRESSION,
        MULTIPLICATIVE_EXPRESSION_SLASH_CAST_EXPRESSION,
        MULTIPLICATIVE_EXPRESSION_PERCENT_CAST_EXPRESSION
    } eMultiplicativeExpression;

    union UMultiplicativeExpression
    {
        struct SCastExpression
        {
            CastExpression* castExpression;
        } sCastExpression;
        struct SMultiplicativeExpressionAsteriskCastExpression
        {
            MultiplicativeExpression* multiplicativeExpression;
            CastExpression* castExpression;
        } sMultiplicativeExpressionAsteriskCastExpression;
        struct SMultiplicativeExpressionSlashCastExpression
        {
            MultiplicativeExpression* multiplicativeExpression;
            CastExpression* castExpression;
        } sMultiplicativeExpressionSlashCastExpression;
        struct SMultiplicativeExpressionPercentCastExpression
        {
            MultiplicativeExpression* multiplicativeExpression;
            CastExpression* castExpression;
        } sMultiplicativeExpressionPercentCastExpression;
    
        UMultiplicativeExpression():
            sCastExpression{nullptr}{}
    } uMultiplicativeExpression;

    MultiplicativeExpression():
        Symbol(),
        eMultiplicativeExpression(NONE),
        uMultiplicativeExpression(){}
};

class NewLine : public Symbol
{
public:
    char element;

    NewLine():
        Symbol(),
        element(0){}
};

class Nondigit : public Symbol
{
public:
    char element;
    
    Nondigit():
        Symbol(),
        element(0){}
};

class NonDirective : public Symbol
{
public:
    PpTokens* ppTokens;
    NewLine* newLine;

    NonDirective():
        Symbol(),
        ppTokens(nullptr),
        newLine(nullptr){}
};

class NonzeroDigit : public Symbol
{
public:
    char element;

    NonzeroDigit():
        Symbol(),
        element(0){}
};

class OctalConstant : public Symbol
{
public:
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
        struct SOctalConstant
        {
            OctalConstant* octalConstant;
            OctalDigit* octalDigit;
        } sOctalConstant;

        UOctalConstant():
            sZero{0}{}
    } uOctalConstant;

    OctalConstant():
        Symbol(),
        eOctalConstant(NONE),
        uOctalConstant(){}
};

class OctalDigit : public Symbol
{
public:
    char element;

    OctalDigit():
        Symbol(),
        element(0){}
};

class OctalEscapeSequence : public Symbol
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
        Symbol(),
        eOctalEscapeSequence(NONE),
        uOctalEscapeSequence(){}
};

class Other : public Symbol
{
public:
    char element;

    Other():
        Symbol(),
        element(0){}
};

class PreprocessingFile : public Symbol
{
public:
    Group* group;

    PreprocessingFile():
        Symbol(),
        group(nullptr){}
};

class PrimaryExpression : public Symbol
{
public:
    enum EPrimaryExpression
    {
        NONE,
        IDENTIFIER,
        CONSTANT,
        STRING_LITERAL,
        EXPRESSION,
        GENERIC_SELECTION
    } ePrimaryExpression;

    union UPrimaryExpression
    {
        struct SIdentifier
        {
            PreprocessingToken* identifier;
        } sIdentifier;
        struct SConstant
        {
            Constant* constant;
        } sConstant;
        struct SStringLiteral
        {
            PreprocessingToken* stringLiteral;
        } sStringLiteral;
        struct SExpression
        {
            Expression* expression;
        } sExpression;
        struct SGenericSelection
        {
            GenericSelection* genericSelection;
        } sGenericSelection;

        UPrimaryExpression():
            sIdentifier{nullptr}{}
    } uPrimaryExpression;

    PrimaryExpression():
        Symbol(),
        ePrimaryExpression(NONE),
        uPrimaryExpression(){}
};

class PostfixExpression : public Symbol
{
public:
    enum EPostfixExpression
    {
        NONE,
        PRIMARY_EXPRESSION,
        POSTFIX_EXPRESSION_LSQUARE_EXPRESSION_RSQUALE,
        POSTFIX_EXPRESSION_LPAREN_ARGUMENT_EXPRESSION_LIST_RPAREN,
        POSTFIX_EXPRESSION_DOT_IDENTIFIER,
        POSTFIX_EXPRESSION_ARROW_IDENTIFIER,
        POSTFIX_EXPRESSION_PLUSPLUS,
        POSTFIX_EXPRESSION_MINUSMINUS,
        TYPE_NAME_INITIALIZER_LIST,
        TYPE_NAME_INITIALIZER_LIST_COMMA
    } ePostfixExpression;

    union UPostfixExpression
    {
        struct SPrimaryExpression
        {
            PrimaryExpression* primaryExpression;
        } sPrimaryExpression;
        struct SPostfixExpressionLsquareExpressionRsquare
        {
            PostfixExpression* postfixExpression;
            Expression* expression;
        } sPostfixExpressionLsquareExpressionRsquare;
        struct SPostfixExpressionArgumentExpressionList
        {
            PostfixExpression* postfixExpression;
            ArgumentExpressionList* argumentExpressionList;
        } sPostfixExpressionArgumentExpressionList;
        struct SPostfixExpressionDotIdentifier
        {
            PostfixExpression* postfixExpression;
            PreprocessingToken* identifier;
        } sPostfixExpressionDotIdentifier;
        struct SPostfixExpressionArrowIdentifier
        {
            PostfixExpression* postfixExpression;
            PreprocessingToken* identifier;
        } sPostfixExpressionArrowIdentifier;
        struct SPostfixExpressionPlusplus
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpressionPlusplus;
        struct SPostfixExpressionMinusminus
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpressionMinusminus;
        struct STypeNameInitializerList
        {
            TypeName* typeName;
            InitializerList* initializerList;
        } sTypeNameInitializerList;
        struct STypeNameInitializerListComma
        {
            TypeName* typeName;
            InitializerList* initializerList;
        } sTypeNameInitializerListComma;

        UPostfixExpression():
            sPrimaryExpression{nullptr}{}
    } uPostfixExpression;

    PostfixExpression():
        Symbol(),
        ePostfixExpression(NONE),
        uPostfixExpression(){}
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

class PpTokens : public Symbol
{
public:
    enum EPpTokens
    {
        NONE,
        PREPROCESSING_TOKEN,
        PP_TOKENS_PREPROCESSING_TOKEN
    } ePpTokens;

    union UPpTokens
    {
        struct SPreprocessingToken
        {
            PreprocessingToken* preprocessingToken;
        } sPreprocessingToken;
        struct SPpTokensPreprocessingToken
        {
            PpTokens* ppTokens;
            PreprocessingToken* preprocessingToken;
        } sPpTokensPreprocessingToken;

        UPpTokens():
            sPreprocessingToken{nullptr}{}
    } uPpTokens;

    PpTokens():
        Symbol(),
        ePpTokens(NONE),
        uPpTokens(){}
};

class Punctuator : public Symbol
{
public:
    std::string element;

    Punctuator():
        Symbol(),
        element(){}
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

class RelationalExpression : public Symbol
{
public:
    enum ERelationalExpression
    {
        NONE,
        SHIFT_EXPRESSION,
        RELATIONAL_EXPRESSION_LESS_SHIFT_EXPRESSION,
        RELATIONAL_EXPRESSION_GREATER_SHIFT_EXPRESSION,
        RELATIONAL_EXPRESSION_LESS_EQUAL_SHIFT_EXPRESSION,
        RELATIONAL_EXPRESSION_GREATER_EQUAL_SHIFT_EXPRESSION
    } eRelationalExpression;

    union URelationalExpression
    {
        struct SShiftExpression
        {
            ShiftExpression* shiftExpression;
        } sShiftExpression;
        struct SRelationalExpressionLessShiftExpression
        {
            RelationalExpression* relationalExpression;
            ShiftExpression* shiftExpression;
        } sRelationalExpressionLessShiftExpression;
        struct SRelationalExpressionGreaterShiftExpression
        {
            RelationalExpression* relationalExpression;
            ShiftExpression* shiftExpression;
        } sRelationalExpressionGreaterShiftExpression;
        struct SRelationalExpressionLessEqualShiftExpression
        {
            RelationalExpression* relationalExpression;
            ShiftExpression* shiftExpression;
        } sRelationalExpressionLessEqualShiftExpression;
        struct SRelationalExpressionGreaterEqualShiftExpression
        {
            RelationalExpression* relationalExpression;
            ShiftExpression* shiftExpression;
        } sRelationalExpression;

        URelationalExpression():
            sShiftExpression{nullptr}{}
    } uRelationalExpression;

    RelationalExpression():
        Symbol(),
        eRelationalExpression(NONE),
        uRelationalExpression(){}
};

class ReplacementList : public Symbol
{
public:
    PpTokens* ppTokens;

    ReplacementList():
        Symbol(),
        ppTokens(nullptr){}
};

class SChar : public Symbol
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
        Symbol(),
        eSChar(NONE),
        uSChar(){}
};

class SCharSequence : public Symbol
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
        Symbol(),
        eSCharSequence(NONE),
        uSCharSequence(){}
};

class ShiftExpression : public Symbol
{
public:
    enum EShiftExpression
    {
        NONE,
        ADDITIVE_EXPRESSION,
        SHIFT_EXPRESSION_LSHIFT_ADDITIVE_EXPRESSION,
        SHIFT_EXPRESSION_RSHIFT_ADDITIVE_EXPRESSION
    } eShiftExpression;

    union UShiftExpression
    {
        struct SAdditiveExpression
        {
            AdditiveExpression* additiveExpression;
        } sAdditiveExpression;
        struct SShiftExpressionLshiftAdditiveExpression
        {
            ShiftExpression* shiftExpression;
            AdditiveExpression* additiveExpression;
        } sShiftExpressionLshiftAdditiveExpression;
        struct SShiftExpressionRshiftAdditiveExpression
        {
            ShiftExpression* shiftExpression;
            AdditiveExpression* additiveExpression;
        } sShiftExpressionRshiftAdditiveExpression;

        UShiftExpression():
            sAdditiveExpression{nullptr}{}
    } uShiftExpression;

    ShiftExpression():
        Symbol(),
        eShiftExpression(NONE),
        uShiftExpression(){}
};

class Sign : public Symbol
{
public:
    char element;

    Sign():
        Symbol(),
        element(0){}
};

class SimpleEscapeSequence : public Symbol
{
public:
    char element;

    SimpleEscapeSequence():
        Symbol(),
        element(0){}
};

class StringLiteral : public Symbol
{
public:
    EncodingPrefix* encodingPrefix;
    SCharSequence* sCharSequence;

    StringLiteral():
        Symbol(),
        encodingPrefix(nullptr),
        sCharSequence(nullptr){}
};

class TextLine : public Symbol
{
public:
    PpTokens* ppTokens;
    NewLine* newLine;

    TextLine():
        Symbol(),
        ppTokens(nullptr),
        newLine(nullptr){}
};

class UnaryExpression : public Symbol
{
public:
    enum EUnaryExpression
    {
        NONE,
        POSTFIX_EXPRESSION,
        PLUSPLUS_UNARY_EXPRESSION,
        MINUSMINUS_UNARY_EXPRESSION,
        UNARY_OPERATOR_CAST_EXPRESSION,
        SIZEOF_UNARY_EXPRESSION,
        SIZEOF_TYPE_NAME,
        ALIGNOF_TYPE_NAME
    } eUnaryExpression;

    union UUnaryExpression
    {
        struct SPostfixExpression
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpression;
        struct SPlusplusUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sPlusplusUnaryExpression;
        struct SMinusminusUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sMinuxminuxUnaryExpression;
        struct SUnaryOperatorCastExpression
        {
            UnaryOperator* unaryOperator;
            CastExpression* castExpression;
        } sUnaryOperatorCastExpression;
        struct SSizeofUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sSizeofUnaryExpression;
        struct SSizeofTypeName
        {
            TypeName* typeName;
        } sSizeofTypeName;
        struct SAlignofTypeName
        {
            TypeName* typeName;
        } sAlignofTypeName;

        UUnaryExpression():
            sPostfixExpression{nullptr}{}
    } uUnaryExpression;

    UnaryExpression():
        Symbol(),
        eUnaryExpression(NONE),
        uUnaryExpression(){}
};

class UnaryOperator : public Symbol
{
public:
    char element;

    UnaryOperator():
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

class UnsignedSuffix : public Symbol
{
public:
    char element;

    UnsignedSuffix():
        Symbol(),
        element(0){}
};