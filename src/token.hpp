#pragma once

#include <vector>
#include <string>

class BaseToken;

class AbstractDeclarator;
class AdditiveExpression;
class AlignmentSpecifier;
class ANDExpression;
class ArgumentExpressionList;
class AssignmentExpression;
class AssignmentOperator;
class AtomicTypeSpecifier;
class BinaryExponentPart;
class BlockItem;
class BlockItemList;
class CastExpression;
class CChar;
class CCharSequence;
class CharacterConstant;
class CompoundStatement;
class ConditionalExpression;
class Constant;
class ConstantExpression;
class ControlLine;
class DecimalConstant;
class DecimalFloatingConstant;
class Declaration;
class DeclarationList;
class DeclarationSpecifiers;
class Declarator;
class Designation;
class Designator;
class DesignatorList;
class Digit;
class DigitSequence;
class DirectAbstractDeclarator;
class DirectDeclarator;
class ElifGroup;
class ElifGroups;
class ElseGroup;
class EndifLine;
class EncodingPrefix;
class EnumerationConstant;
class Enumerator;
class EnumeratorList;
class EnumSpecifier;
class EqualityExpression;
class EscapeSequence;
class ExclusiveORExpression;
class ExponentPart;
class Expression;
class ExpressionStatement;
class ExternalDeclaration;
class FloatingConstant;
class FloatingSuffix;
class FractionalConstant;
class FunctionDefinition;
class FunctionSpecifier;
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
class InitDeclarator;
class InitDeclaratorList;
class Initializer;
class InitializerList;
class IntegerConstant;
class IntegerSuffix;
class IterationStatement;
class JumpStatement;
class Keyword;
class LabeledStatement;
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
class ParameterDeclaration;
class ParameterList;
class ParameterTypeList;
class Pointer;
class PostfixExpression;
class PPNumber;
class PPTokens;
class PreprocessingFile;
class PreprocessingToken;
class PrimaryExpression;
class Punctuator;
class QChar;
class QCharSequence;
class RelationalExpression;
class ReplacementList;
class SChar;
class SCharSequence;
class SelectionStatement;
class ShiftExpression;
class Sign;
class SimpleEscapeSequence;
class SpecifierQualifierList;
class Statement;
class StaticAssertDeclaration;
class StorageClassSpecifier;
class StringLiteral;
class StructDeclaration;
class StructDeclarationList;
class StructDeclarator;
class StructDeclaratorList;
class StructOrUnion;
class StructOrUnionSpecifier;
class TextLine;
class Token;
class TranslationUnit;
class TypedefName;
class TypeName;
class TypeQualifier;
class TypeQualifierList;
class TypeSpecifier;
class UnaryExpression;
class UnaryOperator;
class UniversalCharacterName;
class UnsignedSuffix;

namespace TOKEN
{
    void error(const char* message);

    void getString(BinaryExponentPart*, std::string& data);
    void getString(CChar*, std::string& data);
    void getString(CCharSequence*, std::string& data);
    void getString(CharacterConstant*, std::string& data);
    void getString(Constant*, std::string& data);
    void getString(ControlLine*, std::string& data);
    void getString(DecimalConstant*, std::string& data);
    void getString(DecimalFloatingConstant*, std::string& data);
    void getString(Digit*, std::string& data);
    void getString(DigitSequence*, std::string& data);
    void getString(ElifGroup*, std::string& data);
    void getString(ElifGroups*, std::string& data);
    void getString(ElseGroup*, std::string& data);
    void getString(EndifLine*, std::string& data);
    void getString(EncodingPrefix*, std::string& data);
    void getString(EnumerationConstant*, std::string& data);
    void getString(EscapeSequence*, std::string& data);
    void getString(ExponentPart*, std::string& data);
    void getString(FloatingConstant*, std::string& data);
    void getString(FloatingSuffix*, std::string& data);
    void getString(FractionalConstant*, std::string& data);
    void getString(Group*, std::string& data);
    void getString(GroupPart*, std::string& data);
    void getString(HChar*, std::string& data);
    void getString(HCharSequence*, std::string& data);
    void getString(HeaderName*, std::string& data);
    void getString(HexadecimalConstant*, std::string& data);
    void getString(HexadecimalDigit*, std::string& data);
    void getString(HexadecimalDigitSequence*, std::string& data);
    void getString(HexadecimalEscapeSequence*, std::string& data);
    void getString(HexadecimalFloatingConstant*, std::string& data);
    void getString(HexadecimalFractionalConstant*, std::string& data);
    void getString(HexadecimalPrefix*, std::string& data);
    void getString(HexQuad*, std::string& data);
    void getString(Identifier*, std::string& data);
    void getString(IdentifierList*, std::string& data);
    void getString(IdentifierNondigit*, std::string& data);
    void getString(IfGroup*, std::string& data);
    void getString(IfSection*, std::string& data);
    void getString(IntegerConstant*, std::string& data);
    void getString(IntegerSuffix*, std::string& data);
    void getString(Keyword*, std::string& data);
    void getString(LongSuffix*, std::string& data);
    void getString(LongLongSuffix*, std::string& data);
    void getString(Lparen*, std::string& data);
    void getString(NewLine*, std::string& data);
    void getString(Nondigit*, std::string& data);
    void getString(NonDirective*, std::string& data);
    void getString(NonzeroDigit*, std::string& data);
    void getString(OctalConstant*, std::string& data);
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
    void getString(Token*, std::string& data);
    void getString(UniversalCharacterName*, std::string& data);
    void getString(UnsignedSuffix*, std::string& data);
};

class BaseToken
{
protected:
    BaseToken(){}
    virtual ~BaseToken(){}

public:
    static void destroy(){}
};

