#include "pp_directive_processor.hpp"
#include "preprocessor.hpp"
#include "macro.hpp"
#include <iostream>

PPDirectiveProcessor::PPDirectiveProcessor(Preprocessor* pp):
    mPP(pp),
    mIsValid(true)
{
}

bool PPDirectiveProcessor::execute()
{
    process(mPP->mPreprocessingFile);

    std::string data;
    for(auto&& e : mPP->mProcessedPPTokens)
    {
        data.clear();
        TOKEN::getString(e, data);
        std::cout << data << std::endl;
    }

    return mIsValid;
}

void PPDirectiveProcessor::process(ControlLine* controlLine)
{
    switch(controlLine->eControlLine)
    {
        case(ControlLine::INCLUDE_PP_TOKENS_NEW_LINE):
            ctrlInclude(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::UNDEF_IDENTIFIER_NEW_LINE):
            ctrlUndef(controlLine);
            break;
        case(ControlLine::LINE_PP_TOKENS_NEW_LINE):
            ctrlLine(controlLine);
            break;
        case(ControlLine::ERROR_PP_TOKENS_NEW_LINE):
            ctrlError(controlLine);
            break;
        case(ControlLine::PRAGMA_PP_TOKENS_NEW_LINE):
            ctrlPragma(controlLine);
            break;
        case(ControlLine::NEW_LINE):
            ctrl(controlLine);
            break;

        default:
            unexpectedEnumError("ControlLine", controlLine->eControlLine);
            break;
    }
}

void PPDirectiveProcessor::process(Group* group)
{
    // group convert grouppart vector
    std::vector<GroupPart*> groupPartVec;
    std::vector<GroupPart*> inversedGroupPartVec;
    for(Group* g = group; g != nullptr;)
    {
        switch(g->eGroup)
        {
            case(Group::GROUP_PART):
                inversedGroupPartVec.push_back(g->uGroup.sGroupPart.groupPart);
                g = nullptr;
                break;
            case(Group::GROUP_GROUP_PART):
                inversedGroupPartVec.push_back(g->uGroup.sGroupGroupPart.groupPart);
                g = g->uGroup.sGroupGroupPart.group;
                break;
            
            default:
                unexpectedEnumError("Group", g->eGroup);
                break;
        }
    }
    for(auto iter = inversedGroupPartVec.rbegin();
        iter != inversedGroupPartVec.rend();
        iter++)
        groupPartVec.push_back(*iter);

    // concatenate textline
    for(std::size_t i = 0; i + 1 < groupPartVec.size(); i++)
    {
        if(groupPartVec[i]->eGroupPart == GroupPart::TEXT_LINE &&
           groupPartVec[i + 1]->eGroupPart == GroupPart::TEXT_LINE)
        {
            PPTokens* result = nullptr;
            if(groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens &&
               !groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens)
                result = groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens;
            else if(!groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens &&
                    groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens)
                result = groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens;
            else if(groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens &&
                    groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens)
            {
                PPTokens* f = groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens;
                for(bool isContinued = true; isContinued;)
                {
                    switch(f->ePPTokens)
                    {
                        case(PPTokens::PREPROCESSING_TOKEN):
                            isContinued = false;
                            break;
                        case(PPTokens::PP_TOKENS_PREPROCESSING_TOKEN):
                            f = f->uPPTokens.sPPTokensPreprocessingToken.ppTokens;
                            break;

                        default:
                            unexpectedEnumError("PPTokens", f->ePPTokens);
                            break;
                    }
                }

                PPTokens::UPPTokens::SPPTokensPreprocessingToken tmp = {groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens, f->uPPTokens.sPreprocessingToken.preprocessingToken};
                f->ePPTokens = PPTokens::PP_TOKENS_PREPROCESSING_TOKEN;
                f->uPPTokens.sPPTokensPreprocessingToken = tmp;

                result = groupPartVec[i + 1]->uGroupPart.sTextLine.textLine->ppTokens;
            }

            if(result)
            {
                groupPartVec[i]->uGroupPart.sTextLine.textLine->ppTokens = result;
                groupPartVec.erase(groupPartVec.begin() + i + 1);
            }
            else
            {
                groupPartVec.erase(groupPartVec.begin() + i,
                                   groupPartVec.begin() + i + 2);
            }
            i--;
        }
    }

    for(auto&& e : groupPartVec)
        process(e);
}

