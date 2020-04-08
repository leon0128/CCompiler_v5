#include "tokenizer.hpp"
#include <iostream>

std::vector<Token*> tmp;

Tokenizer::Tokenizer():
    mTokens(tmp),
    mIdx(0),
    mIsValid(true)
{
}

bool Tokenizer::execute(std::vector<Token*>& tokens)
{
    mTokens = tokens;

    getTranslationUnit();

    return mIsValid;
}

Declaration* Tokenizer::getDeclaration()
{
    Declaration declaration;

    auto befIdx = mIdx;
    bool isValid = true;

    if(declaration.uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers = getDeclarationSpecifiers())
    {
        declaration.uDeclaration.sDeclarationSpecifiersInitDeclaratorList.initDeclaratorList = getInitDeclaratorList();
        
        if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
        {
            mIdx++;
            declaration.eDeclaration = Declaration::DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST;
        }
        else
            isValid = false;
    }
    else if(declaration.uDeclaration.sStaticAssertDeclaration.staticAssertDeclaration = getStaticAssertDeclaration())
        declaration.eDeclaration = Declaration::STATIC_ASSERT_DECLARATION;
    else
        isValid = false;

    if(isValid)
        return new Declaration(declaration);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

DeclarationList* Tokenizer::getDeclarationList()
{
    std::vector<Declaration*> declarationVec;

    auto befIdx = mIdx;

    for(Declaration* d = getDeclaration(); d != nullptr; d = getDeclaration())
        declarationVec.push_back(d);
    
    if(!declarationVec.empty())
    {
        DeclarationList* declarationList = new DeclarationList();
        declarationList->declarations = std::move(declarationVec);
        return declarationList;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Declarator* Tokenizer::getDeclarator()
{
    
}

ExternalDeclaration* Tokenizer::getExternalDeclaration()
{
    ExternalDeclaration externalDeclaration;

    auto befIdx = mIdx;
    bool isValid = true;

    if(externalDeclaration.uExternalDeclaration.sFunctionDefinition.functionDefinition = getFunctionDefinition())
        externalDeclaration.eExternalDeclaration = ExternalDeclaration::FUNCTION_DEFINITION;
    else if(externalDeclaration.uExternalDeclaration.sDeclaration.declaration = getDeclaration())
        externalDeclaration.eExternalDeclaration = ExternalDeclaration::DECLARATION;
    else
        isValid = false;
    
    if(isValid)
        return new ExternalDeclaration(externalDeclaration);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

FunctionDefinition* Tokenizer::getFunctionDefinition()
{
    FunctionDefinition functionDefinition;

    auto befIdx = mIdx;
    bool isValid = true;

    if(functionDefinition.declarationSpecifiers = getDeclarationSpecifiers())
    {
        if(functionDefinition.declarator = getDeclarator())
        {
            functionDefinition.declarationList = getDeclarationList();
            
            if(functionDefinition.compoundStatement = getCompoundStatement())
                ;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new FunctionDefinition(functionDefinition);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

TranslationUnit* Tokenizer::getTranslationUnit()
{
    std::vector<ExternalDeclaration*> externalDeclarationVec;

    auto befIdx = mIdx;
    bool isValid = true;

    while(mIdx < mTokens.size() &&
          isValid)
    {
        ExternalDeclaration* ed = getExternalDeclaration();
        if(ed)
            externalDeclarationVec.push_back(ed);
        else
        {
            isValid = false;
            std::cerr << "Tokenizer error:\n"
                      << "    what: failed to get ExternalDeclaration."
                      << std::endl;
        }
    }

    if(isValid)
    {
        if(!externalDeclarationVec.empty())
        {
            TranslationUnit* translationUnit = new TranslationUnit();
            translationUnit->externalDeclarations = std::move(externalDeclarationVec);
            return translationUnit;
        }
        else
            return nullptr;
    }
    else
    {
        mIsValid = false;
        mIdx = befIdx;
        return nullptr;
    }
}

bool Tokenizer::isMatched(std::size_t idx, Token::EToken tag, std::string data) const
{
    if(isMatched(idx, tag))
    {
        std::string d;
        TOKEN::getString(mTokens[idx], d);
        
        return d == data;
    }
    else
        return false;
}

bool Tokenizer::isMatched(std::size_t idx, Token::EToken tag) const
{
    if(idx >= mTokens.size())
        return false;

    return mTokens[idx]->eToken == tag;
}