class AbstractDeclarator : public BaseToken
{
public:
    enum EAbstractDeclarator
    {
        NONE,
        POINTER,
        POINTER_DIRECT_ABSTRACT_DECLARATOR
    } eAbstractDeclarator;

    union UAbstractDeclarator
    {
        struct SPointer
        {
            Pointer* pointer;
        } sPointer;
        struct UPointerDirectAbstractDeclarator
        {
            Pointer* pointer;
            DirectAbstractDeclarator* directAbstractDeclarator;
        } sPointerDirectAbstractDeclarator;

        UAbstractDeclarator():
            sPointer{nullptr}{}
    } uAbstractDeclarator;

    AbstractDeclarator():
        BaseToken(),
        eAbstractDeclarator(NONE),
        uAbstractDeclarator(){}
};

class AdditiveExpression : public BaseToken
{
public:
    enum EAdditiveExpression
    {
        NONE,
        PLUS,
        MINUS
    };

    std::vector<std::pair<MultiplicativeExpression*, EAdditiveExpression>> multiplicativeExpressionPairVec;

    AdditiveExpression():
        BaseToken(),
        multiplicativeExpressionPairVec(){}
};

class AlignmentSpecifier : public BaseToken
{
public:
    enum EAlignmentSpecifier
    {
        NONE,
        TYPE_NAME,
        CONSTANT_EXPRESSION
    } eAlignmentSpecifier;

    union UAlignmentSpecifier
    {
        struct STypeName
        {
            TypeName* typeName;
        } sTypeName;
        struct SConstantExpression
        {
            ConstantExpression* constantExpression;
        } sConstantExpression;

        UAlignmentSpecifier():
            sTypeName{nullptr}{}
    } uAlignmentSpecifier;

    AlignmentSpecifier():
        BaseToken(),
        eAlignmentSpecifier(NONE),
        uAlignmentSpecifier(){}
};

class ANDExpression : public BaseToken
{
public:
    std::vector<EqualityExpression*> equalityExpressions;

    ANDExpression():
        BaseToken(),
        equalityExpressions(){}
};

class ArgumentExpressionList : public BaseToken
{
public:
    std::vector<AssignmentExpression*> assignmentExpressions;

    ArgumentExpressionList():
        BaseToken(),
        assignmentExpressions(){}
};

class AssignmentExpression : public BaseToken
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
        BaseToken(),
        eAssignmentExpression(NONE),
        uAssignmentExpression(){}
};

class AssignmentOperator : public BaseToken
{
public:
    Punctuator* punctuator;

    AssignmentOperator():
        BaseToken(),
        punctuator(nullptr){}
};

class AtomicTypeSpecifier : public BaseToken
{
public:
    TypeName* typeName;

    AtomicTypeSpecifier():
        BaseToken(),
        typeName(nullptr){}
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

class BlockItem : public BaseToken
{
public:
    enum EBlockItem
    {
        NONE,
        DECLARATION,
        STATEMENT
    } eBlockItem;

    union UBlockItem
    {
        struct SDeclaration
        {
            Declaration* declaration;
        } sDeclaration;
        struct SStatement
        {
            Statement* statement;
        } sStatement;

        UBlockItem():
            sDeclaration{nullptr}{}
    } uBlockItem;

    BlockItem():
        BaseToken(),
        eBlockItem(NONE),
        uBlockItem(){}
};

class BlockItemList : public BaseToken
{
public:
    std::vector<BlockItem*> blockItems;

    BlockItemList():
        BaseToken(),
        blockItems(){}
};

class CastExpression : public BaseToken
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
        BaseToken(),
        eCastExpression(NONE),
        uCastExpression(){}
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

class CompoundStatement : public BaseToken
{
public:
    BlockItemList* blockItemList;

    CompoundStatement():
        BaseToken(),
        blockItemList(nullptr){}
};

class ConditionalExpression : public BaseToken
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
        BaseToken(),
        eConditionalExpression(NONE),
        uConditionalExpression(){}
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

class ConstantExpression : public BaseToken
{
public:
    ConditionalExpression* conditionalExpression;

    ConstantExpression():
        BaseToken(),
        conditionalExpression(nullptr){}
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

class Declaration : public BaseToken
{
public:
    enum EDeclaration
    {
        NONE,
        DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST,
        STATIC_ASSERT_DECLARATION
    } eDeclaration;

    union UDeclaration
    {
        struct SDeclarationSpecifiersInitDeclaratorList
        {
            DeclarationSpecifiers* declarationSpecifiers;
            InitDeclaratorList* initDeclaratorList;
        } sDeclarationSpecifiersInitDeclaratorList;
        struct SStaticAssertDeclaration
        {
            StaticAssertDeclaration* staticAssertDeclaration;
        } sStaticAssertDeclaration;

        UDeclaration():
            sDeclarationSpecifiersInitDeclaratorList{nullptr, nullptr}{}
    } uDeclaration;

    Declaration():
        BaseToken(),
        eDeclaration(NONE),
        uDeclaration(){}
};

class DeclarationList : public BaseToken
{
public:
    std::vector<Declaration*> declarations;

    DeclarationList():
        BaseToken(),
        declarations(){}
};

class DeclarationSpecifiers : public BaseToken
{
public:
    enum EDeclarationSpecifiers
    {
        NONE,
        STORAGE_CLASS_SPECIFIER,
        TYPE_SPECIFIER,
        TYPE_QUALIFIER,
        FUNCTION_SPECIFIER,
        ALIGNMENT_SPECIFIER
    } eDeclarationSpecifiers;