void PPDirectiveProcessor::process(GroupPart* groupPart)
{
    switch(groupPart->eGroupPart)
    {
        case(GroupPart::IF_SECTION):
            process(groupPart->uGroupPart.sIfSection.ifSection);
            break;
        case(GroupPart::CONTROL_LINE):
            process(groupPart->uGroupPart.sControlLine.controlLine);
            break;
        case(GroupPart::TEXT_LINE):
            process(groupPart->uGroupPart.sTextLine.textLine);
            break;
        case(GroupPart::NON_DIRECTIVE):
            process(groupPart->uGroupPart.sNonDirective.nonDirective);
        
        default:
            unexpectedEnumError("GroupPart", groupPart->eGroupPart);
            break;
    }
}

void PPDirectiveProcessor::process(IfSection* ifSection)
{
    bool isConsumed = false;

    // if-group
    switch(ifSection->ifGroup->eIfGroup)
    {
        case(IfGroup::IF_PP_TOKENS_NEW_LINE_GROUP):
            if(isNonzero(ifSection->ifGroup->uIfGroup.sIfPPTokensNewLineGroup.ppTokens))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uIfGroup.sIfPPTokensNewLineGroup.group)
                    process(ifSection->ifGroup->uIfGroup.sIfPPTokensNewLineGroup.group);
            }
            break;
        case(IfGroup::IFDEF_IDENTIFIER_NEW_LINE_GROUP):
            if(isDefined(ifSection->ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.group)
                    process(ifSection->ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.group);
            }
            break;
        case(IfGroup::IFNDEF_IDENTIFIER_NEW_LINE_GROUP):
            if(!isDefined(ifSection->ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.group)
                    process(ifSection->ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.group);
            }
            break;
        
        default:
            unexpectedEnumError("IfGroup", ifSection->ifGroup->eIfGroup);
            break;
    }

    // elif-groups
    if(!isConsumed &&
       ifSection->elifGroups)
    {
        std::vector<ElifGroup*> inversedElifGroupVec;
        for(ElifGroups* e = ifSection->elifGroups; e != nullptr;)
        {
            switch(e->eElifGroups)
            {
                case(ElifGroups::ELIF_GROUP):
                    inversedElifGroupVec.push_back(e->uElifGroups.sElifGroup.elifGroup);
                    e = nullptr;
                    break;
                case(ElifGroups::ELIF_GROUPS_ELIF_GROUP):
                    inversedElifGroupVec.push_back(e->uElifGroups.sElifGroupsElifGroup.elifGroup);
                    e = e->uElifGroups.sElifGroupsElifGroup.elifGroups;
                    break;
                
                default:
                    unexpectedEnumError("ElifGroups", e->eElifGroups);
                    break;
            }
        }

        for(auto i = inversedElifGroupVec.size(); i > 0; i--)
        {
            if(isNonzero(inversedElifGroupVec[i - 1]->ppTokens))
            {
                isConsumed = true;
                if(inversedElifGroupVec[i - 1]->group)
                    process(inversedElifGroupVec[i - 1]->group);
                break;
            }
        }
    }

    // else-group
    if(!isConsumed &&
       ifSection->elseGroup)
    {
        isConsumed = true;
        if(ifSection->elseGroup->group)
            process(ifSection->elseGroup->group);
    }
}

void PPDirectiveProcessor::process(PreprocessingFile* preprocessingFile)
{
    if(preprocessingFile->group)
        process(preprocessingFile->group);
}

void PPDirectiveProcessor::process(TextLine* textLine)
{
    std::vector<PreprocessingToken*> ppTokenVec;
    convertPPTokens(textLine->ppTokens, ppTokenVec);

    replaceMacro(ppTokenVec);
    
    mPP->mProcessedPPTokens.insert(mPP->mProcessedPPTokens.end(),
                                   ppTokenVec.begin(),
                                   ppTokenVec.end());
}

