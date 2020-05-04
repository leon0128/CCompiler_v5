#include "tokenizer.hpp"
#include <iostream>

std::vector<Token*> tmp;

Tokenizer::Tokenizer():
    mTokens(tmp),
    mIdx(0),
    mTypedefNames(),
    mIsValid(true)
{
}

TranslationUnit* Tokenizer::execute(std::vector<Token*>& tokens)
{
    mTokens = tokens;

    return getTranslationUnit();
}

AbstractDeclarator* Tokenizer::getAbstractDeclarator()
{
    AbstractDeclarator abstractDeclarator;

    auto befIdx = mIdx;
    bool isValid = true;

    Pointer* pointer = getPointer();

    if(abstractDeclarator.uAbstractDeclarator.sPointerDirectAbstractDeclarator.directAbstractDeclarator = getDirectAbstractDeclarator())
    {
        abstractDeclarator.uAbstractDeclarator.sPointerDirectAbstractDeclarator.pointer = pointer;
        abstractDeclarator.eAbstractDeclarator = AbstractDeclarator::POINTER_DIRECT_ABSTRACT_DECLARATOR;
    }
    else if(pointer)
    {
        abstractDeclarator.uAbstractDeclarator.sPointer.pointer = pointer;
        abstractDeclarator.eAbstractDeclarator = AbstractDeclarator::POINTER;
    }
    else
        isValid = false;

    if(isValid)
        return new AbstractDeclarator(abstractDeclarator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

AdditiveExpression* Tokenizer::getAdditiveExpression()
{
    std::vector<std::pair<MultiplicativeExpression*, AdditiveExpression::EAdditiveExpression>> multiplicativeExpressionPairVec;

    MultiplicativeExpression* me = getMultiplicativeExpression();
    if(me)
    {
        multiplicativeExpressionPairVec.emplace_back(me, AdditiveExpression::NONE);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "+"))
            {
                mIdx++;

                if(me = getMultiplicativeExpression())
                    multiplicativeExpressionPairVec.emplace_back(me, AdditiveExpression::PLUS);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, "-"))
            {
                mIdx++;

                if(me = getMultiplicativeExpression())
                    multiplicativeExpressionPairVec.emplace_back(me, AdditiveExpression::MINUS);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!multiplicativeExpressionPairVec.empty())
    {
        AdditiveExpression* additiveExpression = new AdditiveExpression();
        additiveExpression->multiplicativeExpressionPairVec = std::move(multiplicativeExpressionPairVec);
        return additiveExpression;
    }
    else
        return nullptr;
}

AlignmentSpecifier* Tokenizer::getAlignmentSpecifier()
{
    AlignmentSpecifier alignmentSpecifier;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "_Alignas") &&
       isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(alignmentSpecifier.uAlignmentSpecifier.sTypeName.typeName = getTypeName())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                alignmentSpecifier.eAlignmentSpecifier = AlignmentSpecifier::TYPE_NAME;
            }
            else
                isValid = false;
        }
        else if(alignmentSpecifier.uAlignmentSpecifier.sConstantExpression.constantExpression = getConstantExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                alignmentSpecifier.eAlignmentSpecifier = AlignmentSpecifier::CONSTANT_EXPRESSION;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new AlignmentSpecifier(alignmentSpecifier);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

ANDExpression* Tokenizer::getANDExpression()
{
    std::vector<EqualityExpression*> equalityExpressionVec;

    EqualityExpression* ee = getEqualityExpression();
    if(ee)
    {
        equalityExpressionVec.push_back(ee);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "&"))
            {
                mIdx++;

                if(ee = getEqualityExpression())
                    equalityExpressionVec.push_back(ee);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!equalityExpressionVec.empty())
    {
        ANDExpression* _ANDExpression = new ANDExpression();
        _ANDExpression->equalityExpressions = std::move(equalityExpressionVec);
        return _ANDExpression;
    }
    else
        return nullptr;
}

ArgumentExpressionList* Tokenizer::getArgumentExpressionList()
{
    std::vector<AssignmentExpression*> assignmentExpressionVec;

    AssignmentExpression* ae = getAssignmentExpression();
    if(ae)
    {
        assignmentExpressionVec.push_back(ae);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(ae = getAssignmentExpression())
                    assignmentExpressionVec.push_back(ae);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!assignmentExpressionVec.empty())
    {
        ArgumentExpressionList* argumentExpressionList = new ArgumentExpressionList();
        argumentExpressionList->assignmentExpressions = std::move(assignmentExpressionVec);
        return argumentExpressionList;
    }
    else
        return nullptr;
}