    union UDeclarationSpecifiers
    {
        struct SStorageClassSpecifier
        {
            StorageClassSpecifier* storageClassSpecifier;
            DeclarationSpecifiers* declarationSpecifier;
        } sStorageClassSpecifier;
        struct STypeSpecifier
        {
            TypeSpecifier* typeSpecifier;
            DeclarationSpecifiers* declarationSpecifiers;
        } sTypeSpecifier;
        struct STypeQualifier
        {
            TypeQualifier* typeQualifier;
            DeclarationSpecifiers* declarationSpecifiers;
        } sTypeQualifier;
        struct SFunctionSpecifier
        {
            FunctionSpecifier* functionSpecifier;
            DeclarationSpecifiers* declarationSpecifiers;
        } sFunctionSpecifier;
        struct SAlignmentSpecifier
        {
            AlignmentSpecifier* alignmentSpecifier;
            DeclarationSpecifiers* declarationSpecifiers;
        } sAlignmentSpecifier;

        UDeclarationSpecifiers():
            sStorageClassSpecifier{nullptr, nullptr}{}
    } uDeclarationSpecifiers;

    DeclarationSpecifiers():
        BaseToken(),
        eDeclarationSpecifiers(NONE),
        uDeclarationSpecifiers(){}
};

class Declarator : public BaseToken
{
public:
    Pointer* pointer;
    DirectDeclarator* directDeclarator;

    Declarator():
        BaseToken(),
        pointer(nullptr),
        directDeclarator(nullptr){}
};

class Designation : public BaseToken
{
public:
    DesignatorList* designatorList;

    Designation():
        BaseToken(),
        designatorList(nullptr){}
};

class Designator : public BaseToken
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
            Identifier* identifier;
        } sIdentifier;

        UDesignator():
            sConstantExpression{nullptr}{}
    } uDesignator;

    Designator():
        BaseToken(),
        eDesignator(NONE),
        uDesignator(){}
};

class DesignatorList : public BaseToken
{
public:
    std::vector<Designator*> designators;

    DesignatorList():
        BaseToken(),
        designators(){}
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

class DirectAbstractDeclarator : public BaseToken
{
public:
    enum EDirectAbstractDeclarator
    {
        NONE,
        ABSTRACT_DECLARATOR,
        DIRECT_ABSTRACT_DECLARATOR_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION,
        DIRECT_ABSTRACT_DECLARATOR_STATIC_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION,
        DIRECT_ABSTRACT_DECLARATOR_TYPE_QUALIFIER_LIST_STATIC_ASSIGNMENT_EXPRESSION,
        DIRECT_ABSTRACT_DECLARATOR_ASTERISK,
        DIRECT_ABSTRACT_DECLARATOR_PARAMETER_TYPE_LIST
    } eDirectAbstractDeclarator;

    union UDirectAbstractDeclarator
    {
        struct SAbstractDeclarator
        {
            AbstractDeclarator* abstractDeclarator;
        } sAbstractDeclarator;
        struct SDirectAbstractDeclaratorTypeQualifierListAssignmentExpression
        {
            DirectAbstractDeclarator* directAbstractDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectAbstractDeclaratorTypeQualifierListAssignmentExpression;
        struct SDirectAbstractDeclaratorStaticTypeQualifierListAssignmentExpression
        {
            DirectAbstractDeclarator* directAbstractDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectAbstractDeclaratorStaticTypeQualifierListAssignmentExpression;
        struct SDirectAbstractDeclaratorTypeQualifierListStaticAssignmentExpression
        {
            DirectAbstractDeclarator* directAbstractDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectAbstractDeclaratorTypeQualifierListStaticAssignmentExpression;
        struct SDirectAbstractDeclaratorAsterisk
        {
            DirectAbstractDeclarator* directAbstractDeclarator;
        } sDirectAbstractDeclaratorAsterisk;
        struct SDirectAbstractDeclaratorParameterTypeList
        {
            DirectAbstractDeclarator* directAbstractDeclarator;
            ParameterTypeList* parameterTypeList;
        } sDirectAbstractDeclaratorParameterTypeList;

        UDirectAbstractDeclarator():
            sAbstractDeclarator{nullptr}{}
    } uDirectAbstractDeclarator;

    DirectAbstractDeclarator():
        BaseToken(),
        eDirectAbstractDeclarator(NONE),
        uDirectAbstractDeclarator(){}
};

class DirectDeclarator : public BaseToken
{
public:
    enum EDirectDeclarator
    {
        NONE,
        IDENTIFIER,
        DECLARATOR,
        DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION,
        DIRECT_DECLARATOR_STATIC_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION,
        DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_STATIC_ASSIGNMENT_EXPRESSION,
        DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST,
        DIRECT_DECLARATOR_PARAMETER_TYPE_LIST,
        DIRECT_DECLARATOR_IDENTIFIER_LIST
    } eDirectDeclarator;

    union UDirectDeclarator
    {
        struct SIdentifier
        {
            Identifier* identifier;
        } sIdentifier;
        struct SDeclarator
        {
            Declarator* declarator;
        } sDeclarator;
        struct SDirectDeclaratorTypeQualifierListAssignmentExpression
        {
            DirectDeclarator* directDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectDeclaratorTypeQualifierListAssignmentExpression;
        struct SDirectDeclaratorStaticTypeQualifierListAssignmentExpression
        {
            DirectDeclarator* directDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectDeclaratorStaticTypeQualifierListAssignmentExpression;
        struct SDirectDeclaratorTypeQualifierListStaticAssignmentExpression
        {
            DirectDeclarator* directDeclarator;
            TypeQualifierList* typeQualifierList;
            AssignmentExpression* assignmentExpression;
        } sDirectDeclaratorTypeQualifierListStaticAssignmentExpression;
        struct SDirectDeclaratorTypeQualifierList
        {
            DirectDeclarator* directDeclarator;
            TypeQualifierList* typeQualifierList;
        } sDirectDeclaratorTypeQualifierList;
        struct SDirectDeclaratorParameterTypeList
        {
            DirectDeclarator* directDeclarator;
            ParameterTypeList* parameterTypeList;
        } sDirectDeclaratorParameterTypeList;
        struct SDirectDeclaratorIdentifierList
        {
            DirectDeclarator* directDeclarator;
            IdentifierList* identifierList;
        } sDirectDeclaratorIdentifierList;
    