void PPDirectiveProcessor::replaceMacro(std::vector<PreprocessingToken*>& ppTokenVec)
{
    for(std::size_t i = 0; i < ppTokenVec.size(); i++)
    {
        if(ppTokenVec[i]->ePreprocessingToken == PreprocessingToken::IDENTIFIER)
        {
            std::string identifier;
            TOKEN::getString(ppTokenVec[i]->uPreprocessingToken.sIdentifier.identifier, identifier);
            for(auto&& macro : Macro::MACROS)
            {
                if(identifier == macro.identifier)
                {
                    // object-like
                    if(!macro.isFunction)
                    {
                        ppTokenVec.erase(ppTokenVec.begin() + i);
                        ppTokenVec.insert(ppTokenVec.begin() + i,
                                          macro.replacements.begin(),
                                          macro.replacements.end());
                        
                        i--;
                    }
                    // function-like
                    else
                    {
                        bool isValid = true;

                        std::size_t endPos = i + 2;
                        std::size_t sepPos = i + 1;
                        std::vector<std::vector<PreprocessingToken*>> argsVec;

                        // separate arguments
                        if(isMatched(ppTokenVec, i + 1, PreprocessingToken::PUNCTUATOR, "("))
                        {
                            int numParen = 0;
                            for(std::size_t j = i + 2; i < ppTokenVec.size(); i++)
                            {
                                if(isMatched(ppTokenVec, j, PreprocessingToken::PUNCTUATOR, "("))
                                    numParen++;
                                else if(isMatched(ppTokenVec, j, PreprocessingToken::PUNCTUATOR, ")"))
                                {
                                    if(numParen > 0)
                                        numParen--;
                                    else if(numParen == 0)
                                    {
                                        endPos = j;
                                        argsVec.emplace_back(ppTokenVec.begin() + sepPos + 1,
                                                             ppTokenVec.begin() + j);
                                        break;
                                    }
                                }
                                else if(isMatched(ppTokenVec, j, PreprocessingToken::PUNCTUATOR, ",") &&
                                        numParen == 0)
                                {
                                    argsVec.emplace_back(ppTokenVec.begin() + sepPos + 1,
                                                         ppTokenVec.begin() + j);
                                    sepPos = j;
                                }
                            }
                        }
                        else
                            isValid = false;

                        // check number of arguments
                        std::cout << "num of arguments: "
                                  << macro.arguments.size() << std::endl;

                        // expand arguments
                        if(isValid)
                        {
                            for(auto&& e : argsVec)
                                replaceMacro(e);
                        }

                        if(isValid)
                        {
                        }
                        else
                        {
                            mIsValid = false;
                            std::cerr << "Macro Replacement error:\n"
                                      << "    what: function-like macor replacement is invalid.\n"
                                      << "    macro-name: " << macro.identifier
                                      << std::endl;
                        }
                    }
                }
            }
        }
    }
}

void PPDirectiveProcessor::convertPPTokens(PPTokens* ppTokens, std::vector<PreprocessingToken*>& vec)
{
    std::vector<PreprocessingToken*> inversedVec;
    for(PPTokens* p = ppTokens; p != nullptr;)
    {
        switch(p->ePPTokens)
        {
            case(PPTokens::PREPROCESSING_TOKEN):
                inversedVec.push_back(p->uPPTokens.sPreprocessingToken.preprocessingToken);
                p = nullptr;
                break;
            case(PPTokens::PP_TOKENS_PREPROCESSING_TOKEN):
                inversedVec.push_back(p->uPPTokens.sPPTokensPreprocessingToken.preprocessingToken);
                p = p->uPPTokens.sPPTokensPreprocessingToken.ppTokens;
                break;

            default:
                unexpectedEnumError("PPTokens", p->ePPTokens);
                break;
        }
    }
    
    for(auto iter = inversedVec.rbegin(); iter != inversedVec.rend(); iter++)
        vec.push_back(*iter);
}