AssignmentExpression* Tokenizer::getAssignmentExpression()
{
    AssignmentExpression assignmentExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(assignmentExpression.uAssignmentExpression.sUnaryExpressionAssignmentOperatorAssignmentExpression.unaryExpression = getUnaryExpression())
    {
        if(assignmentExpression.uAssignmentExpression.sUnaryExpressionAssignmentOperatorAssignmentExpression.assignmentOperator = getAssignmentOperator())
        {
            if(assignmentExpression.uAssignmentExpression.sUnaryExpressionAssignmentOperatorAssignmentExpression.assignmentExpression = getAssignmentExpression())
                assignmentExpression.eAssignmentExpression = AssignmentExpression::UNARY_EXPRESSION_ASSIGNMENT_OPERATOR_ASSIGNMENT_EXPRESSION;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(!isValid)
    {
        mIdx = befIdx;

        if(assignmentExpression.uAssignmentExpression.sConditionalExpression.conditionalExpression = getConditionalExpression())
        {
            isValid = true; 
            assignmentExpression.eAssignmentExpression = AssignmentExpression::CONDITIONAL_EXPRESSION;
        }
        else
            isValid = false;
    }

    if(isValid)
        return new AssignmentExpression(assignmentExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

AssignmentOperator* Tokenizer::getAssignmentOperator()
{
    if(!isMatched(mIdx, Token::PUNCTUATOR))
        return nullptr;

    std::string& punc = mTokens[mIdx]->uToken.sPunctuator.punctuator->element;
    if(punc == "=" ||
       punc == "*=" ||
       punc == "/=" ||
       punc == "%=" ||
       punc == "+=" ||
       punc == "-=" ||
       punc == "<<=" ||
       punc == ">>=" ||
       punc == "&=" ||
       punc == "^=" ||
       punc == "|=")
    {
        mIdx++;

        AssignmentOperator* assignmentOperator = new AssignmentOperator();
        assignmentOperator->punctuator = mTokens[mIdx - 1]->uToken.sPunctuator.punctuator;
        return assignmentOperator;
    }
    else
        return nullptr;
}

AtomicTypeSpecifier* Tokenizer::getAtomicTypeSpecifier()
{
    AtomicTypeSpecifier atomicTypeSpecifier;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "_Atomic") &&
       isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(atomicTypeSpecifier.typeName = getTypeName())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                mIdx++;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new AtomicTypeSpecifier(atomicTypeSpecifier);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

BlockItem* Tokenizer::getBlockItem()
{
    BlockItem blockItem;

    auto befIdx = mIdx;
    bool isValid = true;

    if(blockItem.uBlockItem.sDeclaration.declaration = getDeclaration())
        blockItem.eBlockItem = BlockItem::DECLARATION;
    else if(blockItem.uBlockItem.sStatement.statement = getStatement())
        blockItem.eBlockItem = BlockItem::STATEMENT;
    else
        isValid = false;

    if(isValid)
        return new BlockItem(blockItem);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

BlockItemList* Tokenizer::getBlockItemList()
{
    std::vector<BlockItem*> blockItemVec;

    for(BlockItem* bi = getBlockItem(); bi != nullptr; bi = getBlockItem())
        blockItemVec.push_back(bi);
    
    if(!blockItemVec.empty())
    {
        BlockItemList* blockItemList = new BlockItemList();
        blockItemList->blockItems = std::move(blockItemVec);
        return blockItemList;
    }
    else
        return nullptr;
}

CastExpression* Tokenizer::getCastExpression()
{
    CastExpression castExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "("))
    {
        mIdx++;

        if(castExpression.uCastExpression.sTypeNameCastExpression.typeName = getTypeName())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                if(castExpression.uCastExpression.sTypeNameCastExpression.castExpression = getCastExpression())
                    castExpression.eCastExpression = CastExpression::TYPE_NAME_CAST_EXPRESSION;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(!isValid)
    {
        mIdx = befIdx;

        if(castExpression.uCastExpression.sUnaryExpression.unaryExpression = getUnaryExpression())
        {
            isValid = true;
            castExpression.eCastExpression = CastExpression::UNARY_EXPRESSION;
        }
    }

    if(isValid)
        return new CastExpression(castExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

CompoundStatement* Tokenizer::getCompoundStatement()
{
    CompoundStatement compoundStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "{"))
    {
        mIdx++;

        mTypedefNames.emplace_back();

        compoundStatement.blockItemList = getBlockItemList();

        if(isMatched(mIdx, Token::PUNCTUATOR, "}"))
            mIdx++;
        else
            isValid = false;
        
        mTypedefNames.pop_back();
    }
    else
        isValid = false;

    if(isValid)
        return new CompoundStatement(compoundStatement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

ConditionalExpression* Tokenizer::getConditionalExpression()
{
    ConditionalExpression conditionalExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    LogicalORExpression* logicalORExpression = getLogicalORExpression();
    if(logicalORExpression)
    {
        auto midIdx = mIdx;

        if(isMatched(mIdx, Token::PUNCTUATOR, "?"))
        {
            mIdx++;

            if(conditionalExpression.uConditionalExpression.sLogicalORExpressionExpressionConditionalExpression.expression = getExpression())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, ":"))
                {
                    mIdx++;

                    if(conditionalExpression.uConditionalExpression.sLogicalORExpressionExpressionConditionalExpression.conditionalExpression = getConditionalExpression())
                    {
                        conditionalExpression.uConditionalExpression.sLogicalORExpressionExpressionConditionalExpression.logicalORExpression = logicalORExpression;
                        conditionalExpression.eConditionalExpression = ConditionalExpression::LOGICAL_OR_EXPRESSION_EXPRESSION_CONDITIONAL_EXPRESSION;
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;

        if(!isValid)
        {
            isValid = true;
            mIdx = midIdx;

            conditionalExpression.uConditionalExpression.sLogicalORExpression.logicalORExpression = logicalORExpression;
            conditionalExpression.eConditionalExpression = ConditionalExpression::LOGICAL_OR_EXPRESSION;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new ConditionalExpression(conditionalExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Constant* Tokenizer::getConstant()
{
    if(isMatched(mIdx, Token::CONSTANT))
    {
        mIdx++;

        return mTokens[mIdx - 1]->uToken.sConstant.constant;
    }
    else
        return nullptr;
}

ConstantExpression* Tokenizer::getConstantExpression()
{
    ConstantExpression constantExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(constantExpression.conditionalExpression = getConditionalExpression())
        ;
    else
        isValid = false;

    if(isValid)
        return new ConstantExpression(constantExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Declaration* Tokenizer::getDeclaration()
{
    Declaration declaration;

    auto befIdx = mIdx;
    bool isValid = true;

    if(declaration.uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers = getDeclarationSpecifiers())
    {
        declaration.uDeclaration.sDeclarationSpecifiersInitDeclaratorList.initDeclaratorList = getInitDeclaratorList(isTypedefDeclaration(declaration.uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers));
        
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
    Declarator declarator;

    auto befIdx = mIdx;
    bool isValid = true;

    declarator.pointer = getPointer();
    
    if(declarator.directDeclarator = getDirectDeclarator())
        ;
    else
        isValid = false;
    
    if(isValid)
        return new Declarator(declarator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

DeclarationSpecifiers* Tokenizer::getDeclarationSpecifiers()
{
    DeclarationSpecifiers declarationSpecifiers;

    auto befIdx = mIdx;
    bool isValid = true;

    if(declarationSpecifiers.uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier = getStorageClassSpecifier())
    {
        declarationSpecifiers.uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier = getDeclarationSpecifiers();
        declarationSpecifiers.eDeclarationSpecifiers = DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER;
    }
    else if(declarationSpecifiers.uDeclarationSpecifiers.sTypeSpecifier.typeSpecifier = getTypeSpecifier())
    {
        declarationSpecifiers.uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers = getDeclarationSpecifiers();
        declarationSpecifiers.eDeclarationSpecifiers = DeclarationSpecifiers::TYPE_SPECIFIER;
    }
    else if(declarationSpecifiers.uDeclarationSpecifiers.sTypeQualifier.typeQualifier = getTypeQualifier())
    {
        declarationSpecifiers.uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers = getDeclarationSpecifiers();
        declarationSpecifiers.eDeclarationSpecifiers = DeclarationSpecifiers::TYPE_QUALIFIER;
    }
    else if(declarationSpecifiers.uDeclarationSpecifiers.sFunctionSpecifier.functionSpecifier = getFunctionSpecifier())
    {
        declarationSpecifiers.uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers = getDeclarationSpecifiers();
        declarationSpecifiers.eDeclarationSpecifiers = DeclarationSpecifiers::FUNCTION_SPECIFIER;
    }
    else if(declarationSpecifiers.uDeclarationSpecifiers.sAlignmentSpecifier.alignmentSpecifier = getAlignmentSpecifier())
    {
        declarationSpecifiers.uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers = getDeclarationSpecifiers();
        declarationSpecifiers.eDeclarationSpecifiers = DeclarationSpecifiers::ALIGNMENT_SPECIFIER;
    }
    else
        isValid = false;

    if(isValid)
        return new DeclarationSpecifiers(declarationSpecifiers);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Designation* Tokenizer::getDesignation()
{
    Designation designation;

    auto befIdx = mIdx;
    bool isValid = true;

    if(designation.designatorList = getDesignatorList())
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "="))
            mIdx++;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new Designation(designation);
    else
    {
        mIdx = befIdx;
        return nullptr;
    } 
}

Designator* Tokenizer::getDesignator()
{
    Designator designator;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "["))
    {
        mIdx++;

        if(designator.uDesignator.sConstantExpression.constantExpression = getConstantExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
            {
                mIdx++;

                designator.eDesignator = Designator::CONSTANT_EXPRESSION;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::PUNCTUATOR, ".") &&
            isMatched(mIdx + 1, Token::IDENTIFIER))
    {
        mIdx += 2;

        designator.uDesignator.sIdentifier.identifier = mTokens[mIdx - 1]->uToken.sIdentifier.identifier;
        designator.eDesignator = Designator::IDENTIFIER;
    }
    else
        isValid = false;

    if(isValid)
        return new Designator(designator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

DesignatorList* Tokenizer::getDesignatorList()
{
    std::vector<Designator*> designatorVec;

    for(Designator* d = getDesignator(); d != nullptr; d = getDesignator())
        designatorVec.push_back(d);

    if(!designatorVec.empty())
    {
        DesignatorList* designatorList = new DesignatorList();
        designatorList->designators = std::move(designatorVec);
        return designatorList;
    }
    else
        return nullptr;
}

DirectAbstractDeclarator* Tokenizer::getDirectAbstractDeclarator(DirectAbstractDeclarator* bef)
{
    DirectAbstractDeclarator directAbstractDeclarator;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "("))
    {
        mIdx++;

        if(!bef &&
           (directAbstractDeclarator.uDirectAbstractDeclarator.sAbstractDeclarator.abstractDeclarator = getAbstractDeclarator()))
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::ABSTRACT_DECLARATOR;
            }
            else
                isValid = false;
        }
        else
        {
            directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorParameterTypeList.parameterTypeList = getParameterTypeList();
            
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorParameterTypeList.directAbstractDeclarator = bef;
                directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::DIRECT_ABSTRACT_DECLARATOR_PARAMETER_TYPE_LIST;
            }
            else
                isValid = false;
        }
    }
    else if(isMatched(mIdx, Token::PUNCTUATOR, "["))
    {
        mIdx++;
        auto midIdx = mIdx;

        directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorTypeQualifierListAssignmentExpression.typeQualifierList = getTypeQualifierList();
        directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorTypeQualifierListAssignmentExpression.assignmentExpression = getAssignmentExpression();
        if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
        {
            mIdx++;
            
            directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::DIRECT_ABSTRACT_DECLARATOR_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION;
        }
        else
        {
            mIdx = midIdx;
            isValid = false;
        }

        if(!isValid &&
           isMatched(mIdx, Token::KEYWORD, "static"))
        {
            mIdx++;

            directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorStaticTypeQualifierListAssignmentExpression.typeQualifierList = getTypeQualifierList();

            if(directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorStaticTypeQualifierListAssignmentExpression.assignmentExpression = getAssignmentExpression())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
                {
                    mIdx++;
                    isValid = true;

                    directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorStaticTypeQualifierListAssignmentExpression.directAbstractDeclarator = bef;
                    directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::DIRECT_ABSTRACT_DECLARATOR_STATIC_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION;
                }
                else
                    mIdx = midIdx;
            }
            else
                mIdx = midIdx;
        }
        
        if(!isValid &&
           (directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorTypeQualifierListStaticAssignmentExpression.typeQualifierList = getTypeQualifierList()))
        {
            if(isMatched(mIdx, Token::KEYWORD, "static"))
            {
                mIdx++;

                if(directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorTypeQualifierListStaticAssignmentExpression.assignmentExpression = getAssignmentExpression())
                {
                    if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
                    {
                        mIdx++;
                        isValid = true;
                        
                        directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorTypeQualifierListStaticAssignmentExpression.directAbstractDeclarator = bef;
                        directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::DIRECT_ABSTRACT_DECLARATOR_TYPE_QUALIFIER_LIST_STATIC_ASSIGNMENT_EXPRESSION;
                    }
                    else
                        mIdx = midIdx;
                }
                else
                    mIdx = midIdx;
            }
            else
                mIdx = midIdx;
        }

        if(!isValid &&
           isMatched(mIdx, Token::PUNCTUATOR, "*") &&
           isMatched(mIdx + 1, Token::PUNCTUATOR, "]"))
        {
            mIdx += 2;
            isValid = true;

            directAbstractDeclarator.uDirectAbstractDeclarator.sDirectAbstractDeclaratorAsterisk.directAbstractDeclarator = bef;
            directAbstractDeclarator.eDirectAbstractDeclarator = DirectAbstractDeclarator::DIRECT_ABSTRACT_DECLARATOR_ASTERISK;
        }
    }
    else
        isValid = false;

    if(isValid)
    {
        DirectAbstractDeclarator* retVal = new DirectAbstractDeclarator(directAbstractDeclarator);
        DirectAbstractDeclarator* aft = getDirectAbstractDeclarator(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

DirectDeclarator* Tokenizer::getDirectDeclarator(DirectDeclarator* bef)
{
    DirectDeclarator directDeclarator;

    auto befIdx = mIdx;
    bool isValid = true;

    if(!bef)
    {
        if(directDeclarator.uDirectDeclarator.sIdentifier.identifier = getIdentifier())
            directDeclarator.eDirectDeclarator = DirectDeclarator::IDENTIFIER;
        else if(isMatched(mIdx, Token::PUNCTUATOR, "("))
        {
            mIdx++;

            if(directDeclarator.uDirectDeclarator.sDeclarator.declarator = getDeclarator())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                {
                    mIdx++;

                    directDeclarator.eDirectDeclarator = DirectDeclarator::DECLARATOR;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "["))
        {
            mIdx++;
            auto midIdx = mIdx;

            directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListAssignmentExpression.typeQualifierList = getTypeQualifierList();
            directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListAssignmentExpression.assignmentExpression = getAssignmentExpression();
            
            if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
            {
                mIdx++;

                directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListAssignmentExpression.directDeclarator = bef;
                directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION;
            }
            else
            {
                isValid = false;
                mIdx = midIdx;
            }

            if(!isValid &&
               isMatched(mIdx, Token::KEYWORD, "static"))
            {
                mIdx++;

                directDeclarator.uDirectDeclarator.sDirectDeclaratorStaticTypeQualifierListAssignmentExpression.typeQualifierList = getTypeQualifierList();

                if(directDeclarator.uDirectDeclarator.sDirectDeclaratorStaticTypeQualifierListAssignmentExpression.assignmentExpression = getAssignmentExpression())
                {
                    if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
                    {
                        mIdx++;
                        isValid = true;

                        directDeclarator.uDirectDeclarator.sDirectDeclaratorStaticTypeQualifierListAssignmentExpression.directDeclarator = bef;
                        directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_STATIC_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION;
                    }
                    else
                        mIdx = midIdx;
                }
                else
                    mIdx = midIdx;
            }
            
            if(!isValid)
            {
                if(directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListStaticAssignmentExpression.typeQualifierList = getTypeQualifierList())
                {
                    if(isMatched(mIdx, Token::KEYWORD, "static"))
                    {
                        mIdx++;

                        if(directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListStaticAssignmentExpression.assignmentExpression = getAssignmentExpression())
                        {
                            if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
                            {
                                mIdx++;
                                isValid = true;

                                directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierListStaticAssignmentExpression.directDeclarator = bef;
                                directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_STATIC_ASSIGNMENT_EXPRESSION;
                            }
                            else
                                mIdx = midIdx;
                        }
                        else
                            mIdx = midIdx;
                    }
                    else
                        mIdx = midIdx;
                }
            }

            if(!isValid)
            {
                directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierList.typeQualifierList = getTypeQualifierList();
                if(isMatched(mIdx, Token::PUNCTUATOR, "*") &&
                   isMatched(mIdx + 1, Token::PUNCTUATOR, "]"))
                {
                    mIdx += 2;
                    isValid = true;

                    directDeclarator.uDirectDeclarator.sDirectDeclaratorTypeQualifierList.directDeclarator = bef;
                    directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST;
                }
                else
                    mIdx = midIdx;
            }
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "("))
        {
            mIdx++;
            auto midIdx = mIdx;

            if(directDeclarator.uDirectDeclarator.sDirectDeclaratorParameterTypeList.parameterTypeList = getParameterTypeList())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                {
                    mIdx++;

                    directDeclarator.uDirectDeclarator.sDirectDeclaratorParameterTypeList.directDeclarator = bef;
                    directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_PARAMETER_TYPE_LIST;
                }
                else
                {
                    isValid = false;
                    mIdx = midIdx;
                }
            }
            else
            {
                isValid = false;
                mIdx = midIdx;
            }

            if(!isValid)
            {
                directDeclarator.uDirectDeclarator.sDirectDeclaratorIdentifierList.identifierList = getIdentifierList();

                if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                {
                    mIdx++;
                    isValid = true;

                    directDeclarator.uDirectDeclarator.sDirectDeclaratorIdentifierList.directDeclarator = bef;
                    directDeclarator.eDirectDeclarator = DirectDeclarator::DIRECT_DECLARATOR_IDENTIFIER_LIST;
                }
            }
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        DirectDeclarator* retVal = new DirectDeclarator(directDeclarator);
        DirectDeclarator* aft = getDirectDeclarator(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

EnumerationConstant* Tokenizer::getEnumerationConstant()
{
    if(isMatched(mIdx, Token::IDENTIFIER))
    {
        mIdx++;

        EnumerationConstant* enumerationConstant = new EnumerationConstant();
        enumerationConstant->identifier = mTokens[mIdx - 1]->uToken.sIdentifier.identifier;
        return enumerationConstant;
    }
    else
        return nullptr;
}

Enumerator* Tokenizer::getEnumerator()
{
    Enumerator enumerator;

    auto befIdx = mIdx;
    bool isValid = true;

    EnumerationConstant* enumerationConstant = getEnumerationConstant();
    if(enumerationConstant)
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "="))
        {
            mIdx++;

            if(enumerator.uEnumerator.sEnumerationConstantConstantExpression.constantExression = getConstantExpression())
            {
                enumerator.uEnumerator.sEnumerationConstantConstantExpression.enumerationConstant = enumerationConstant;
                enumerator.eEnumerator = Enumerator::ENUMERATION_CONSTANT_CONSTANT_EXPRESSION;
            }
            else
            {
                mIdx--;
                isValid = false;
            }
        }

        if(!isValid)
        {
            isValid = true;

            enumerator.uEnumerator.sEnumerationConstant.enumerationConstant = enumerationConstant;
            enumerator.eEnumerator = Enumerator::ENUMERATION_CONSTANT;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new Enumerator(enumerator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

EnumeratorList* Tokenizer::getEnumeratorList()
{
    std::vector<Enumerator*> enumeratorVec;

    Enumerator* e = getEnumerator();
    if(e)
    {
        enumeratorVec.push_back(e);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(e = getEnumerator())
                    enumeratorVec.push_back(e);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!enumeratorVec.empty())
    {
        EnumeratorList* enumeratorList = new EnumeratorList();
        enumeratorList->enumerators = std::move(enumeratorVec);
        return enumeratorList;
    }
    else
        return nullptr;
}

EnumSpecifier* Tokenizer::getEnumSpecifier()
{
    EnumSpecifier enumSpecifier;
    
    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "enum"))
    {
        Identifier* identifeir = getIdentifier();
        auto midIdx = mIdx;

        if(isMatched(mIdx, Token::PUNCTUATOR, "{"))
        {
            mIdx++;

            EnumeratorList* enumeratorList = getEnumeratorList();
            if(enumeratorList)
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, "}"))
                {
                    mIdx++;

                    enumSpecifier.uEnumSpecifier.sIdentifierEnumeratorList.identifier = identifeir;
                    enumSpecifier.uEnumSpecifier.sIdentifierEnumeratorList.enumeratorList = enumeratorList;
                    enumSpecifier.eEnumSpecifier = EnumSpecifier::IDENTIFIER_ENUMERATOR_LIST;
                }
                else if(isMatched(mIdx, Token::PUNCTUATOR, ","))
                {
                    mIdx++;

                    if(isMatched(mIdx, Token::PUNCTUATOR, "}"))
                    {
                        mIdx++;

                        enumSpecifier.uEnumSpecifier.sIdentifierEnumeratorListComma.identifier = identifeir;
                        enumSpecifier.uEnumSpecifier.sIdentifierEnumeratorListComma.enumeratorList = enumeratorList;
                        enumSpecifier.eEnumSpecifier = EnumSpecifier::IDENTIFIER_ENUMERATOR_LIST_COMMA;
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;

        if(!isValid &&
           identifeir)
        {
            isValid = true;
            mIdx = midIdx;

            enumSpecifier.uEnumSpecifier.sIdentifier.identifier = identifeir;
            enumSpecifier.eEnumSpecifier = EnumSpecifier::IDENTIFIER;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new EnumSpecifier(enumSpecifier);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

EqualityExpression* Tokenizer::getEqualityExpression()
{
    std::vector<std::pair<RelationalExpression*, EqualityExpression::EEqualityExpression>> relationalExpressionPairVec;

    RelationalExpression* re = getRelationalExpression();
    if(re)
    {
        relationalExpressionPairVec.emplace_back(re, EqualityExpression::NONE);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "=="))
            {
                mIdx++;

                if(re = getRelationalExpression())
                    relationalExpressionPairVec.emplace_back(re, EqualityExpression::EQUAL_EQUAL);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, "!="))
            {
                mIdx++;

                if(re = getRelationalExpression())
                    relationalExpressionPairVec.emplace_back(re, EqualityExpression::NOT_EQUAL);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!relationalExpressionPairVec.empty())
    {
        EqualityExpression* equalityExpression = new EqualityExpression();
        equalityExpression->relationalExpressionPairVec = std::move(relationalExpressionPairVec);
        return equalityExpression;
    }
    else
        return nullptr;
}

ExclusiveORExpression* Tokenizer::getExclusiveORExpression()
{
    std::vector<ANDExpression*> _ANDExpressionVec;

    ANDExpression* _ANDe = getANDExpression();
    if(_ANDe)
    {
        _ANDExpressionVec.push_back(_ANDe);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "^"))
            {
                mIdx++;

                if(_ANDe = getANDExpression())
                    _ANDExpressionVec.push_back(_ANDe);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!_ANDExpressionVec.empty())
    {
        ExclusiveORExpression* exclusiveORExpression = new ExclusiveORExpression();
        exclusiveORExpression->_ANDExpressions = std::move(_ANDExpressionVec);
        return exclusiveORExpression;
    }
    else
        return nullptr;

    return nullptr;
}

Expression* Tokenizer::getExpression()
{
    std::vector<AssignmentExpression*> assignmentExpressionVec;

    AssignmentExpression* ae = getAssignmentExpression();
    if(ae)
    {
        assignmentExpressionVec.push_back(ae);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(ae = getAssignmentExpression())
                    assignmentExpressionVec.push_back(ae);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!assignmentExpressionVec.empty())
    {
        Expression* expression = new Expression();
        expression->assignmentExpressions = std::move(assignmentExpressionVec);
        return expression;
    }
    else
        return nullptr;
}

ExpressionStatement* Tokenizer::getExpressionStatement()
{
    ExpressionStatement expressionStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    expressionStatement.expression = getExpression();

    if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
    {
        mIdx++;
        return new ExpressionStatement(expressionStatement);
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
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

FunctionSpecifier* Tokenizer::getFunctionSpecifier()
{
    if(mIdx >= mTokens.size())
        return nullptr;

    std::string data;
    TOKEN::getString(mTokens[mIdx], data);

    if(data == "inline" ||
       data == "_Norreturn")
    {
        mIdx++;

        FunctionSpecifier* functionSpecifier = new FunctionSpecifier();
        functionSpecifier->element = std::move(data);
        return functionSpecifier;
    }
    else
        return nullptr;
}

GenericAssociation* Tokenizer::getGenericAssociation()
{
    GenericAssociation genericAssociation;

    auto befIdx = mIdx;
    bool isValid = true;

    if(genericAssociation.uGenericAssociation.sTypeNameAssignmentExpression.typeName = getTypeName())
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, ":"))
        {
            mIdx++;

            if(genericAssociation.uGenericAssociation.sTypeNameAssignmentExpression.assignmentExpression = getAssignmentExpression())
                genericAssociation.eGenericAssociation = GenericAssociation::TYPE_NAME_ASSIGNMENT_EXPRESSION;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "default") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, ":"))
    {
        mIdx += 2;

        if(genericAssociation.uGenericAssociation.sDefaultAssignmentExpression.assignmentExpression = getAssignmentExpression())
            genericAssociation.eGenericAssociation = GenericAssociation::DEFAULT_ASSIGNMENT_EXPRESSION;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new GenericAssociation(genericAssociation);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

GenericAssocList* Tokenizer::getGenericAssocList()
{
    std::vector<GenericAssociation*> genericAssociationVec;

    GenericAssociation* ga = getGenericAssociation();
    if(ga)
    {
        genericAssociationVec.push_back(ga);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(ga = getGenericAssociation())
                    genericAssociationVec.push_back(ga);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!genericAssociationVec.empty())
    {
        GenericAssocList* genericAssocList = new GenericAssocList();
        genericAssocList->genericAssociations = std::move(genericAssociationVec);
        return genericAssocList;
    }
    else
        return nullptr;
}

GenericSelection* Tokenizer::getGenericSelection()
{
    GenericSelection genericSelection;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "_Generic") &&
       isMatched(mIdx, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(genericSelection.assignmentExpression = getAssignmentExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(genericSelection.genericAssocList = getGenericAssocList())
                {
                    if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                        mIdx++;
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new GenericSelection(genericSelection);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Identifier* Tokenizer::getIdentifier()
{
    if(isMatched(mIdx, Token::IDENTIFIER))
    {
        mIdx++;

        return mTokens[mIdx - 1]->uToken.sIdentifier.identifier;
    }
    else
        return nullptr;
}

IdentifierList* Tokenizer::getIdentifierList(IdentifierList* bef)
{
    IdentifierList identifierList;
    
    auto befIdx = mIdx;
    bool isValid = true;

    if(!bef)
    {
        if(identifierList.uIdentifierList.sIdentifier.identifier = getIdentifier())
            identifierList.eIdentifierList = IdentifierList::IDENTIFIER;
        else
            isValid = false;
    }
    else
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, ","))
        {
            mIdx++;

            if(identifierList.uIdentifierList.sIdentifierListIdentifier.identifier = getIdentifier())
            {
                identifierList.uIdentifierList.sIdentifierListIdentifier.identifierList = bef;
                identifierList.eIdentifierList = IdentifierList::IDENTIFIER_LIST_IDENTIFIER;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        IdentifierList* retVal = new IdentifierList(identifierList);
        IdentifierList* aft = getIdentifierList(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

InclusiveORExpression* Tokenizer::getInclusiveORExpression()
{
    std::vector<ExclusiveORExpression*> exclusiveORExpressionVec;

    ExclusiveORExpression* eORe = getExclusiveORExpression();
    if(eORe)
    {
        exclusiveORExpressionVec.push_back(eORe);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "|"))
            {
                mIdx++;

                if(eORe = getExclusiveORExpression())
                    exclusiveORExpressionVec.push_back(eORe);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!exclusiveORExpressionVec.empty())
    {
        InclusiveORExpression* inclusiveORExpression = new InclusiveORExpression();
        inclusiveORExpression->exclusiveORExpressions = std::move(exclusiveORExpressionVec);
        return inclusiveORExpression;
    }
    else
        return nullptr;
}

InitDeclarator* Tokenizer::getInitDeclarator()
{
    InitDeclarator initDeclarator;

    auto befIdx = mIdx;
    bool isValid = true;

    Declarator* declarator = getDeclarator();
    if(declarator)
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "="))
        {
            mIdx++;

            if(initDeclarator.uInitDeclarator.sDeclaratorInitializer.initializer = getInitializer())
            {
                initDeclarator.uInitDeclarator.sDeclaratorInitializer.declarator = declarator;
                initDeclarator.eInitDeclarator = InitDeclarator::DECLARATOR_INITIALIZER;
            }
            else
            {
                mIdx--;
                isValid = false;
            }
        }
        else
            isValid = false;
        
        if(!isValid)
        {
            isValid = true;
            initDeclarator.uInitDeclarator.sDeclarator.declarator = declarator;
            initDeclarator.eInitDeclarator = InitDeclarator::DECLARATOR;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new InitDeclarator(initDeclarator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

InitDeclaratorList* Tokenizer::getInitDeclaratorList(bool isTypedef)
{
    std::vector<InitDeclarator*> initDeclaratorVec;

    auto befIdx = mIdx;

    InitDeclarator* id = getInitDeclarator();
    if(id)
    {
        initDeclaratorVec.push_back(id);
        if(isTypedef)
            addTypedefDeclaration(id);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(id = getInitDeclarator())
                {
                    initDeclaratorVec.push_back(id);
                    if(isTypedef)
                        addTypedefDeclaration(id);
                }
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!initDeclaratorVec.empty())
    {
        InitDeclaratorList* initDeclaratorList = new InitDeclaratorList();
        initDeclaratorList->initDeclarators = std::move(initDeclaratorVec);
        return initDeclaratorList;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Initializer* Tokenizer::getInitializer()
{
    Initializer initializer;

    auto befIdx = mIdx;
    bool isValid = true;

    if(initializer.uInitializer.sAssignmentExpression.assignmentExpression = getAssignmentExpression())
        initializer.eInitializer = Initializer::ASSIGNMENT_EXPRESSION;
    else if(isMatched(mIdx, Token::PUNCTUATOR, "{"))
    {
        mIdx++;

        InitializerList* initializerList = getInitializerList();
        if(initializerList)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "}"))
            {
                mIdx++;

                initializer.uInitializer.sInitializerList.initializerList = initializerList;
                initializer.eInitializer = Initializer::INITIALIZER_LIST;
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, ",") &&
                    isMatched(mIdx + 1, Token::PUNCTUATOR, "}"))
            {
                mIdx += 2;

                initializer.uInitializer.sInitializerListComma.initializerList = initializerList;
                initializer.eInitializer = Initializer::INITIALIZER_LIST_COMMA;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new Initializer(initializer);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

InitializerList* Tokenizer::getInitializerList()
{
    std::vector<InitializerList::SDesignationInitializer> sDesignationInitializerVec;

    InitializerList::SDesignationInitializer sdi;

    sdi.designation = getDesignation();
    if(sdi.initializer = getInitializer())
    {
        sDesignationInitializerVec.push_back(sdi);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                auto midIdx = mIdx;
                mIdx++;

                sdi.designation = getDesignation();
                if(sdi.initializer = getInitializer())
                    sDesignationInitializerVec.push_back(sdi);
                else
                {
                    mIdx = midIdx;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!sDesignationInitializerVec.empty())
    {
        InitializerList* initializerList = new InitializerList();
        initializerList->sDesignationInitializers = std::move(sDesignationInitializerVec);
        return initializerList;
    }
    else
        return nullptr;
}

IterationStatement* Tokenizer::getIterationStatement()
{
    IterationStatement iterationStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "while") &&
       isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(iterationStatement.uIterationStatement.sWhileExpressionStatement.expression = getExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                if(iterationStatement.uIterationStatement.sWhileExpressionStatement.statement = getStatement())
                    iterationStatement.eIterationStatement = IterationStatement::WHILE_EXPRESSION_STATEMENT;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "do"))
    {
        mIdx++;

        if(iterationStatement.uIterationStatement.sDoStatementWhileExpression.statement = getStatement())
        {
            if(isMatched(mIdx, Token::KEYWORD, "while") &&
               isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
            {
                mIdx += 2;

                if(iterationStatement.uIterationStatement.sDoStatementWhileExpression.expression = getExpression())
                {
                    if(isMatched(mIdx, Token::PUNCTUATOR, ")") &&
                       isMatched(mIdx + 1, Token::PUNCTUATOR, ";"))
                    {
                        mIdx += 2;

                        iterationStatement.eIterationStatement = IterationStatement::DO_STATEMENT_WHILE_EXPRESSION;
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "for") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(iterationStatement.uIterationStatement.sForDeclarationExpressionExpressionStatement.declaration = getDeclaration())
        {
            iterationStatement.uIterationStatement.sForDeclarationExpressionExpressionStatement.expression = getExpression();

            if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
            {
                mIdx++;

                iterationStatement.uIterationStatement.sForDeclarationExpressionExpressionStatement.expression_1 = getExpression();

                if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                {
                    mIdx++;

                    if(iterationStatement.uIterationStatement.sForDeclarationExpressionExpressionStatement.statement = getStatement())
                        iterationStatement.eIterationStatement = IterationStatement::FOR_DECLARATION_EXPRESSION_EXPRESSION_STATEMENT;
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
        {
            iterationStatement.uIterationStatement.sForExpressionExpressionExpressionStatement.expression = getExpression();
            
            if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
            {
                mIdx++;

                iterationStatement.uIterationStatement.sForExpressionExpressionExpressionStatement.expression_1 = getExpression();

                if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
                {
                    mIdx++;

                    iterationStatement.uIterationStatement.sForExpressionExpressionExpressionStatement.expression_2 = getExpression();

                    if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                    {
                        mIdx++;

                        if(iterationStatement.uIterationStatement.sForExpressionExpressionExpressionStatement.statement = getStatement())
                            iterationStatement.eIterationStatement = IterationStatement::FOR_EXPRESSION_EXPRESSION_EXPRESSION_STATEMENT;
                        else
                            isValid = false;
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;
    
    if(isValid)
        return new IterationStatement(iterationStatement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

JumpStatement* Tokenizer::getJumpStatement()
{
    JumpStatement jumpStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "goto"))
    {
        mIdx++;

        if(jumpStatement.uJumpStatement.sGotoIdentifier.identifier = getIdentifier())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
            {
                mIdx++;

                jumpStatement.eJumpStatement = JumpStatement::GOTO_IDENTIFIER;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "continue") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, ";"))
    {
        mIdx += 2;
        
        jumpStatement.eJumpStatement = JumpStatement::CONTINUE;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "break") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, ";"))
    {
        mIdx += 2;

        jumpStatement.eJumpStatement = JumpStatement::BREAK;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "return"))
    {
        mIdx++;

        jumpStatement.uJumpStatement.sReturnExpression.expression = getExpression();

        if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
            jumpStatement.eJumpStatement = JumpStatement::RETURN_EXPRESSION;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new JumpStatement(jumpStatement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

LabeledStatement* Tokenizer::getLabeledStatement()
{
    LabeledStatement labeledStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(labeledStatement.uLabeledStatement.sIdentifierStatement.identifier = getIdentifier())
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, ":"))
        {
            mIdx++;

            if(labeledStatement.uLabeledStatement.sIdentifierStatement.statement = getStatement())
                labeledStatement.eLabeledStatement = LabeledStatement::IDENTIFIER_STATEMENT;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "case"))
    {
        mIdx++;

        if(labeledStatement.uLabeledStatement.sCaseConstantExpressionStatement.constantExpression = getConstantExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ":"))
            {
                mIdx++;

                if(labeledStatement.uLabeledStatement.sCaseConstantExpressionStatement.statement = getStatement())
                    labeledStatement.eLabeledStatement = LabeledStatement::CASE_CONSTANT_EXPRESSION_STATEMENT;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "default") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, ":"))
    {
        mIdx += 2;

        if(labeledStatement.uLabeledStatement.sDefaultStatement.statement = getStatement())
            labeledStatement.eLabeledStatement = LabeledStatement::DEFAULT_STATEMENT;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new LabeledStatement(labeledStatement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

LogicalANDExpression* Tokenizer::getLogicalANDExpression()
{
    std::vector<InclusiveORExpression*> inclusiveORExpressionVec;

    InclusiveORExpression* iORe = getInclusiveORExpression();
    if(iORe)
    {
        inclusiveORExpressionVec.push_back(iORe);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "&&"))
            {
                mIdx++;

                if(iORe = getInclusiveORExpression())
                    inclusiveORExpressionVec.push_back(iORe);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!inclusiveORExpressionVec.empty())
    {
        LogicalANDExpression* logicalANDExpression = new LogicalANDExpression();
        logicalANDExpression->inclusiveORExpressions = std::move(inclusiveORExpressionVec);
        return logicalANDExpression;
    }
    else
        return nullptr;
}


LogicalORExpression* Tokenizer::getLogicalORExpression()
{
    std::vector<LogicalANDExpression*> logicalANDExpressionVec;

    LogicalANDExpression* lANDe = getLogicalANDExpression();
    if(lANDe)
    {
        logicalANDExpressionVec.push_back(lANDe);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "||"))
            {
                mIdx++;

                if(lANDe = getLogicalANDExpression())
                    logicalANDExpressionVec.push_back(lANDe);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!logicalANDExpressionVec.empty())
    {
        LogicalORExpression* logicalORExpression = new LogicalORExpression();
        logicalORExpression->logicalANDExpressions = std::move(logicalANDExpressionVec);
        return logicalORExpression;
    }
    else
        return nullptr;
}

MultiplicativeExpression* Tokenizer::getMultiplicativeExpression()
{
    std::vector<std::pair<CastExpression*, MultiplicativeExpression::EMultiplicativeExpression>> castExpressionPairVec;

    CastExpression* ce = getCastExpression();
    if(ce)
    {
        castExpressionPairVec.emplace_back(ce, MultiplicativeExpression::NONE);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "*"))
            {
                mIdx++;

                if(ce = getCastExpression())
                    castExpressionPairVec.emplace_back(ce, MultiplicativeExpression::ASTERISK);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, "/"))
            {
                mIdx++;

                if(ce = getCastExpression())
                    castExpressionPairVec.emplace_back(ce, MultiplicativeExpression::SLASH);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, "%"))
            {
                mIdx++;

                if(ce = getCastExpression())
                    castExpressionPairVec.emplace_back(ce, MultiplicativeExpression::PERCENT);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!castExpressionPairVec.empty())
    {
        MultiplicativeExpression* multiplicativeExpression = new MultiplicativeExpression();
        multiplicativeExpression->castExpressionPairVec = std::move(castExpressionPairVec);
        return multiplicativeExpression;
    }
    else
        return nullptr;
}

ParameterDeclaration* Tokenizer::getParameterDeclaration()
{
    ParameterDeclaration parameterDeclaration;

    auto befIdx = mIdx;
    bool isValid = true;

    DeclarationSpecifiers* declarationSpecifiers = getDeclarationSpecifiers();
    if(declarationSpecifiers)
    {
        if(parameterDeclaration.uParameterDeclaration.sDeclarationSpecifiersDeclarator.declarator = getDeclarator())
        {
            parameterDeclaration.uParameterDeclaration.sDeclarationSpecifiersDeclarator.declarationSpecifiers = declarationSpecifiers;
            parameterDeclaration.eParameterDeclaration = ParameterDeclaration::DECLARATION_SPECIFIERS_DECLARATOR;
        }
        else if(parameterDeclaration.uParameterDeclaration.sDeclarationSpecifiersAbstractDeclarator.abstractDeclarator = getAbstractDeclarator())
        {
            parameterDeclaration.uParameterDeclaration.sDeclarationSpecifiersAbstractDeclarator.declarationSpecifiers = declarationSpecifiers;
            parameterDeclaration.eParameterDeclaration = ParameterDeclaration::DECLARATION_SPECIFIERS_ABSTRACT_DECLARATOR;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new ParameterDeclaration(parameterDeclaration);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

ParameterList* Tokenizer::getParameterList()
{
    std::vector<ParameterDeclaration*> parameterDeclarationVec;

    ParameterDeclaration* pd = getParameterDeclaration();
    if(pd)
    {
        parameterDeclarationVec.push_back(pd);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                if(pd = getParameterDeclaration())
                    parameterDeclarationVec.push_back(pd);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!parameterDeclarationVec.empty())
    {
        ParameterList* parameterList = new ParameterList();
        parameterList->parameterDeclarations = std::move(parameterDeclarationVec);
        return parameterList;
    }
    else
        return nullptr;
}

ParameterTypeList* Tokenizer::getParameterTypeList()
{
    ParameterTypeList parameterTypeList;

    auto befIdx = mIdx;
    bool isValid = true;

    ParameterList* parameterList = getParameterList();
    if(parameterList)
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, ",") &&
           isMatched(mIdx + 1, Token::PUNCTUATOR, "..."))
        {
            mIdx += 2;

            parameterTypeList.uParameterTypeList.sParameterListDotdotdot.parameterList = parameterList;
            parameterTypeList.eParameterTypeList = ParameterTypeList::PARAMETER_LIST_DOTDOTDOT;
        }
        else
        {
            parameterTypeList.uParameterTypeList.sParameterList.parameterList = parameterList;
            parameterTypeList.eParameterTypeList = ParameterTypeList::PARAMETER_LIST;
        }
    }
    else
        isValid  = false;
    
    if(isValid)
        return new ParameterTypeList(parameterTypeList);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Pointer* Tokenizer::getPointer()
{
    Pointer pointer;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "*"))
    {
        mIdx++;

        TypeQualifierList* typeQualifierList = getTypeQualifierList();

        if(pointer.uPointer.sTypeQualifierPointer.pointer = getPointer())
        {
            pointer.uPointer.sTypeQualifierPointer.typeQualifierList = typeQualifierList;
            pointer.ePointer = Pointer::TYPE_QUALIFIER_LIST_POINTER;
        }
        else
        {
            pointer.uPointer.sTypeQualifierList.typeQualifierList = typeQualifierList;
            pointer.ePointer = Pointer::TYPE_QUALIFIER_LIST;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new Pointer(pointer);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

PostfixExpression* Tokenizer::getPostfixExpression(PostfixExpression* bef)
{
    PostfixExpression postfixExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(!bef)
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "("))
        {
            mIdx++;

            TypeName* typeName = getTypeName();
            if(typeName &&
               isMatched(mIdx, Token::PUNCTUATOR, ")") &&
               isMatched(mIdx + 1, Token::PUNCTUATOR, "{"))
            {
                mIdx += 2;

                IdentifierList* identifierList = getIdentifierList();
                if(identifierList &&
                   isMatched(mIdx, Token::PUNCTUATOR, "}"))
                {
                    mIdx++;

                    postfixExpression.uPostfixExpression.sTypeNameIdentifierList.typeName = typeName;
                    postfixExpression.uPostfixExpression.sTypeNameIdentifierList.identifierList = identifierList;
                    postfixExpression.ePostfixExpression = PostfixExpression::TYPE_NAME_INITIALIZER_LIST;
                }
                else if(identifierList &&
                        isMatched(mIdx, Token::PUNCTUATOR, ",") &&
                        isMatched(mIdx + 1, Token::PUNCTUATOR, "}"))
                {
                    mIdx += 2;

                    postfixExpression.uPostfixExpression.sTypeNameIdentifierListDot.typeName = typeName;
                    postfixExpression.uPostfixExpression.sTypeNameIdentifierListDot.identifierList = identifierList;
                    postfixExpression.ePostfixExpression = PostfixExpression::TYPE_NAME_INITIALIZER_LIST_DOT;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;

        if(!isValid)
        {
            mIdx = befIdx;
            isValid = true;

            if(postfixExpression.uPostfixExpression.sPrimaryExpression.primaryExpression = getPrimaryExpression())
                postfixExpression.ePostfixExpression = PostfixExpression::PRIMARY_EXPRESSION;
            else
                isValid = false;
        }
    }
    else
    {
        if(isMatched(mIdx, Token::PUNCTUATOR, "["))
        {
            mIdx++;

            if(postfixExpression.uPostfixExpression.sPostfixExpressionExpression.expression = getExpression())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, "]"))
                {
                    mIdx++;

                    postfixExpression.uPostfixExpression.sPostfixExpressionExpression.postfixExpression = bef;
                    postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_EXPRESSION;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "("))
        {
            mIdx++;

            postfixExpression.uPostfixExpression.sPostfixExpressionArgumentExpressionList.argumentExpressionList = getArgumentExpressionList();

            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                postfixExpression.uPostfixExpression.sPostfixExpressionArgumentExpressionList.postfixExpression = bef;
                postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_ARGUMENT_EXPRESSION_LIST;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "."))
        {
            mIdx++;

            if(postfixExpression.uPostfixExpression.sPostfixExpressionDotIdentifier.identifier = getIdentifier())
            {
                postfixExpression.uPostfixExpression.sPostfixExpressionDotIdentifier.postfixExpression = bef;
                postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_DOT_IDENTIFIER;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "->"))
        {
            mIdx++;

            if(postfixExpression.uPostfixExpression.sPostfixExpressionArrowIdentifier.identifier = getIdentifier())
            {
                postfixExpression.uPostfixExpression.sPostfixExpressionArrowIdentifier.postfixExpression = bef;
                postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_ARROW_IDENTIFIER;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "++"))
        {
            mIdx++;

            postfixExpression.uPostfixExpression.sPostfixExpressionPlusplus.postfixExpression = bef;
            postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_PLUSPLUS;
        }
        else if(isMatched(mIdx, Token::PUNCTUATOR, "--"))
        {
            mIdx++;

            postfixExpression.uPostfixExpression.sPostfixExpressionMinusminus.postfixExpression = bef;
            postfixExpression.ePostfixExpression = PostfixExpression::POSTFIX_EXPRESSION_MINUSMINUS;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        PostfixExpression* retVal = new PostfixExpression(postfixExpression);
        PostfixExpression* aft = getPostfixExpression(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

PrimaryExpression* Tokenizer::getPrimaryExpression()
{
    PrimaryExpression primaryExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(primaryExpression.uPrimaryExpression.sIdentifier.identifier = getIdentifier())
        primaryExpression.ePrimaryExpression = PrimaryExpression::IDENTIFIER;
    else if(primaryExpression.uPrimaryExpression.sConstant.constant = getConstant())
        primaryExpression.ePrimaryExpression = PrimaryExpression::CONSTANT;
    else if(primaryExpression.uPrimaryExpression.sStringLiteral.stringLiteral = getStringLiteral())
        primaryExpression.ePrimaryExpression = PrimaryExpression::STRING_LITERAL;
    else if(primaryExpression.uPrimaryExpression.sGenericSelection.genericSelection = getGenericSelection())
        primaryExpression.ePrimaryExpression = PrimaryExpression::GENERIC_SELECTION;
    else if(isMatched(mIdx, Token::PUNCTUATOR, "("))
    {
        mIdx++;

        if(primaryExpression.uPrimaryExpression.sExpression.expression = getExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                primaryExpression.ePrimaryExpression = PrimaryExpression::EXPRESSION;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new PrimaryExpression(primaryExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

RelationalExpression* Tokenizer::getRelationalExpression()
{
    std::vector<std::pair<ShiftExpression*, RelationalExpression::ERelationalExpression>> shiftExpressionPairVec;

    ShiftExpression* se = getShiftExpression();
    if(se)
    {
        shiftExpressionPairVec.emplace_back(se, RelationalExpression::NONE);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "<"))
            {
                mIdx++;

                if(se = getShiftExpression())
                    shiftExpressionPairVec.emplace_back(se, RelationalExpression::LESS);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, ">"))
            {
                mIdx++;

                if(se = getShiftExpression())
                    shiftExpressionPairVec.emplace_back(se, RelationalExpression::GREATER);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, "<="))
            {
                mIdx++;

                if(se = getShiftExpression())
                    shiftExpressionPairVec.emplace_back(se, RelationalExpression::LESS_EQUAL);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, ">="))
            {
                mIdx++;

                if(se = getShiftExpression())
                    shiftExpressionPairVec.emplace_back(se, RelationalExpression::GREATER_EQUAL);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!shiftExpressionPairVec.empty())
    {
        RelationalExpression* relationalExpression = new RelationalExpression();
        relationalExpression->shiftExpressionPairVec = std::move(shiftExpressionPairVec);
        return relationalExpression;
    }
    else
        return nullptr;
}

SelectionStatement* Tokenizer::getSelectionStatement()
{
    SelectionStatement selectionStatement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "if") &&
       isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        Expression* expression = getExpression();
        if(expression &&
           isMatched(mIdx, Token::PUNCTUATOR, ")"))
        {
            mIdx++;

            Statement* statement = getStatement();
            if(statement)
            {
                if(isMatched(mIdx, Token::KEYWORD, "else"))
                {
                    mIdx++;

                    if(selectionStatement.uSelectionStatement.sIfExpressionStatementElseStatement.statement_1 = getStatement())
                    {
                        selectionStatement.uSelectionStatement.sIfExpressionStatementElseStatement.expression = expression;
                        selectionStatement.uSelectionStatement.sIfExpressionStatementElseStatement.statement = statement;
                        selectionStatement.eSelectionStatement = SelectionStatement::IF_EXPRESSION_STATEMENT_ELSE_STATEMENT;
                    }
                    else
                        isValid = false;
                }
                else
                {
                    selectionStatement.uSelectionStatement.sIfExpressionStatement.expression = expression;
                    selectionStatement.uSelectionStatement.sIfExpressionStatement.statement = statement;
                    selectionStatement.eSelectionStatement = SelectionStatement::IF_EXPRESSION_STATEMENT;
                }
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "switch") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(selectionStatement.uSelectionStatement.sSwitchExpressionStatement.expression = getExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                if(selectionStatement.uSelectionStatement.sSwitchExpressionStatement.statement = getStatement())
                    selectionStatement.eSelectionStatement = SelectionStatement::SWITCH_EXPRESSION_STATEMENT;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new SelectionStatement(selectionStatement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

ShiftExpression* Tokenizer::getShiftExpression()
{
    std::vector<std::pair<AdditiveExpression*, ShiftExpression::EShiftExpression>> additiveExpressionPairVec;

    AdditiveExpression* ae = getAdditiveExpression();
    if(ae)
    {
        additiveExpressionPairVec.emplace_back(ae, ShiftExpression::NONE);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, "<<"))
            {
                mIdx++;

                if(ae = getAdditiveExpression())
                    additiveExpressionPairVec.emplace_back(ae, ShiftExpression::LEFT);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else if(isMatched(mIdx, Token::PUNCTUATOR, ">>"))
            {
                mIdx++;

                if(ae = getAdditiveExpression())
                    additiveExpressionPairVec.emplace_back(ae, ShiftExpression::RIGHT);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!additiveExpressionPairVec.empty())
    {
        ShiftExpression* shiftExpression = new ShiftExpression();
        shiftExpression->additiveExpressionPairVec = std::move(additiveExpressionPairVec);
        return shiftExpression;
    }
    else
        return nullptr;
}

SpecifierQualifierList* Tokenizer::getSpecifierQualifierList()
{
    SpecifierQualifierList specifierQualifierList;

    auto befIdx = mIdx;
    bool isValid = true;

    if(specifierQualifierList.uSpecifierQualifier.sTypeSpecifierSpecifierQualifierList.typeSpecifier = getTypeSpecifier())
    {
        specifierQualifierList.uSpecifierQualifier.sTypeSpecifierSpecifierQualifierList.specifierQualifierList = getSpecifierQualifierList();
        specifierQualifierList.eSpecifierQualifierList = SpecifierQualifierList::TYPE_SPECIFIER_SPECIFIER_QUALIFIER_LIST;
    }
    else if(specifierQualifierList.uSpecifierQualifier.sTypeQualifierSpecifierQualifierList.typeQualifier = getTypeQualifier())
    {
        specifierQualifierList.uSpecifierQualifier.sTypeQualifierSpecifierQualifierList.specifierQualifierList = getSpecifierQualifierList();
        specifierQualifierList.eSpecifierQualifierList = SpecifierQualifierList::TYPE_QUALIFIER_SPECIFIER_QUALIFIER_LIST;
    }
    else
        isValid = false;

    if(isValid)
        return new SpecifierQualifierList(specifierQualifierList);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

Statement* Tokenizer::getStatement()
{
    Statement statement;

    auto befIdx = mIdx;
    bool isValid = true;

    if(statement.uStatement.sLabeledStatement.labeledStatement = getLabeledStatement())
        statement.eStatement = Statement::LABELED_STATEMENT;
    else if(statement.uStatement.sCompoundStatement.compoundStatement = getCompoundStatement())
        statement.eStatement = Statement::COMPOUND_STATEMENT;
    else if(statement.uStatement.sExpressionStatement.expressionStatement = getExpressionStatement())
        statement.eStatement = Statement::EXPRESSION_STATEMENT;
    else if(statement.uStatement.sSelectionStatement.selectionStatement = getSelectionStatement())
        statement.eStatement = Statement::SELECTION_STATEMENT;
    else if(statement.uStatement.sIterationStatement.iterationStatement = getIterationStatement())
        statement.eStatement = Statement::ITERATION_STATEMENT;
    else if(statement.uStatement.sJumpStatement.jumpStatement = getJumpStatement())
        statement.eStatement = Statement::JUMP_STATEMENT;
    else
        isValid = false;
    
    if(isValid)
        return new Statement(statement);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

StaticAssertDeclaration* Tokenizer::getStaticAssertDeclaration()
{
    StaticAssertDeclaration staticAssertDeclaration;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::KEYWORD, "_Static_assert") &&
       isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(staticAssertDeclaration.constantExpression = getConstantExpression())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ",") &&
               isMatched(mIdx + 1, Token::STRING_LITERAL) &&
               isMatched(mIdx + 2, Token::PUNCTUATOR, ")") &&
               isMatched(mIdx + 3, Token::PUNCTUATOR, ";"))
            {
                mIdx += 4;

                staticAssertDeclaration.stringLiteral = mTokens[mIdx - 3]->uToken.sStringLiteral.stringLiteral;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new StaticAssertDeclaration(staticAssertDeclaration);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

StorageClassSpecifier* Tokenizer::getStorageClassSpecifier()
{
    if(mIdx >= mTokens.size())
        return nullptr;

    std::string data;
    TOKEN::getString(mTokens[mIdx], data);

    if(data == "typedef" ||
       data == "extern" ||
       data == "static" ||
       data == "_Thread_local" ||
       data == "auto" ||
       data == "register")
    {
        mIdx++;

        StorageClassSpecifier* storageClassSpecifier = new StorageClassSpecifier();
        storageClassSpecifier->element = std::move(data);
        return storageClassSpecifier;
    }
    else
        return nullptr;
}

StringLiteral* Tokenizer::getStringLiteral()
{
    if(isMatched(mIdx, Token::STRING_LITERAL))
    {
        mIdx++;

        return mTokens[mIdx - 1]->uToken.sStringLiteral.stringLiteral;
    }
    else
        return nullptr;
}

StructDeclaration* Tokenizer::getStructDeclaration()
{
    StructDeclaration structDeclaration;

    auto befIdx = mIdx;
    bool isValid = true;

    if(structDeclaration.uStructDeclaration.sSpecifierQualifierListStructDeclaratorList.specifierQualifierList = getSpecifierQualifierList())
    {
        structDeclaration.uStructDeclaration.sSpecifierQualifierListStructDeclaratorList.structDeclaratorList = getStructDeclaratorList();

        if(isMatched(mIdx, Token::PUNCTUATOR, ";"))
        {
            mIdx++;

            structDeclaration.eStructDeclaration = StructDeclaration::SPECIFIER_QUALIFIER_LIST_STRUCT_DECLARATOR_LIST;
        }
        else
            isValid = false;
    }
    else if(structDeclaration.uStructDeclaration.sStaticAssertDeclaration.staticAssertDeclaration = getStaticAssertDeclaration())
        structDeclaration.eStructDeclaration = StructDeclaration::STATIC_ASSERT_DECLARATION;
    else
        isValid = false;

    if(isValid)
        return new StructDeclaration(structDeclaration);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

StructDeclarationList* Tokenizer::getStructDeclarationList()
{
    std::vector<StructDeclaration*> structDeclarationVec;

    for(StructDeclaration* sd = getStructDeclaration(); sd != nullptr; sd = getStructDeclaration())
        structDeclarationVec.push_back(sd);
    
    if(!structDeclarationVec.empty())
    {
        StructDeclarationList* structDeclarationList = new StructDeclarationList();
        structDeclarationList->structDeclarations = std::move(structDeclarationVec);
        return structDeclarationList;
    }
    else
        return nullptr;
}

StructDeclarator* Tokenizer::getStructDeclarator()
{
    StructDeclarator structDeclarator;

    auto befIdx = mIdx;
    bool isValid = true;

    Declarator* declarator = getDeclarator();
    
    if(isMatched(mIdx, Token::PUNCTUATOR, ":"))
    {
        mIdx++;

        if(structDeclarator.uStructDeclarator.sDeclaratorConstantExpression.constantExpression = getConstantExpression())
        {
            structDeclarator.uStructDeclarator.sDeclaratorConstantExpression.declarator = declarator;
            structDeclarator.eStructDeclarator = StructDeclarator::DECLARATOR_CONSTANT_EXPRESSION;
        }
        else
        {
            mIdx--;
            isValid = false;
        }
    }

    if(declarator &&
       !isValid)
    {
        isValid = true;

        structDeclarator.uStructDeclarator.sDeclarator.declarator = declarator;
        structDeclarator.eStructDeclarator = StructDeclarator::DECLARATOR;
    }

    if(isValid)
        return new StructDeclarator(structDeclarator);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

StructDeclaratorList* Tokenizer::getStructDeclaratorList()
{
    std::vector<StructDeclarator*> structDeclaratorVec;

    StructDeclarator* sd = getStructDeclarator();
    if(sd)
    {
        structDeclaratorVec.push_back(sd);

        while(1)
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ","))
            {
                mIdx++;

                sd = getStructDeclarator();
                if(sd)
                    structDeclaratorVec.push_back(sd);
                else
                {
                    mIdx--;
                    break;
                }
            }
            else
                break;
        }
    }

    if(!structDeclaratorVec.empty())
    {
        StructDeclaratorList* structDeclaratorList = new StructDeclaratorList();
        structDeclaratorList->structDeclarators = std::move(structDeclaratorVec);
        return structDeclaratorList;
    }
    else
        return nullptr;
}

StructOrUnion* Tokenizer::getStructOrUnion()
{
    if(mIdx >= mTokens.size())
        return nullptr;

    std::string data;
    TOKEN::getString(mTokens[mIdx], data);

    if(data == "struct" ||
       data == "union")
    {
        mIdx++;

        StructOrUnion* structOrUnion = new StructOrUnion();
        structOrUnion->element = std::move(data);
        return structOrUnion;
    }
    else
        return nullptr;
}

StructOrUnionSpecifier* Tokenizer::getStructOrUnionSpecifier()
{
    StructOrUnionSpecifier structOrUnionSpecifier;

    auto befIdx = mIdx;
    bool isValid = true;

    StructOrUnion* structOrUnion = getStructOrUnion();
    if(structOrUnion)
    {
        Identifier* identifier = getIdentifier();
        auto midIdx = mIdx;
        
        if(isMatched(mIdx, Token::PUNCTUATOR, "{"))
        {
            mIdx++;

            if(structOrUnionSpecifier.uStructOrUnionSpecifier.sStructOrUnionIdentifierStructDeclarationList.structDeclarationList = getStructDeclarationList())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, "}"))
                {
                    mIdx++;

                    structOrUnionSpecifier.uStructOrUnionSpecifier.sStructOrUnionIdentifierStructDeclarationList.structOrUnion = structOrUnion;
                    structOrUnionSpecifier.uStructOrUnionSpecifier.sStructOrUnionIdentifierStructDeclarationList.identifier = identifier;
                    structOrUnionSpecifier.eStructOrUnionSpecifier = StructOrUnionSpecifier::STRUCT_OR_UNION_IDENTIFIER_STRUCT_DECLARATION_LIST;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;

        if(!isValid &&
           identifier)
        {
            mIsValid = true;
            mIdx = midIdx;

            structOrUnionSpecifier.uStructOrUnionSpecifier.sStructOrUnionIdentifier.structOrUnion = structOrUnion;
            structOrUnionSpecifier.uStructOrUnionSpecifier.sStructOrUnionIdentifier.identifier = identifier;
            structOrUnionSpecifier.eStructOrUnionSpecifier = StructOrUnionSpecifier::STRUCT_OR_UNION_IDENTIFIER;
        }
    }
    else
        isValid = false;

    if(isValid)
        return new StructOrUnionSpecifier(structOrUnionSpecifier);
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

    mTypedefNames.emplace_back();

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

    mTypedefNames.pop_back();

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

TypedefName* Tokenizer::getTypedefName()
{
    if(isMatched(mIdx, Token::IDENTIFIER))
    {
        if(isDeclarated(mTokens[mIdx]->uToken.sIdentifier.identifier))
        {
            mIdx++;

            TypedefName* typedefName = new TypedefName();
            typedefName->identifier = mTokens[mIdx - 1]->uToken.sIdentifier.identifier;
            return typedefName;
        }
        else
            return nullptr;
    }
    else
        return nullptr;
}

TypeName* Tokenizer::getTypeName()
{
    TypeName typeName;

    auto befIdx = mIdx;
    bool isValid = true;

    if(typeName.specifierQualifierList = getSpecifierQualifierList())
    {
        if(typeName.abstractDeclarator = getAbstractDeclarator())
            ;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new TypeName(typeName);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

TypeQualifier* Tokenizer::getTypeQualifier()
{
    if(mIdx >= mTokens.size())
        return nullptr;

    std::string data;
    TOKEN::getString(mTokens[mIdx], data);

    if(data == "const" ||
       data == "restrict" ||
       data == "volatile" ||
       data == "_Atomic")
    {
        mIdx++;
    
        TypeQualifier* typeQualifier = new TypeQualifier();
        typeQualifier->element = std::move(data);
        return typeQualifier;
    }
    else
        return nullptr;
}

TypeQualifierList* Tokenizer::getTypeQualifierList()
{
    std::vector<TypeQualifier*> typeQualifierVec;

    for(TypeQualifier* tq = getTypeQualifier(); tq != nullptr; tq = getTypeQualifier())
        typeQualifierVec.push_back(tq);
    
    if(!typeQualifierVec.empty())
    {
        TypeQualifierList* typeQualifierList = new TypeQualifierList();
        typeQualifierList->typeQualifiers = std::move(typeQualifierVec);
        return typeQualifierList;
    }
    else
        return nullptr;
}

TypeSpecifier* Tokenizer::getTypeSpecifier()
{
    if(mIdx >= mTokens.size())
        return nullptr;

    TypeSpecifier typeSpecifier;

    auto befIdx = mIdx;
    bool isValid = true;
    
    std::string data;
    TOKEN::getString(mTokens[mIdx], data);
    
    if(data == "void" ||
       data == "char" ||
       data == "short" ||
       data == "int" ||
       data == "long" ||
       data == "float" ||
       data == "double" ||
       data == "signed" ||
       data == "unsigned" ||
       data == "_Bool" ||
       data == "_Complex")
    {
        mIdx++;

        typeSpecifier.uTypeSpecifier.sBase.element = new std::string(data);
        typeSpecifier.eTypeSpecifier = TypeSpecifier::BASE;
    }
    else if(typeSpecifier.uTypeSpecifier.sAtomicTypeSpecifier.atomicTypeSpecifier = getAtomicTypeSpecifier())
        typeSpecifier.eTypeSpecifier = TypeSpecifier::ATOMIC_TYPE_SPECIFIER;
    else if(typeSpecifier.uTypeSpecifier.sStructOrUnionSpecifier.structOrUnionSpecifier = getStructOrUnionSpecifier())
        typeSpecifier.eTypeSpecifier = TypeSpecifier::STRUCT_OR_UNION_SPECIFIER;
    else if(typeSpecifier.uTypeSpecifier.sEnumSpecifier.enumSpecifier = getEnumSpecifier())
        typeSpecifier.eTypeSpecifier = TypeSpecifier::ENUM_SPECIFIER;
    else if(typeSpecifier.uTypeSpecifier.sTypedefName.typedefName = getTypedefName())
        typeSpecifier.eTypeSpecifier = TypeSpecifier::TYPEDEF_NAME;
    else
        isValid = false;

    if(isValid)
        return new TypeSpecifier(typeSpecifier);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

UnaryExpression* Tokenizer::getUnaryExpression()
{
    UnaryExpression unaryExpression;

    auto befIdx = mIdx;
    bool isValid = true;

    if(isMatched(mIdx, Token::PUNCTUATOR, "++"))
    {
        mIdx++;

        if(unaryExpression.uUnaryExpression.sPlusPlusUnaryExpression.unaryExpression = getUnaryExpression())
            unaryExpression.eUnaryExpression = UnaryExpression::PLUS_PLUS_UNARY_EXPRESSION;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::PUNCTUATOR, "--"))
    {
        mIdx++;

        if(unaryExpression.uUnaryExpression.sMinusMinusUnaryExpression.unaryExpression = getUnaryExpression())
            unaryExpression.eUnaryExpression = UnaryExpression::MINUS_MINUS_UNARY_EXPRESSION;
        else
            isValid = false;
    }
    else if(unaryExpression.uUnaryExpression.sUnaryOperatorCastExpression.unaryOperator = getUnaryOperator())
    {
        if(unaryExpression.uUnaryExpression.sUnaryOperatorCastExpression.castExpression = getCastExpression())
            unaryExpression.eUnaryExpression = UnaryExpression::UNARY_OPERATOR_CAST_EXPRESSION;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "_Alignof") &&
            isMatched(mIdx + 1, Token::PUNCTUATOR, "("))
    {
        mIdx += 2;

        if(unaryExpression.uUnaryExpression.sSizeofTypeName.typeName = getTypeName())
        {
            if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
            {
                mIdx++;

                unaryExpression.eUnaryExpression = UnaryExpression::SIZEOF_TYPE_NAME;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, Token::KEYWORD, "sizeof"))
    {
        mIdx++;
        auto midIdx = mIdx;

        if(isMatched(mIdx, Token::PUNCTUATOR, "("))
        {
            mIdx++;

            if(unaryExpression.uUnaryExpression.sSizeofTypeName.typeName = getTypeName())
            {
                if(isMatched(mIdx, Token::PUNCTUATOR, ")"))
                {
                    mIdx++;

                    unaryExpression.eUnaryExpression = UnaryExpression::SIZEOF_TYPE_NAME;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else
            isValid = false;

        if(!isValid)
        {
            mIdx = midIdx;

            if(unaryExpression.uUnaryExpression.sSizeofUnaryExpression.unaryExpression = getUnaryExpression())
            {
                isValid = true;

                unaryExpression.eUnaryExpression = UnaryExpression::SIZEOF_UNARY_EXPRESSION;
            }
        }
    }
    else if(unaryExpression.uUnaryExpression.sPostfixExpression.postfixExpression = getPostfixExpression())
        unaryExpression.eUnaryExpression = UnaryExpression::POSTFIX_EXPRESSION;
    else
        isValid = false;
    
    if(isValid)
        return new UnaryExpression(unaryExpression);
    else
    {
        mIdx = befIdx;
        return nullptr;
    }
}

UnaryOperator* Tokenizer::getUnaryOperator()
{
    if(!isMatched(mIdx, Token::PUNCTUATOR))
        return nullptr;
    
    std::string data;
    TOKEN::getString(mTokens[mIdx], data);

    if(data == "&" ||
       data == "*" ||
       data == "+" ||
       data == "-" ||
       data == "~" ||
       data == "!")
    {
        mIdx++;

        UnaryOperator* unaryOperator = new UnaryOperator();
        unaryOperator->punctuator = mTokens[mIdx - 1]->uToken.sPunctuator.punctuator;
        return unaryOperator;
    }
    else
        return nullptr;
}

bool Tokenizer::isTypedefDeclaration(DeclarationSpecifiers* declarationSpecifier) const
{
    bool isTypedef = false;

    for(DeclarationSpecifiers* ds = declarationSpecifier; ds != nullptr;)
    {
        switch(ds->eDeclarationSpecifiers)
        {
            case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
                if(ds->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier->element == "typedef")
                {
                    isTypedef = true;
                    ds = nullptr;
                }
                else
                    ds = ds->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier;
                break;
            case(DeclarationSpecifiers::TYPE_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::TYPE_QUALIFIER):
                ds = ds->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers;
                break;
            
            default:
                ds = nullptr;
                std::cerr << "implementaion-error:\n"
                          << "    what: unexpected enumeration.\n"
                          << "    enum: " << ds->eDeclarationSpecifiers
                          << std::endl;
                break;
        }
    }

    return isTypedef;
}

bool Tokenizer::isDeclarated(Identifier* identifier) const
{
    bool isValid = false;

    std::string data;
    TOKEN::getString(identifier, data);

    for(auto&& map : mTypedefNames)
    {
        if(map.find(data) != map.end())
        {
            isValid = true;
            break;
        }
    }

    return isValid;
}

void Tokenizer::extractIdentifierData(Declarator* declarator, std::string& data) const
{
    if(declarator)
    {
        for(DirectDeclarator* dd = declarator->directDeclarator; dd != nullptr;)
        {
            switch(dd->eDirectDeclarator)
            {
                case(DirectDeclarator::IDENTIFIER):
                    TOKEN::getString(dd->uDirectDeclarator.sIdentifier.identifier, data);
                    break;
                case(DirectDeclarator::DECLARATOR):
                    extractIdentifierData(dd->uDirectDeclarator.sDeclarator.declarator, data);
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorTypeQualifierListAssignmentExpression.directDeclarator;
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_STATIC_TYPE_QUALIFIER_LIST_ASSIGNMENT_EXPRESSION):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorStaticTypeQualifierListAssignmentExpression.directDeclarator;
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST_STATIC_ASSIGNMENT_EXPRESSION):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorTypeQualifierListStaticAssignmentExpression.directDeclarator;
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_TYPE_QUALIFIER_LIST):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorTypeQualifierList.directDeclarator;
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_PARAMETER_TYPE_LIST):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorParameterTypeList.directDeclarator;
                    break;
                case(DirectDeclarator::DIRECT_DECLARATOR_IDENTIFIER_LIST):
                    dd = dd->uDirectDeclarator.sDirectDeclaratorIdentifierList.directDeclarator;
                    break;
                
                default:
                    dd = nullptr;
                    std::cerr << "implementation-error:\n"
                              << "    what: unexpected enumuration.\n"
                              << "    enum: " << dd->eDirectDeclarator
                              << std::endl;
                    break;
            }
        }
    }
}

void Tokenizer::addTypedefDeclaration(InitDeclarator* initDeclarator)
{
    if(initDeclarator)
    {
        Declarator* declarator = nullptr;

        switch(initDeclarator->eInitDeclarator)
        {
            case(InitDeclarator::DECLARATOR):
                declarator = initDeclarator->uInitDeclarator.sDeclarator.declarator;
                break;
            case(InitDeclarator::DECLARATOR_INITIALIZER):
                declarator = initDeclarator->uInitDeclarator.sDeclaratorInitializer.declarator;
                break;

            default:
                std::cerr << "implementation-error:\n"
                          << "    what: unexpected enumration.\n"
                          << "    enum: " << initDeclarator->eInitDeclarator
                          << std::endl;
        }

        std::string data;
        extractIdentifierData(declarator, data);
        mTypedefNames.back().emplace(data, 0);
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