        UDirectDeclarator():
            sIdentifier{nullptr}{}
    } uDirectDeclarator;

    DirectDeclarator():
        BaseToken(),
        eDirectDeclarator(NONE),
        uDirectDeclarator(){}
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

class Enumerator : public BaseToken
{
public:
    enum EEnumerator
    {
        NONE,
        ENUMERATION_CONSTANT,
        ENUMERATION_CONSTANT_CONSTANT_EXPRESSION
    } eEnumerator;

    union UEnumerator
    {
        struct SEnumerationConstant
        {
            EnumerationConstant* enumerationConstant;
        } sEnumerationConstant;
        struct SEnumerationConstantConstantExression
        {
            EnumerationConstant* enumerationConstant;
            ConstantExpression* constantExression;
        } sEnumerationConstantConstantExpression;

        UEnumerator():
            sEnumerationConstant{nullptr}{}
    } uEnumerator;

    Enumerator():
        BaseToken(),
        eEnumerator(NONE),
        uEnumerator(){}
};

class EnumeratorList : public BaseToken
{
public:
    std::vector<Enumerator*> enumerators;

    EnumeratorList():
        BaseToken(),
        enumerators(){}
};

class EnumSpecifier : public BaseToken
{
public:
    enum EEnumSpecifier
    {
        NONE,
        IDENTIFIER_ENUMERATOR_LIST,
        IDENTIFIER_ENUMERATOR_LIST_COMMA,
        IDENTIFIER
    } eEnumSpecifier;

    union UEnumSpecifier
    {
        struct SIdentifierEnumeratorList
        {
            Identifier* identifier;
            EnumeratorList* enumeratorList;
        } sIdentifierEnumeratorList;
        struct SIdentifierEnumeratorListComma
        {
            Identifier* identifier;
            EnumeratorList* enumeratorList;
        } sIdentifierEnumeratorListComma;
        struct SIdentifier
        {
            Identifier* identifier;
        } sIdentifier;

        UEnumSpecifier():
            sIdentifierEnumeratorList{nullptr, nullptr}{}
    } uEnumSpecifier;

    EnumSpecifier():
        BaseToken(),
        eEnumSpecifier(NONE),
        uEnumSpecifier(){}
};

class EqualityExpression : public BaseToken
{
public:
    enum EEqualityExpression
    {
        NONE,
        EQUAL_EQUAL,
        NOT_EQUAL
    };

    std::vector<std::pair<RelationalExpression*, EEqualityExpression>> relationalExpressionPairVec;

    EqualityExpression():
        BaseToken(),
        relationalExpressionPairVec(){}
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

class ExclusiveORExpression : public BaseToken
{
public:
    std::vector<ANDExpression*> _ANDExpressions;

    ExclusiveORExpression():
        BaseToken(),
        _ANDExpressions(){}
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

class Expression : public BaseToken
{
public:
    std::vector<AssignmentExpression*> assignmentExpressions;

    Expression():
        BaseToken(),
        assignmentExpressions(){}
};

class ExpressionStatement : public BaseToken
{
public:
    Expression* expression;

    ExpressionStatement():
        BaseToken(),
        expression(nullptr){}
};

class ExternalDeclaration : public BaseToken
{
public:
    enum EExternalDeclaration
    {
        NONE,
        FUNCTION_DEFINITION,
        DECLARATION
    } eExternalDeclaration;

    union UExternalDeclaration
    {
        struct SFunctionDefinition
        {
            FunctionDefinition* functionDefinition;
        } sFunctionDefinition;
        struct SDeclaration
        {
            Declaration* declaration;
        } sDeclaration;

        UExternalDeclaration():
            sFunctionDefinition{nullptr}{}
    } uExternalDeclaration;

    ExternalDeclaration():
        BaseToken(),
        eExternalDeclaration(NONE),
        uExternalDeclaration(){}
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

class FunctionDefinition : public BaseToken
{
public:
    DeclarationSpecifiers* declarationSpecifiers;
    Declarator* declarator;
    DeclarationList* declarationList;
    CompoundStatement* compoundStatement;

    FunctionDefinition():
        BaseToken(),
        declarationSpecifiers(nullptr),
        declarator(nullptr),
        declarationList(nullptr),
        compoundStatement(nullptr){}
};

class FunctionSpecifier : public BaseToken
{
public:
    std::string element;

    FunctionSpecifier():
        BaseToken(),
        element(){}
};

class GenericAssociation : public BaseToken
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
        BaseToken(),
        eGenericAssociation(NONE),
        uGenericAssociation(){}
};

class GenericAssocList : public BaseToken
{
public:
    std::vector<GenericAssociation*> genericAssociations;

    GenericAssocList():
        BaseToken(),
        genericAssociations(){}
};

class GenericSelection : public BaseToken
{
public:
    AssignmentExpression* assignmentExpression;
    GenericAssocList* genericAssocList;

    GenericSelection():
        BaseToken(),
        assignmentExpression(nullptr),
        genericAssocList(nullptr){}
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

class HexadecimalConstant : public BaseToken
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
        } sHexadecimalPrefixHexadecimalDigit;
        struct SHexadecimalConstantHexadecimalDigit
        {
            HexadecimalConstant* hexadecimalConstant;
            HexadecimalDigit* hexadecimalDigit;
        } sHexadecimalConstantHexadecimalDigit;

        UHexadecimalConstant():
            sHexadecimalPrefixHexadecimalDigit{nullptr, nullptr}{}
    } uHexadecimalConstant;