void PPDirectiveProcessor::ctrlInclude(ControlLine* controlLine)
{
    std::vector<PreprocessingToken*> ppTokenVec;
    convertPPTokens(controlLine->uControlLine.sIncludePPTokensNewLine.ppTokens, ppTokenVec);

    replaceMacro(ppTokenVec);
    
    std::string src = "#include ";
    for(auto&& e : ppTokenVec)
        TOKEN::getString(e, src);
    

    Preprocessor pp;
    bool isValid = pp.retokenize(src);

    if(isValid &&
       pp.mPPTokens.size() == 3)
    {
        if(pp.mPPTokens[2].first->ePreprocessingToken != PreprocessingToken::HEADER_NAME)
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
    {
        Preprocessor::ESearch eSearch = Preprocessor::CURRENT_ONLY;
        std::string file;
        switch(pp.mPPTokens[2].first->uPreprocessingToken.sHeaderName.headerName->eHeaderName)
        {
            case(HeaderName::H_CHAR_SEQUENCE):
                eSearch = Preprocessor::SYSTEM_ONLY;
                TOKEN::getString(pp.mPPTokens[2].first->uPreprocessingToken.sHeaderName.headerName->uHeaderName.sHCharSequence.hCharSequence, file);
                break;
            case(HeaderName::Q_CHAR_SEQUENCE):
                eSearch = Preprocessor::CURRENT_AND_SYSTEM;
                TOKEN::getString(pp.mPPTokens[2].first->uPreprocessingToken.sHeaderName.headerName->uHeaderName.sQCharSequence.qCharSequence, file);
                break;
            
            default:
                unexpectedEnumError("HeaderName", pp.mPPTokens[2].first->ePreprocessingToken);
                break;
        }

        Preprocessor p;
        isValid = p.execute(file, mPP->mDir, eSearch);
        
        mPP->mProcessedPPTokens.insert(mPP->mProcessedPPTokens.end(),
                                       p.mProcessedPPTokens.begin(),
                                       p.mProcessedPPTokens.end());
    }

    if(!isValid)
    {
        mIsValid = false;
        std::cerr << "pp-directive-processor error:\n"
                  << "    what: include directive is invalid."
                  << std::endl;
    }
}

void PPDirectiveProcessor::ctrlDefine(ControlLine* controlLine)
{
    auto convertIdentifierList = [&](IdentifierList* list, std::vector<std::string>& vec)
    {
        std::vector<Identifier*> inversedList;
        for(IdentifierList* l = list; l != nullptr;)
        {
            switch(l->eIdentifierList)
            {
                case(IdentifierList::IDENTIFIER):
                    inversedList.push_back(l->uIdentifierList.sIdentifier.identifier);
                    l = nullptr;
                    break;
                case(IdentifierList::IDENTIFIER_LIST_IDENTIFIER):
                    inversedList.push_back(l->uIdentifierList.sIdentifierListIdentifier.identifier);
                    l = l->uIdentifierList.sIdentifierListIdentifier.identifierList;
                    break;

                default:
                    std::cerr << "implementation error:\n"
                              << "    what: unexpected enumeration.\n"
                              << "    class: IdentifierList\n"
                              << "    enum: " << l->eIdentifierList
                              << std::endl;
            }
        }

        vec.resize(inversedList.size());        
        for(std::size_t i = inversedList.size(); i > 0; i--)
            TOKEN::getString(inversedList[i - 1], vec[i - 1]);
    };

    Macro macro;
    
    switch(controlLine->eControlLine)
    {
        case(ControlLine::DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE):
            TOKEN::getString(controlLine->uControlLine.sDefineIdentifierReplacementListNewLine.identifier, macro.identifier);
            convertPPTokens(controlLine->uControlLine.sDefineIdentifierReplacementListNewLine.replacementList->ppTokens, macro.replacements);
            macro.isFunction = false;
            macro.isVariable = false;
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            TOKEN::getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifier, macro.identifier);
            convertIdentifierList(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifierList, macro.arguments);
            convertPPTokens(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.replacementList->ppTokens, macro.replacements);
            macro.isFunction = true;
            macro.isVariable = false;
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            TOKEN::getString(controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.identifier, macro.identifier);
            convertPPTokens(controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.replacementList->ppTokens, macro.replacements);
            macro.isFunction = true;
            macro.isVariable = true;
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            TOKEN::getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifier, macro.identifier);
            convertIdentifierList(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifierList, macro.arguments);
            convertPPTokens(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.replacementList->ppTokens, macro.replacements);
            macro.isFunction = true;
            macro.isVariable = true;
            break;
        
        default:
            break;
    }

    bool isDuplication = false;
    for(auto&& e : Macro::MACROS)
    {
        if(macro.identifier == e.identifier)
        {
            if(macro.isFunction == e.isFunction &&
               macro.isVariable == e.isVariable)
            {
                if(macro.arguments != e.arguments ||
                   macro.replacements != e.replacements)
                    isDuplication = true;
            }
            else
                isDuplication = true;

            break;
        }
    }

    if(isDuplication)
    {
        mIsValid = false;
        std::cerr << "PPDirectiveProcessor error:\n"
                  << "    what: duplication defined.\n"
                  << "    macro-name: " << macro.identifier
                  << std::endl;
    }
    else     
        Macro::MACROS.push_back(std::move(macro));
}

