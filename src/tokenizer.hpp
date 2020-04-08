#pragma once

#include "token.hpp"
#include <vector>

class Tokenizer
{
public:
    Tokenizer();

    bool execute(const std::vector<PreprocessingToken*>&);

private:
    AlignmentSpecifier* getAlignmentSpecifier();
    Declaration* getDeclaration();
    DeclarationSpecifiers* getDeclarationSpecifiers();
    Declarator* getDeclarator(){}
    ExternalDeclaration* getExternalDeclaration(); 
    FunctionDefinition* getFunctionDefinition(){}
    FunctionSpecifier* getFunctionSpecifier(){}
    InitDeclarator* getInitDeclarator();
    InitDeclaratorList* getInitDeclaratorList();
    Initializer* getInitializer(){}
    StaticAssertDeclaration* getStaticAssertDeclaration(){}
    StorageClassSpecifier* getStorageClassSpecifier();
    TranslationUnit* getTranslationUnit();
    TypeName* getTypeName(){}
    TypeQualifier* getTypeQualifier(){}
    TypeSpecifier* getTypeSpecifier(){}

    bool isMatched(std::size_t idx, Token::EToken tag, std::string data) const;
    bool isMatched(std::size_t idx, Token::EToken tag) const;

    std::vector<PreprocessingToken*> mPPTokens;
    std::size_t mIdx;

    bool mIsValid;
};