    HexadecimalConstant():
        BaseToken(),
        eHexadecimalConstant(NONE),
        uHexadecimalConstant(){}
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

class InclusiveORExpression : public BaseToken
{
public:
    std::vector<ExclusiveORExpression*> exclusiveORExpressions;

    InclusiveORExpression():
        BaseToken(),
        exclusiveORExpressions(){}
};

class InitDeclarator : public BaseToken
{
public:
    enum EInitDeclarator
    {
        NONE,
        DECLARATOR,
        DECLARATOR_INITIALIZER
    } eInitDeclarator;

    union UInitDeclarator
    {
        struct SDeclarator
        {
            Declarator* declarator;
        } sDeclarator;
        struct SDeclaratorInitializer
        {
            Declarator* declarator;
            Initializer* initializer;
        } sDeclaratorInitializer;

        UInitDeclarator():
            sDeclarator{nullptr}{}
    } uInitDeclarator;

    InitDeclarator():
        BaseToken(),
        eInitDeclarator(NONE),
        uInitDeclarator(){}
};

class InitDeclaratorList : public BaseToken
{
public:
    std::vector<InitDeclarator*> initDeclarators;

    InitDeclaratorList():
        BaseToken(),
        initDeclarators(){}
};

class Initializer : public BaseToken
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
        BaseToken(),
        eInitializer(NONE),
        uInitializer(){}
};

class InitializerList : public BaseToken
{
public:
    struct SDesignationInitializer
    {
        Designation* designation;
        Initializer* initializer;

        SDesignationInitializer():
            designation(nullptr),
            initializer(nullptr){}
    };

    std::vector<SDesignationInitializer> sDesignationInitializers;

    InitializerList():
        BaseToken(),
        sDesignationInitializers(){}
};

class IntegerConstant : public BaseToken
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
        BaseToken(),
        eIntegerConstant(NONE),
        uIntegerConstant(){}
};

class IntegerSuffix : public BaseToken
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
            UnsignedSuffix* unsignedsuffix;
        } sLongSuffixUnsignedSuffix;
        struct SLongLongSuffixUnsignedSuffix
        {
            LongLongSuffix* longLongSuffix;
            UnsignedSuffix* unsignedSuffix;
        } sLongLongSuffixUnsignedSuffix;
    
        UIntegerSuffix():
            sUnsignedSuffixLongSuffix{nullptr, nullptr}{}
    } uIntegerSuffix;

    IntegerSuffix():
        BaseToken(),
        eIntegerSuffix(NONE),
        uIntegerSuffix(){}
};

class IterationStatement : public BaseToken
{
public:
    enum EIterationStatement
    {
        NONE,
        WHILE_EXPRESSION_STATEMENT,
        DO_STATEMENT_WHILE_EXPRESSION,
        FOR_EXPRESSION_EXPRESSION_EXPRESSION_STATEMENT,
        FOR_DECLARATION_EXPRESSION_EXPRESSION_STATEMENT
    } eIterationStatement;
    
    union UIterationStatement
    {
        struct SWhileExpressionStatement
        {
            Expression* expression;
            Statement* statement;
        } sWhileExpressionStatement;
        struct SDoStatementWhileExpression
        {
            Statement* statement;
            Expression* expression;
        } sDoStatementWhileExpression;
        struct SForExpressionExpressionExpressionStatement
        {
            Expression* expression;
            Expression* expression_1;
            Expression* expression_2;
            Statement* statement;
        } sForExpressionExpressionExpressionStatement;
        struct SForDeclarationExpressionExpressionStatement
        {
            Declaration* declaration;
            Expression* expression;
            Expression* expression_1;
            Statement* statement;
        } sForDeclarationExpressionExpressionStatement;

        UIterationStatement():
            sWhileExpressionStatement{nullptr, nullptr}{}
    } uIterationStatement;

    IterationStatement():
        BaseToken(),
        eIterationStatement(NONE),
        uIterationStatement(){}
};

class JumpStatement : public BaseToken
{
public:
    enum EJumpStatement
    {
        NONE,
        GOTO_IDENTIFIER,
        CONTINUE,
        BREAK,
        RETURN_EXPRESSION
    } eJumpStatement;

    union UJumpStatement
    {
        struct SGotoIdentifier
        {
            Identifier* identifier;
        } sGotoIdentifier;
        struct SReturnExpression
        {
            Expression* expression;
        } sReturnExpression;

        UJumpStatement():
            sGotoIdentifier{nullptr}{}
    } uJumpStatement;

    JumpStatement():
        BaseToken(),
        eJumpStatement(NONE),
        uJumpStatement(){}
};

class Keyword : public BaseToken
{
public:
    std::string element;

    Keyword():
        BaseToken(),
        element(){}
};

class LabeledStatement : public BaseToken
{
public:
    enum ELabeledStatement
    {
        NONE,
        IDENTIFIER_STATEMENT,
        CASE_CONSTANT_EXPRESSION_STATEMENT,
        DEFAULT_STATEMENT
    } eLabeledStatement;

    union ULabeledStatement
    {
        struct SIdentifierStatement
        {
            Identifier* identifier;
            Statement* statement;
        } sIdentifierStatement;
        struct SCaseConstantExpressionStatement
        {
            ConstantExpression* constantExpression;
            Statement* statement;
        } sCaseConstantExpressionStatement;
        struct SDefaultStatement
        {
            Statement* statement;
        } sDefaultStatement;

        ULabeledStatement():
            sIdentifierStatement{nullptr, nullptr}{}
    } uLabeledStatement;

    LabeledStatement():
        BaseToken(),
        eLabeledStatement(),
        uLabeledStatement(){}
};

class LogicalANDExpression : public BaseToken
{
public:
    std::vector<InclusiveORExpression*> inclusiveORExpressions;