void PPDirectiveProcessor::ctrlUndef(ControlLine* controlLine)
{
    std::string identifier;
    TOKEN::getString(controlLine->uControlLine.sUndefIdentifierNewLine.identifier, identifier);

    for(std::size_t i = 0; i < Macro::MACROS.size(); i++)
    {
        if(identifier == Macro::MACROS[i].identifier)
        {
            Macro::MACROS.erase(Macro::MACROS.begin() + i);
            break;
        }
    }
}

bool PPDirectiveProcessor::evalAdditiveExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalMultiplicativeExpression(vec, idx, val))
    {
        while(isValid)
        {
            bool isPlus = true;
            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "+"))
                isPlus = true;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "-"))
                isPlus = false;
            else
                break;

            idx++;

            std::uintmax_t res = 0;
            if(evalMultiplicativeExpression(vec, idx, res))
            {
                if(isPlus)
                    val += res;
                else
                    val -= res;
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalANDExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalEqualityExpression(vec, idx, val))
    {
        while(isValid &&
              isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "&"))
        {
            idx++;

            std::uintmax_t res = 0;
            if(evalEqualityExpression(vec, idx, res))
                val &= res;
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalAssignmentExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    if(evalConditionalExpression(vec, idx, val))
        return true;
    else
    {
        idx = befIdx;
        val = befVal;
        
        return false;
    }
}

bool PPDirectiveProcessor::evalCastExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    if(evalUnaryExpression(vec, idx, val))
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalCharacterConstant(CharacterConstant* characterConstant, std::uintmax_t& val)
{
    if(characterConstant->eCharacterConstant != CharacterConstant::C_CHAR_SEQUENCE)
    {
        std::cerr << "implementation error:\n"
                  << "    what: prefix for character constant is not available."
                  << std::endl;
        return false;
    }

    std::cerr << "implementation error:\n"
              << "    what: character constant is not implemented."
              << std::endl;
    
    return false;
}

