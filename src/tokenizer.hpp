#pragma once

#include "token.hpp"
#include <vector>

class Tokenizer
{
public:
    Tokenizer();

    bool execute(std::vector<Token*>&);

private:
    AbstractDeclarator* getAbstractDeclarator();
    AdditiveExpression* getAdditiveExpression();
    AlignmentSpecifier* getAlignmentSpecifier();
    ANDExpression* getANDExpression();
    ArgumentExpressionList* getArgumentExpressionList();
    AssignmentExpression* getAssignmentExpression();
    AssignmentOperator* getAssignmentOperator();
    AtomicTypeSpecifier* getAtomicTypeSpecifier();
    BlockItem* getBlockItem();
    BlockItemList* getBlockItemList();
    CastExpression* getCastExpression();
    CompoundStatement* getCompoundStatement();
    ConditionalExpression* getConditionalExpression();
    Constant* getConstant();
    ConstantExpression* getConstantExpression();
    Declaration* getDeclaration();
    DeclarationList* getDeclarationList();
    Declarator* getDeclarator();
    DeclarationSpecifiers* getDeclarationSpecifiers();
    Designation* getDesignation();
    Designator* getDesignator();
    DesignatorList* getDesignatorList();
    DirectAbstractDeclarator* getDirectAbstractDeclarator(DirectAbstractDeclarator* bef = nullptr);
    DirectDeclarator* getDirectDeclarator(DirectDeclarator* bef = nullptr);
    EnumerationConstant* getEnumerationConstant();
    Enumerator* getEnumerator();
    EnumeratorList* getEnumeratorList();
    EnumSpecifier* getEnumSpecifier();
    EqualityExpression* getEqualityExpression();
    ExclusiveORExpression* getExclusiveORExpression();
    Expression* getExpression();
    ExpressionStatement* getExpressionStatement();
    ExternalDeclaration* getExternalDeclaration();
    FunctionDefinition* getFunctionDefinition();
    FunctionSpecifier* getFunctionSpecifier();
    GenericAssociation* getGenericAssociation();
    GenericAssocList* getGenericAssocList();
    GenericSelection* getGenericSelection();
    Identifier* getIdentifier();
    IdentifierList* getIdentifierList(IdentifierList* bef = nullptr);
    InclusiveORExpression* getInclusiveORExpression();
    InitDeclarator* getInitDeclarator();
    InitDeclaratorList* getInitDeclaratorList();
    Initializer* getInitializer();
    InitializerList* getInitializerList();
    IterationStatement* getIterationStatement();
    JumpStatement* getJumpStatement();
    LabeledStatement* getLabeledStatement();
    LogicalANDExpression* getLogicalANDExpression();
    LogicalORExpression* getLogicalORExpression();
    MultiplicativeExpression* getMultiplicativeExpression();
    ParameterDeclaration* getParameterDeclaration();
    ParameterList* getParameterList();
    ParameterTypeList* getParameterTypeList();
    Pointer* getPointer();
    PostfixExpression* getPostfixExpression(PostfixExpression* bef = nullptr);
    PrimaryExpression* getPrimaryExpression();
    RelationalExpression* getRelationalExpression();
    SelectionStatement* getSelectionStatement();
    ShiftExpression* getShiftExpression();
    SpecifierQualifierList* getSpecifierQualifierList();
    Statement* getStatement();
    StaticAssertDeclaration* getStaticAssertDeclaration();
    StorageClassSpecifier* getStorageClassSpecifier();
    StringLiteral* getStringLiteral();
    StructDeclaration* getStructDeclaration();
    StructDeclarationList* getStructDeclarationList();
    StructDeclarator* getStructDeclarator();    
    StructDeclaratorList* getStructDeclaratorList();
    StructOrUnion* getStructOrUnion();
    StructOrUnionSpecifier* getStructOrUnionSpecifier();
    TranslationUnit* getTranslationUnit();
    TypedefName* getTypedefName();
    TypeName* getTypeName();
    TypeQualifier* getTypeQualifier();
    TypeQualifierList* getTypeQualifierList();
    TypeSpecifier* getTypeSpecifier();
    UnaryExpression* getUnaryExpression();
    UnaryOperator* getUnaryOperator();

    bool isMatched(std::size_t idx, Token::EToken tag, std::string data) const;
    bool isMatched(std::size_t idx, Token::EToken tag) const;

    std::vector<Token*> mTokens;
    std::size_t mIdx;

    bool mIsValid;
};