    LogicalANDExpression():
        BaseToken(),
        inclusiveORExpressions(){}
};

class LogicalORExpression : public BaseToken
{
public:
    std::vector<LogicalANDExpression*> logicalANDExpressions;

    LogicalORExpression():
        BaseToken(),
        logicalANDExpressions(){}
};

class LongSuffix : public BaseToken
{
public:
    char element;

    LongSuffix():
        BaseToken(),
        element(0){}
};

class LongLongSuffix : public BaseToken
{
public:
    std::string element;

    LongLongSuffix():
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

class MultiplicativeExpression : public BaseToken
{
public:
    enum EMultiplicativeExpression
    {
        NONE,
        ASTERISK,
        SLASH,
        PERCENT
    };

    std::vector<std::pair<CastExpression*, EMultiplicativeExpression>> castExpressionPairVec;

    MultiplicativeExpression():
        BaseToken(),
        castExpressionPairVec(){}
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

class ParameterDeclaration : public BaseToken
{
public:
    enum EParameterDeclaration
    {
        NONE,
        DECLARATION_SPECIFIERS_DECLARATOR,
        DECLARATION_SPECIFIERS_ABSTRACT_DECLARATOR
    } eParameterDeclaration;

    union UParameterDeclaration
    {
        struct SDeclarationSpecifiersDeclarator
        {
            DeclarationSpecifiers* declarationSpecifiers;
            Declarator* declarator;
        } sDeclarationSpecifiersDeclarator;
        struct SDeclarationSpecifiersAbstractDeclarator
        {
            DeclarationSpecifiers* declarationSpecifiers;
            AbstractDeclarator* abstractDeclarator;
        } sDeclarationSpecifiersAbstractDeclarator;

        UParameterDeclaration():
            sDeclarationSpecifiersDeclarator{nullptr, nullptr}{}
    } uParameterDeclaration;

    ParameterDeclaration():
        BaseToken(),
        eParameterDeclaration(NONE),
        uParameterDeclaration(){}
};

class ParameterList : public BaseToken
{
public:
    std::vector<ParameterDeclaration*> parameterDeclarations;

    ParameterList():
        BaseToken(),
        parameterDeclarations(){}
};

class ParameterTypeList : public BaseToken
{
public:
    enum EParameterTypeList
    {
        NONE,
        PARAMETER_LIST,
        PARAMETER_LIST_DOTDOTDOT
    } eParameterTypeList;

    union UParameterTypeList
    {
        struct SParameterList
        {
            ParameterList* parameterList;
        } sParameterList;
        struct SParameterListDotdotdot
        {
            ParameterList* parameterList;
        } sParameterListDotdotdot;

        UParameterTypeList():
            sParameterList{nullptr}{}
    } uParameterTypeList;

    ParameterTypeList():
        BaseToken(),
        eParameterTypeList(NONE),
        uParameterTypeList(){}
};

class Pointer : public BaseToken
{
public:
    enum EPointer
    {
        NONE,
        TYPE_QUALIFIER_LIST,
        TYPE_QUALIFIER_LIST_POINTER
    } ePointer;

    union UPointer
    {
        struct STypeQualifierList
        {
            TypeQualifierList* typeQualifierList;
        } sTypeQualifierList;
        struct STypeQualifierListPointer
        {
            TypeQualifierList* typeQualifierList;
            Pointer* pointer;
        } sTypeQualifierPointer;

        UPointer():
            sTypeQualifierList{nullptr}{}
    } uPointer;

    Pointer():
        BaseToken(),
        ePointer(NONE),
        uPointer(){}
};

class PostfixExpression : public BaseToken
{
public:
    enum EPostfixExpression
    {
        NONE,
        PRIMARY_EXPRESSION,
        POSTFIX_EXPRESSION_EXPRESSION,
        POSTFIX_EXPRESSION_ARGUMENT_EXPRESSION_LIST,
        POSTFIX_EXPRESSION_DOT_IDENTIFIER,
        POSTFIX_EXPRESSION_ARROW_IDENTIFIER,
        POSTFIX_EXPRESSION_PLUSPLUS,
        POSTFIX_EXPRESSION_MINUSMINUS,
        TYPE_NAME_INITIALIZER_LIST,
        TYPE_NAME_INITIALIZER_LIST_DOT
    } ePostfixExpression;

    union UPostfixExpression
    {
        struct SPrimaryExpression
        {
            PrimaryExpression* primaryExpression;
        } sPrimaryExpression;
        struct SPostfixExpressionExpression
        {
            PostfixExpression* postfixExpression;
            Expression* expression;
        } sPostfixExpressionExpression;
        struct SPostfixExpressionArgumentExpressionList
        {
            PostfixExpression* postfixExpression;
            ArgumentExpressionList* argumentExpressionList;
        } sPostfixExpressionArgumentExpressionList;
        struct SPostfixExpressionDotIdentifier
        {
            PostfixExpression* postfixExpression;
            Identifier* identifier;
        } sPostfixExpressionDotIdentifier;
        struct SPostfixExpressionArrowIdentifier
        {
            PostfixExpression* postfixExpression;
            Identifier* identifier;
        } sPostfixExpressionArrowIdentifier;
        struct SPostfixExpressionPlusplus
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpressionPlusplus;
        struct SPostfixExpressionMinusminus
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpressionMinusminus;
        struct STypeNameIdentifierList
        {
            TypeName* typeName;
            IdentifierList* identifierList;
        } sTypeNameIdentifierList;
        struct STypeNameIdentifierListDot
        {
            TypeName* typeName;
            IdentifierList* identifierList;
        } sTypeNameIdentifierListDot;

        UPostfixExpression():
            sPrimaryExpression{nullptr}{}
    } uPostfixExpression;