bool PPDirectiveProcessor::evalConditionalExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalLogicalORExpression(vec, idx, val))
    {
        if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "?"))
        {
            idx++;

            std::uintmax_t trueVal = 0, falseVal = 0;
            if(evalExpression(vec, idx, trueVal))
            {
                if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, ":"))
                {
                    idx++;

                    if(evalConditionalExpression(vec, idx, falseVal))
                        val = val ? trueVal : falseVal;
                    else
                        isValid = false;
                }
                else
                {
                    isValid = false;
                    std::cerr << "PPDirectiveProcessor error:\n"
                              << "    what: expression is incompleted.\n"
                              << "    expression: Conditional Expression"
                              << std::endl;
                }
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalConstant(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(isMatched(vec, idx, PreprocessingToken::PP_NUMBER))
        isValid = evalIntegerConstant(vec[idx++]->uPreprocessingToken.sPPNumber.ppNumber, val);
    else if(isMatched(vec, idx, PreprocessingToken::CHARACTER_CONSTANT))
        isValid = evalCharacterConstant(vec[idx++]->uPreprocessingToken.sCharacterConstant.characterConstant, val);
    else
    {
        isValid = false;
        std::cerr << "PPDirectiveProcessor error:\n"
                  << "    what: syntax error in if directive."
                  << std::endl;
    }

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalConstantExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    if(evalConditionalExpression(vec, idx, val))
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalDecimalConstant(const std::string& data, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(data[idx] >= '1' && data[idx] <= '9')
    {
        while(idx < data.size())
        {
            if(data[idx] >= '0' && data[idx] <= '9')
                val = val * 10 + (data[idx++] - '0');
            else
                break;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalEqualityExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalRelationalExpression(vec, idx, val))
    {
        while(isValid)
        {
            bool isEqual = true;

            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "=="))
                isEqual = true;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "!="))
                isEqual = false;
            else
                break;

            idx++;
            
            std::uintmax_t res = 0;
            if(evalRelationalExpression(vec, idx, res))
            {
                if(isEqual)
                    val = static_cast<std::uintmax_t>(val == res);
                else
                    val = static_cast<std::uintmax_t>(val != res);
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalExclusiveORExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalANDExpression(vec, idx, val))
    {
        while(isValid &&
              isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "^"))
        {
            idx++;

            std::uintmax_t res = 0;
            if(evalANDExpression(vec, idx, res))
                val ^= res;
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    if(evalAssignmentExpression(vec, idx, val))
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalHexadecimalConstant(const std::string& data, std::size_t& idx, std::uintmax_t& val)
{
    if(data.size() < idx + 3)
        return false;

    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(data.substr(idx, 2) == "0x" ||
       data.substr(idx, 2) == "0X")
    {
        idx += 2;

        if((data[idx] >= '0' && data[idx] <= '9') ||
           (data[idx] >= 'a' && data[idx] <= 'f') ||
           (data[idx] >= 'A' && data[idx] <= 'F'))
        {
            while(idx < data.size())
            {
                if(data[idx] >= '0' && data[idx] <= '9')
                    val = val * 16 + (data[idx++] - '0');
                else if(data[idx] >= 'a' && data[idx] <= 'f')
                    val = val * 16 + (data[idx++] - 'a');
                else if(data[idx] >= 'A' && data[idx] <= 'F')
                    val = val * 16 + (data[idx++] - 'A');
                else
                    break;
            }
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalInclusiveORExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalExclusiveORExpression(vec, idx, val))
    {
        while(isValid &&
              isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "|"))
        {
            idx++;

            std::uintmax_t res = 0;
            if(evalExclusiveORExpression(vec, idx, res))
                val |= res;
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalIntegerConstant(PPNumber* ppNumber, std::uintmax_t& val)
{
    auto befVal = val;

    std::string data;
    TOKEN::getString(ppNumber, data);
    std::size_t idx = 0;

    if(evalDecimalConstant(data, idx, val))
        return true;
    else if(evalOctalConstant(data, idx, val))
        return true;
    else if(evalHexadecimalConstant(data, idx, val))
        return true;
    else
    {
        val = befVal;
        return false;
    }

    evalIntegerSuffix(data, idx, val);

    if(idx != data.size())
        return true;
    else
        return false;
}

bool PPDirectiveProcessor::evalIntegerSuffix(const std::string& data, std::size_t& idx, std::uintmax_t& val)
{
    std::string sub = data.substr(idx, 1);
    if(sub == "u" || sub == "U")
    {
        idx++;
        sub = data.substr(idx, 2);
        if(sub == "ll" || sub == "LL")
            idx += 2;
        else
        {
            sub = data.substr(idx, 1);
            if(sub == "l" || sub == "L")
                idx++;
        }
    }
    else
    {
        sub = data.substr(idx, 2);
        if(sub == "ll" || sub == "LL")
        {
            idx += 2;
            sub = data.substr(idx, 1);
            if(sub == "u" || sub == "U")
                idx++;
        }
        else
        {
            sub = data.substr(idx, 1);
            if(sub == "l" || sub == "L")
            {
                idx++;
                sub = data.substr(idx, 1);
                if(sub == "u" || sub == "U")
                    idx++;
            }
        }
    }

    return true;
}

bool PPDirectiveProcessor::evalLogicalANDExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalInclusiveORExpression(vec, idx, val))
    {
        while(isValid &&
              isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "&&"))
        {
            idx++;

            std::uintmax_t res = 0;
            if(evalInclusiveORExpression(vec, idx, res))
                val = static_cast<std::uintmax_t>(val && res);
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalLogicalORExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalLogicalANDExpression(vec, idx, val))
    {
        while(isValid &&
              isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "||"))
        {
            idx++;

            std::uintmax_t res = 0;
            if(evalLogicalANDExpression(vec, idx, res))
                val = static_cast<std::uintmax_t>(val || res);
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalMultiplicativeExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isVald = true;

    if(evalCastExpression(vec, idx, val))
    {
        while(isVald)
        {
            int tag = 0;
            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "*"))
                tag = 1;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "/"))
                tag = 2;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "%"))
                tag = 3;
            else
                break;

            idx++;

            std::uintmax_t res = 0;
            if(evalCastExpression(vec, idx, res))
            {
                if(tag == 1)
                    val *= res;
                else if(tag == 2)
                    val /= res;
                else if(tag == 3)
                    val %= res;
            }
            else
                isVald = false;
        }
    }
    else
        isVald = false;

    if(isVald)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalOctalConstant(const std::string& data, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(data[idx] == '0')
    {
        while(idx < data.size())
        {
            if(data[idx] >= '0' && data[idx] <= '7')
                val = val * 8 + (data[idx++] - '0');
            else
                break;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalPrimaryExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "("))
    {
        idx++;

        if(evalExpression(vec, idx, val))
        {
            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, ")"))
                idx++;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(!evalConstant(vec, idx, val))
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalPostfixExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    if(evalPrimaryExpression(vec, idx, val))
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalRelationalExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalShiftExpression(vec, idx, val))
    {
        while(isValid)
        {
            bool isEqual = true, isLess = true;
            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "<"))
                isEqual = false, isLess = true;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, ">"))
                isEqual = false, isLess = false;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "<="))
                isEqual = true, isLess = true;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, ">="))
                isEqual = true, isLess = false;
            else
                break;

            idx++;

            std::uintmax_t res = 0;
            if(evalShiftExpression(vec, idx, res))
            {
                if((!isEqual) && isLess)
                    val = static_cast<std::uintmax_t>(val < res);
                else if((!isEqual) && (!isLess))
                    val = static_cast<std::uintmax_t>(val > res);
                else if(isEqual && isLess)
                    val = static_cast<std::uintmax_t>(val <= res);
                else if(isEqual && (!isLess))
                    val = static_cast<std::uintmax_t>(val >= res);
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalShiftExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    if(evalAdditiveExpression(vec, idx, val))
    {
        while(isValid)
        {
            bool isLeft = true;
            if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "<<"))
                isLeft = true;
            else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, ">>"))
                isLeft = false;
            else
                break;

            idx++;

            std::uintmax_t res = 0;
            if(evalAdditiveExpression(vec,idx, res))
            {
                if(isLeft)
                    val <<= res;
                else
                    val >>= res;
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::evalUnaryExpression(const std::vector<PreprocessingToken*>& vec, std::size_t& idx, std::uintmax_t& val)
{
    auto befIdx = idx;
    auto befVal = val;

    bool isValid = true;

    int tag = 0;
    if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "+"))
        tag = 1;
    else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "-"))
        tag = 2;
    else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "~"))
        tag = 3;
    else if(isMatched(vec, idx, PreprocessingToken::PUNCTUATOR, "!"))
        tag = 4;
    
    if(tag != 0)
    {
        idx++;

        std::uintmax_t res = 0;
        if(evalCastExpression(vec, idx, res))
        {
            if(tag == 1)
                val *= 1;
            else if(tag == 2)
                val *= -1;
            else if(tag == 3)
                val = ~val;
            else if(tag == 4)
                val = !static_cast<bool>(res);
        }
        else
            isValid = false;
    }
    else if(!evalPostfixExpression(vec, idx, val))
        isValid = false;

    if(isValid)
        return true;
    else
    {
        idx = befIdx;
        val = befVal;

        return false;
    }
}

