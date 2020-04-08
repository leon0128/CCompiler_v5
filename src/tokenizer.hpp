#pragma once

#include "token.hpp"
#include <vector>

class Tokenizer
{
public:
    Tokenizer();

    bool execute(std::vector<Token*>&);

private:
    CompoundStatement* getCompoundStatement(){return nullptr;}
    Declaration* getDeclaration();
    DeclarationList* getDeclarationList();
    Declarator* getDeclarator();
    DeclarationSpecifiers* getDeclarationSpecifiers(){return nullptr;}
    ExternalDeclaration* getExternalDeclaration();
    FunctionDefinition* getFunctionDefinition();
    InitDeclaratorList* getInitDeclaratorList(){return nullptr;}
    
    StaticAssertDeclaration* getStaticAssertDeclaration(){return nullptr;}
    TranslationUnit* getTranslationUnit();

    bool isMatched(std::size_t idx, Token::EToken tag, std::string data) const;
    bool isMatched(std::size_t idx, Token::EToken tag) const;

    std::vector<Token*> mTokens;
    std::size_t mIdx;

    bool mIsValid;
};