    PostfixExpression():
        BaseToken(),
        ePostfixExpression(NONE),
        uPostfixExpression(){}
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

class PrimaryExpression : public BaseToken
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
        BaseToken(),
        ePrimaryExpression(NONE),
        uPrimaryExpression(){}
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

class RelationalExpression : public BaseToken
{
public:
    enum ERelationalExpression
    {
        NONE,
        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL
    };

    std::vector<std::pair<ShiftExpression*, ERelationalExpression>> shiftExpressionPairVec;

    RelationalExpression():
        BaseToken(),
        shiftExpressionPairVec(){}
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

class SelectionStatement : public BaseToken
{
public:
    enum ESelectionStatement
    {
        NONE,
        IF_EXPRESSION_STATEMENT,
        IF_EXPRESSION_STATEMENT_ELSE_STATEMENT,
        SWITCH_EXPRESSION_STATEMENT
    } eSelectionStatement;

    union USelectionStatement
    {
        struct SIfExpressionStatement
        {
            Expression* expression;
            Statement* statement;
        } sIfExpressionStatement;
        struct SIfExpressionStatementElseStatement
        {
            Expression* expression;
            Statement* statement;
            Statement* statement_1;
        } sIfExpressionStatementElseStatement;
        struct SSwitchExpressionStatement
        {
            Expression* expression;
            Statement* statement;
        } sSwitchExpressionStatement;

        USelectionStatement():
            sIfExpressionStatement{nullptr, nullptr}{}
    } uSelectionStatement;

    SelectionStatement():
        BaseToken(),
        eSelectionStatement(NONE),
        uSelectionStatement(){}
};

class ShiftExpression : public BaseToken
{
public:
    enum EShiftExpression
    {
        NONE,
        LEFT,
        RIGHT
    };

    std::vector<std::pair<AdditiveExpression*, EShiftExpression>> additiveExpressionPairVec;

    ShiftExpression():
        BaseToken(),
        additiveExpressionPairVec(){}
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

class SpecifierQualifierList : public BaseToken
{
public:
    enum ESpecifierQualifierList
    {
        NONE,
        TYPE_SPECIFIER_SPECIFIER_QUALIFIER_LIST,
        TYPE_QUALIFIER_SPECIFIER_QUALIFIER_LIST
    } eSpecifierQualifierList;

    union USpecifierQualifierList
    {
        struct STypeSpecifierSpecifierQualifierList
        {
            TypeSpecifier* typeSpecifier;
            SpecifierQualifierList* specifierQualifierList;
        } sTypeSpecifierSpecifierQualifierList;
        struct STypeQualifierSpecifierQualifierList
        {
            TypeQualifier* typeQualifier;
            SpecifierQualifierList* specifierQualifierList;
        } sTypeQualifierSpecifierQualifierList;

        USpecifierQualifierList():
            sTypeSpecifierSpecifierQualifierList{nullptr, nullptr}{}
    } uSpecifierQualifier;

    SpecifierQualifierList():
        BaseToken(),
        eSpecifierQualifierList(NONE),
        uSpecifierQualifier(){}
};

class Statement : public BaseToken
{
public:
    enum EStatement
    {
        NONE,
        LABELED_STATEMENT,
        COMPOUND_STATEMENT,
        EXPRESSION_STATEMENT,
        SELECTION_STATEMENT,
        ITERATION_STATEMENT,
        JUMP_STATEMENT
    } eStatement;

    union UStatement
    {
        struct SLabeledStatement
        {
            LabeledStatement* labeledStatement;
        } sLabeledStatement;
        struct SCompoundStatement
        {
            CompoundStatement* compoundStatement;
        } sCompoundStatement;
        struct SExpressionStatement
        {
            ExpressionStatement* expressionStatement;
        } sExpressionStatement;
        struct SSelectionStatement
        {
            SelectionStatement* selectionStatement;
        } sSelectionStatement;
        struct SIterationStatement
        {
            IterationStatement* iterationStatement;
        } sIterationStatement;
        struct SJumpStatement
        {
            JumpStatement* jumpStatement;
        } sJumpStatement;

        UStatement():
            sLabeledStatement{nullptr}{}
    } uStatement;

    Statement():
        BaseToken(),
        eStatement(NONE),
        uStatement(){}
};

class StaticAssertDeclaration : public BaseToken
{
public:
    ConstantExpression* constantExpression;
    StringLiteral* stringLiteral;

    StaticAssertDeclaration():
        BaseToken(),
        constantExpression(nullptr),
        stringLiteral(nullptr){}
};

class StorageClassSpecifier : public BaseToken
{
public:
    std::string element;

    StorageClassSpecifier():
        BaseToken(),
        element(){}
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

class StructDeclaration : public BaseToken
{
public:
    enum EStructDeclaration
    {
        NONE,
        SPECIFIER_QUALIFIER_LIST_STRUCT_DECLARATOR_LIST,
        STATIC_ASSERT_DECLARATION
    } eStructDeclaration;

    union UStructDeclaration
    {
        struct SSpecifierQualifierListStructDeclaratorList
        {
            SpecifierQualifierList* specifierQualifierList;
            StructDeclaratorList* structDeclaratorList;
        } sSpecifierQualifierListStructDeclaratorList;
        struct SStaticAssertDeclaration
        {
            StaticAssertDeclaration* staticAssertDeclaration;
        } sStaticAssertDeclaration;

        UStructDeclaration():
            sSpecifierQualifierListStructDeclaratorList{nullptr, nullptr}{}
    } uStructDeclaration;

    StructDeclaration():
        BaseToken(),
        eStructDeclaration(NONE),
        uStructDeclaration(){}
};

class StructDeclarationList : public BaseToken
{
public:
    std::vector<StructDeclaration*> structDeclarations;