bool PPDirectiveProcessor::isNonzero(PPTokens* ppTokens)
{
    std::vector<PreprocessingToken*> ppTokenVec;
    convertPPTokens(ppTokens, ppTokenVec);

    // process " defined identifier " and " defined ( identifier ) "
    for(std::size_t i = 0; i < ppTokenVec.size(); i++)
    {
        if(isMatched(ppTokenVec, i, PreprocessingToken::IDENTIFIER, "defined"))
        {
            char c = '0';
            int numDeleted = 0;

            // defined identifier
            if(isMatched(ppTokenVec, i + 1, PreprocessingToken::IDENTIFIER))
            {
                numDeleted = 2;
                if(isDefined(ppTokenVec[i + 1]->uPreprocessingToken.sIdentifier.identifier))
                    c = '1';
                else
                    c = '0';
            }
            // defined ( identifier )
            else if(isMatched(ppTokenVec, i + 1, PreprocessingToken::PUNCTUATOR, "(") &&
                    isMatched(ppTokenVec, i + 2, PreprocessingToken::IDENTIFIER) &&
                    isMatched(ppTokenVec, i + 3, PreprocessingToken::PUNCTUATOR, ")"))
            {
                numDeleted = 4;
                if(isDefined(ppTokenVec[i + 2]->uPreprocessingToken.sIdentifier.identifier))
                    c = '1';
                else
                    c = '0';
            }

            if(numDeleted > 0)
            {
                PreprocessingToken* tmp = new PreprocessingToken();
                tmp->ePreprocessingToken = PreprocessingToken::PP_NUMBER;
                tmp->uPreprocessingToken.sPPNumber.ppNumber = new PPNumber();
                tmp->uPreprocessingToken.sPPNumber.ppNumber->ePPNumber = PPNumber::DIGIT;
                tmp->uPreprocessingToken.sPPNumber.ppNumber->uPPNumber.sDigit.digit = new Digit();
                tmp->uPreprocessingToken.sPPNumber.ppNumber->uPPNumber.sDigit.digit->element = c;

                ppTokenVec.erase(ppTokenVec.begin() + i,
                                 ppTokenVec.begin() + i + numDeleted);
                ppTokenVec.insert(ppTokenVec.begin() + i,
                                  tmp);
            }
        }
    }

    // replace macro
    replaceMacro(ppTokenVec);   

    // remained identifier is converted to 0
    for(auto&& e : ppTokenVec)
    {
        if(e->ePreprocessingToken == PreprocessingToken::IDENTIFIER)
        {
            PreprocessingToken* tmp = new PreprocessingToken();
            tmp->ePreprocessingToken = PreprocessingToken::PP_NUMBER;
            tmp->uPreprocessingToken.sPPNumber.ppNumber = new PPNumber();
            tmp->uPreprocessingToken.sPPNumber.ppNumber->ePPNumber = PPNumber::DIGIT;
            tmp->uPreprocessingToken.sPPNumber.ppNumber->uPPNumber.sDigit.digit = new Digit();
            tmp->uPreprocessingToken.sPPNumber.ppNumber->uPPNumber.sDigit.digit->element = '0';
        
            e = tmp;
        }
    }

    // evaluate
    std::size_t idx = 0;
    std::uintmax_t value = 0;

    if(evalConstantExpression(ppTokenVec, idx, value))
    {
        if(idx != ppTokenVec.size())
        {
            mIsValid = false;
            std::cerr << "PPDirectiveProcessor error:\n"
                      << "    what: constant expression is not evaluated to the end."
                      << std::endl;
        }
    }
    else
    {
        mIsValid = false;
        std::cerr << "PPDirectiveProcessor error:\n"
                  << "    what: constant expression is not evaluated."
                  << std::endl;
    }

    return value != 0;
}

bool PPDirectiveProcessor::isDefined(Identifier* identifier)
{
    std::string data;
    TOKEN::getString(identifier, data);

    for(auto&& e : Macro::MACROS)
    {
        if(data == e.identifier)
            return true;
    }

    return false;
}

bool PPDirectiveProcessor::isMatched(const std::vector<PreprocessingToken*>& vec, std::size_t idx, PreprocessingToken::EPreprocessingToken tag) const
{
    if(idx < vec.size())
        return vec[idx]->ePreprocessingToken == tag;
    else
        return false;
}

bool PPDirectiveProcessor::isMatched(const std::vector<PreprocessingToken*>& vec, std::size_t idx, PreprocessingToken::EPreprocessingToken tag, std::string data) const
{
    if(isMatched(vec, idx, tag))
    {
        std::string d;
        TOKEN::getString(vec[idx], d);

        return d == data;
    }
    else
        return false;
}