    StructDeclarationList():
        BaseToken(),
        structDeclarations(){}
};

class StructDeclarator : public BaseToken
{
public:
    enum EStructDeclarator
    {
        NONE,
        DECLARATOR,
        DECLARATOR_CONSTANT_EXPRESSION
    } eStructDeclarator;

    union UStructDeclarator
    {
        struct SDeclarator
        {
            Declarator* declarator;
        } sDeclarator;
        struct SDeclaratorConstantExpression
        {
            Declarator* declarator;
            ConstantExpression* constantExpression;
        } sDeclaratorConstantExpression;

        UStructDeclarator():
            sDeclarator{nullptr}{}
    } uStructDeclarator;

    StructDeclarator():
        BaseToken(),
        eStructDeclarator(NONE),
        uStructDeclarator(){}
};

class StructDeclaratorList : public BaseToken
{
public:
    std::vector<StructDeclarator*> structDeclarators;

    StructDeclaratorList():
        BaseToken(),
        structDeclarators(){}
};

class StructOrUnion : public BaseToken
{
public:
    std::string element;

    StructOrUnion():
        BaseToken(),
        element(){}
};

class StructOrUnionSpecifier : public BaseToken
{
public:
    enum EStructOrUnionSpecifier
    {
        NONE,
        STRUCT_OR_UNION_IDENTIFIER_STRUCT_DECLARATION_LIST,
        STRUCT_OR_UNION_IDENTIFIER
    } eStructOrUnionSpecifier;

    union UStructOrUnionSpecifier
    {
        struct SStructOrUnionIdentifierStructDeclarationList
        {
            StructOrUnion* structOrUnion;
            Identifier* identifier;
            StructDeclarationList* structDeclarationList;
        } sStructOrUnionIdentifierStructDeclarationList;
        struct SStructOrUnionIdentifier
        {
            StructOrUnion* structOrUnion;
            Identifier* identifier;
        } sStructOrUnionIdentifier;

        UStructOrUnionSpecifier():
            sStructOrUnionIdentifierStructDeclarationList{nullptr, nullptr, nullptr}{}
    } uStructOrUnionSpecifier;

    StructOrUnionSpecifier():
        BaseToken(),
        eStructOrUnionSpecifier(NONE),
        uStructOrUnionSpecifier(){}
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

class TranslationUnit : public BaseToken
{
public:
    std::vector<ExternalDeclaration*> externalDeclarations;

    TranslationUnit():
        BaseToken(),
        externalDeclarations(){}
};

class TypedefName : public BaseToken
{
public:
    Identifier* identifier;

    TypedefName():
        BaseToken(),
        identifier(nullptr){}
};

class TypeName : public BaseToken
{
public:
    SpecifierQualifierList* specifierQualifierList;
    AbstractDeclarator* abstractDeclarator;

    TypeName():
        BaseToken(),
        specifierQualifierList(nullptr),
        abstractDeclarator(nullptr){}
};

class TypeQualifier : public BaseToken
{
public:
    std::string element;

    TypeQualifier():
        BaseToken(),
        element(){}
};

class TypeQualifierList : public BaseToken
{
public:
    std::vector<TypeQualifier*> typeQualifiers;

    TypeQualifierList():
        BaseToken(),
        typeQualifiers(){}
};

class TypeSpecifier : public BaseToken
{
public:
    enum ETypeSpecifier
    {
        NONE,
        BASE,
        ATOMIC_TYPE_SPECIFIER,
        STRUCT_OR_UNION_SPECIFIER,
        ENUM_SPECIFIER,
        TYPEDEF_NAME
    } eTypeSpecifier;

    union UTypeSpecifier
    {
        struct SBase
        {
            std::string* element;
        } sBase;
        struct SAtomicTypeSpecifier
        {
            AtomicTypeSpecifier* atomicTypeSpecifier;
        } sAtomicTypeSpecifier;
        struct SStructOrUnionSpecifier
        {
            StructOrUnionSpecifier* structOrUnionSpecifier;
        } sStructOrUnionSpecifier;
        struct SEnumSpecifier
        {
            EnumSpecifier* enumSpecifier;
        } sEnumSpecifier;
        struct STypedefName
        {
            TypedefName* typedefName;
        } sTypedefName;

        UTypeSpecifier():
            sAtomicTypeSpecifier{nullptr}{}
    } uTypeSpecifier;

    TypeSpecifier():
        BaseToken(),
        eTypeSpecifier(NONE),
        uTypeSpecifier(){}
};

class UnaryExpression : public BaseToken
{
public:
    enum EUnaryExpression
    {
        NONE,
        POSTFIX_EXPRESSION,
        PLUS_PLUS_UNARY_EXPRESSION,
        MINUS_MINUS_UNARY_EXPRESSION,
        UNARY_OPERATOR_CAST_EXPRESSION,
        SIZEOF_UNARY_EXPRESSION,
        SIZEOF_TYPE_NAME,
        ALIGNAF_TYPE_NAME
    } eUnaryExpression;

    union UUnaryExpression
    {
        struct SPostfixExpression
        {
            PostfixExpression* postfixExpression;
        } sPostfixExpression;
        struct SPlusPlusUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sPlusPlusUnaryExpression;
        struct SMinusMinusUnaryExpression
        {
            UnaryExpression* unaryExpression;
        } sMinusMinusUnaryExpression;
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
        BaseToken(),
        eUnaryExpression(NONE),
        uUnaryExpression(){}
};

class UnaryOperator : public BaseToken
{
public:
    Punctuator* punctuator;

    UnaryOperator():
        BaseToken(),
        punctuator(nullptr){}
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

class UnsignedSuffix : public BaseToken
{
public:
    char element;

    UnsignedSuffix():
        BaseToken(),
        element(0){}
};