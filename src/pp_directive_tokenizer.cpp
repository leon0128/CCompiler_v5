#include "pp_directive_tokenizer.hpp"
#include "preprocessor.hpp"
#include <iostream>
#include <utility>

PPDirectiveTokenizer::PPDirectiveTokenizer(Preprocessor* pp):
    mPP(pp),
    mIdx(0),
    mIsValid(true)
{
}

bool PPDirectiveTokenizer::execute()
{
    mPP->mPreprocessingFile = getPreprocessingFile();

    if(mIdx != mPP->mPPTokens.size())
        mIsValid = false;

    return mIsValid;
}

ControlLine* PPDirectiveTokenizer::getControlLine()
{
    ControlLine controlLine;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#"))
    {
        mIdx++;

        if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "include"))
        {
            mIdx++;

            if(controlLine.uControlLine.sIncludePPTokensNewLine.ppTokens = getPPTokens())
            {
                if(controlLine.uControlLine.sIncludePPTokensNewLine.newLine = getNewLine())
                    controlLine.eControlLine = ControlLine::INCLUDE_PP_TOKENS_NEW_LINE;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "define") &&
                isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER))
        {
            Identifier* identifier = mPP->mPPTokens[mIdx + 1].first->uPreprocessingToken.sIdentifier.identifier;
            mIdx += 2;

            if(controlLine.uControlLine.sDefineIdentifierReplacementListNewLine.replacementList = getReplacementList())
            {
                if(controlLine.uControlLine.sDefineIdentifierReplacementListNewLine.newLine = getNewLine())
                {
                    controlLine.uControlLine.sDefineIdentifierReplacementListNewLine.identifier = identifier;
                    controlLine.eControlLine = ControlLine::DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE;
                }
                else
                    isValid = false;
            }
            else
            {
                Lparen* lparen = getLparen();
                if(lparen)
                {
                    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "...") &&
                       isMatched(mIdx + 1, PreprocessingToken::PUNCTUATOR, ")"))
                    {
                        mIdx += 2;

                        if(controlLine.uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.replacementList = getReplacementList())
                        {
                            if(controlLine.uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.newLine = getNewLine())
                            {
                                controlLine.eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                                controlLine.uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.identifier = identifier;
                                controlLine.uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.lparen = lparen;
                            }
                            else
                                isValid = false;
                        }
                        else
                            isValid = false;
                    }
                    else
                    {
                        IdentifierList* identifierList = getIdentifierList();
                        if(identifierList &&
                           isMatched(mIdx, PreprocessingToken::PUNCTUATOR, ",") &&
                           isMatched(mIdx + 1, PreprocessingToken::PUNCTUATOR, "...") &&
                           isMatched(mIdx + 2, PreprocessingToken::PUNCTUATOR, ")"))
                        {
                            mIdx += 3;

                            if(controlLine.uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.replacementList = getReplacementList())
                            {
                                if(controlLine.uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.newLine = getNewLine())
                                {
                                    controlLine.eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.lparen = lparen;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifier = identifier;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifierList = identifierList;
                                }
                                else
                                    isValid = false;
                            }
                            else
                                isValid = false;
                        }
                        else if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, ")"))
                        {
                            mIdx++;

                            if(controlLine.uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.replacementList = getReplacementList())
                            {
                                if(controlLine.uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.newLine = getNewLine())
                                {
                                    controlLine.eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifier = identifier;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.lparen = lparen;
                                    controlLine.uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifierList;
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
            }
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "undef") &&
                isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER))
        {
            controlLine.uControlLine.sUndefIdentifierNewLine.identifier = mPP->mPPTokens[mIdx + 1].first->uPreprocessingToken.sIdentifier.identifier;
            mIdx += 2;

            if(controlLine.uControlLine.sUndefIdentifierNewLine.newLine = getNewLine())
                controlLine.eControlLine = ControlLine::UNDEF_IDENTIFIER_NEW_LINE;
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "line"))
        {
            mIdx++;

            if(controlLine.uControlLine.sLinePPTokensNewLine.ppTokens = getPPTokens())
            {
                if(controlLine.uControlLine.sLinePPTokensNewLine.newLine = getNewLine())
                    controlLine.eControlLine = ControlLine::LINE_PP_TOKENS_NEW_LINE;
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "error"))
        {
            mIdx++;

            controlLine.uControlLine.sErrorPPtokensNewLine.ppTokens = getPPTokens();
            if(controlLine.uControlLine.sErrorPPtokensNewLine.newLine = getNewLine())
                controlLine.eControlLine = ControlLine::ERROR_PP_TOKENS_NEW_LINE;
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "pragma"))
        {
            mIdx++;

            controlLine.uControlLine.sPragmaPPTokensNewLine.ppTokens = getPPTokens();
            if(controlLine.uControlLine.sPragmaPPTokensNewLine.newLine = getNewLine())
                controlLine.eControlLine = ControlLine::PRAGMA_PP_TOKENS_NEW_LINE;
            else
                isValid = false;
        }
        else if(controlLine.uControlLine.sNewLine.newLine = getNewLine())
            controlLine.eControlLine = ControlLine::NEW_LINE;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new ControlLine(controlLine);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ElifGroup* PPDirectiveTokenizer::getElifGroup()
{
    ElifGroup elifGroup;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#") &&
       isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER, "elif"))
    {
        mIdx += 2;

        if(elifGroup.ppTokens = getPPTokens())
        {
            if(elifGroup.newLine = getNewLine())
                elifGroup.group = getGroup();
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new ElifGroup(elifGroup);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ElifGroups* PPDirectiveTokenizer::getElifGroups(ElifGroups* bef)
{
    ElifGroups elifGroups;
    bool isValid = true;

    if(!bef)
    {
        if(elifGroups.uElifGroups.sElifGroup.elifGroup = getElifGroup())
            elifGroups.eElifGroups = ElifGroups::ELIF_GROUP;
        else
            isValid = false;
    }
    else
    {
        if(elifGroups.uElifGroups.sElifGroupsElifGroup.elifGroup = getElifGroup())
        {
            elifGroups.uElifGroups.sElifGroupsElifGroup.elifGroups = bef;
            elifGroups.eElifGroups = ElifGroups::ELIF_GROUPS_ELIF_GROUP;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        ElifGroups* retVal = new ElifGroups(elifGroups);
        ElifGroups* aft = getElifGroups(retVal);
        if(aft)
            return aft;
        else
            retVal;
    }
    else
        return nullptr;
}

ElseGroup* PPDirectiveTokenizer::getElseGroup()
{
    ElseGroup elseGroup;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#") &&
       isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER, "else"))
    {
        mIdx += 2;

        if(elseGroup.newLine = getNewLine())
            elseGroup.group = getGroup();
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new ElseGroup(elseGroup);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

EndifLine* PPDirectiveTokenizer::getEndifLine()
{
    EndifLine endifLine;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#") &&
       isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER, "endif"))
    {
        mIdx += 2;

        if(endifLine.newLine = getNewLine())
            ;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new EndifLine(endifLine);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Group* PPDirectiveTokenizer::getGroup(Group* bef)
{
    Group group;
    auto isValid = true;

    if(!bef)
    {
        if(group.uGroup.sGroupPart.groupPart = getGroupPart())
            group.eGroup = Group::GROUP_PART;
        else
            isValid = false;
    }
    else
    {
        if(group.uGroup.sGroupGroupPart.groupPart = getGroupPart())
        {
            group.uGroup.sGroupGroupPart.group = bef;
            group.eGroup = Group::GROUP_GROUP_PART;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        Group* retVal = new Group(group);
        Group* aft = getGroup(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

GroupPart* PPDirectiveTokenizer::getGroupPart()
{
    GroupPart groupPart;
    bool isValid = true;
    auto idx = mIdx;

    if(groupPart.uGroupPart.sIfSection.ifSection = getIfSection())
        groupPart.eGroupPart = GroupPart::IF_SECTION;
    else if(groupPart.uGroupPart.sControlLine.controlLine = getControlLine())
        groupPart.eGroupPart = GroupPart::CONTROL_LINE;
    else if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#"))
    {
        mIdx++;

        if(groupPart.uGroupPart.sNonDirective.nonDirective = getNonDirective())
            groupPart.eGroupPart = GroupPart::NON_DIRECTIVE;
        else
            isValid = false;
    }
    else if(groupPart.uGroupPart.sTextLine.textLine = getTextLine())
        groupPart.eGroupPart = GroupPart::TEXT_LINE;
    else
        isValid = false;

    
    if(isValid)
        return new GroupPart(groupPart);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IdentifierList* PPDirectiveTokenizer::getIdentifierList(IdentifierList* bef)
{
    IdentifierList identifierList;
    bool isValid = true;
    auto idx = mIdx;

    if(!bef)
    {
        if(isMatched(mIdx, PreprocessingToken::IDENTIFIER))
        {
            identifierList.eIdentifierList = IdentifierList::IDENTIFIER;
            identifierList.uIdentifierList.sIdentifier.identifier = mPP->mPPTokens[mIdx].first->uPreprocessingToken.sIdentifier.identifier;
            
            mIdx++;
        }
        else
            isValid = false;
    }
    else
    {
        if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, ",") &&
           isMatched(mIdx + 1, PreprocessingToken::IDENTIFIER))
        {
            identifierList.eIdentifierList = IdentifierList::IDENTIFIER_LIST_IDENTIFIER;
            identifierList.uIdentifierList.sIdentifierListIdentifier.identifierList = bef;
            identifierList.uIdentifierList.sIdentifierListIdentifier.identifier = mPP->mPPTokens[mIdx + 1].first->uPreprocessingToken.sIdentifier.identifier;

            mIdx += 2;
        }
        else
            isValid = false;
    }

    if(isValid)
        return new IdentifierList(identifierList);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IfGroup* PPDirectiveTokenizer::getIfGroup()
{
    IfGroup ifGroup;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#"))
    {
        mIdx++;

        if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "if"))
        {
            mIdx++;

            if(ifGroup.uIfGroup.sIfPPTokensNewLineGroup.ppTokens = getPPTokens())
            {
                if(ifGroup.uIfGroup.sIfPPTokensNewLineGroup.newLine = getNewLine())
                {
                    ifGroup.uIfGroup.sIfPPTokensNewLineGroup.group = getGroup();
                    ifGroup.eIfGroup = IfGroup::IF_PP_TOKENS_NEW_LINE_GROUP;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "ifdef"))
        {
            mIdx++;

            if(isMatched(mIdx, PreprocessingToken::IDENTIFIER))
            {
                ifGroup.uIfGroup.sIfdefIdentifierNewLineGroup.identifier = mPP->mPPTokens[mIdx].first->uPreprocessingToken.sIdentifier.identifier;
                mIdx++;
                
                if(ifGroup.uIfGroup.sIfdefIdentifierNewLineGroup.newLine = getNewLine())
                {
                    ifGroup.uIfGroup.sIfdefIdentifierNewLineGroup.group = getGroup();
                    ifGroup.eIfGroup = IfGroup::IFDEF_IDENTIFIER_NEW_LINE_GROUP;
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "ifndef"))
        {
            mIdx++;

            if(isMatched(mIdx, PreprocessingToken::IDENTIFIER))
            {
                ifGroup.uIfGroup.sIfndefIdentifierNewLineGroup.identifier = mPP->mPPTokens[mIdx].first->uPreprocessingToken.sIdentifier.identifier;
                mIdx++;

                if(ifGroup.uIfGroup.sIfndefIdentifierNewLineGroup.newLine = getNewLine())
                {
                    ifGroup.uIfGroup.sIfndefIdentifierNewLineGroup.group = getGroup();
                    ifGroup.eIfGroup = IfGroup::IFNDEF_IDENTIFIER_NEW_LINE_GROUP;
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
        return new IfGroup(ifGroup);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IfSection* PPDirectiveTokenizer::getIfSection()
{
    IfSection ifSection;
    bool isValid = true;
    auto idx = mIdx;

    if(ifSection.ifGroup = getIfGroup())
    {
        ifSection.elifGroups = getElifGroups();
        ifSection.elseGroup = getElseGroup();

        if(ifSection.endifLine = getEndifLine())
            ;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new IfSection(ifSection);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Lparen* PPDirectiveTokenizer::getLparen()
{
    if(mIdx == 0)
        return nullptr;

    bool isValid = true;

    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "("))
    {
        std::string data;
        TOKEN::getString(mPP->mPPTokens[mIdx - 1].first, data);
        if(mPP->mPPTokens[mIdx - 1].second + data.size() == mPP->mPPTokens[mIdx].second)
            mIdx++;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
    {
        Lparen* lparen = new Lparen();
        lparen->element = '(';
        return lparen;
    }
    else
        return nullptr;
}

NewLine* PPDirectiveTokenizer::getNewLine()
{
    bool isValid = true;

    if(isMatched(mIdx, PreprocessingToken::OTHER, "\n"))
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        NewLine* newLine = new NewLine();
        newLine->element = '\n';
        return newLine;
    }
    else
        return nullptr;
}

NonDirective* PPDirectiveTokenizer::getNonDirective()
{
    if(isMatched(mIdx, PreprocessingToken::IDENTIFIER, "if") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "ifdef") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "ifndef") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "elif") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "else") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "endif") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "include") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "define") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "undef") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "line") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "error") ||
       isMatched(mIdx, PreprocessingToken::IDENTIFIER, "pragma"))
        return nullptr;

    NonDirective nonDirective;
    bool isValid = true;
    auto idx = mIdx;

    if(nonDirective.ppTokens = getPPTokens())
    {
        if(nonDirective.newLine = getNewLine())
            ;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new NonDirective(nonDirective);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

PPTokens* PPDirectiveTokenizer::getPPTokens(PPTokens* bef)
{
    if(mIdx >= mPP->mPPTokens.size())
        return nullptr;

    PPTokens ppTokens;
    bool isValid = true;

    if(!bef)
    {
        if(!isMatched(mIdx, PreprocessingToken::OTHER, "\n"))
        {
            ppTokens.ePPTokens = PPTokens::PREPROCESSING_TOKEN;
            ppTokens.uPPTokens.sPreprocessingToken.preprocessingToken = mPP->mPPTokens[mIdx].first;
            mIdx++;
        }
        else
            isValid = false;
    }
    else
    {
        if(!isMatched(mIdx, PreprocessingToken::OTHER, "\n"))
        {
            ppTokens.ePPTokens = PPTokens::PP_TOKENS_PREPROCESSING_TOKEN;
            ppTokens.uPPTokens.sPPTokensPreprocessingToken.ppTokens = bef;
            ppTokens.uPPTokens.sPPTokensPreprocessingToken.preprocessingToken = mPP->mPPTokens[mIdx].first;
            mIdx++;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        PPTokens* retVal = new PPTokens(ppTokens);
        PPTokens* aft = getPPTokens(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

PreprocessingFile* PPDirectiveTokenizer::getPreprocessingFile()
{
    PreprocessingFile* preprocessingFile = new PreprocessingFile();
    preprocessingFile->group = getGroup();
    return preprocessingFile;
}

ReplacementList* PPDirectiveTokenizer::getReplacementList()
{
    ReplacementList* replacementList = new ReplacementList();
    replacementList->ppTokens = getPPTokens();
    return replacementList;
}

TextLine* PPDirectiveTokenizer::getTextLine()
{
    if(isMatched(mIdx, PreprocessingToken::PUNCTUATOR, "#"))
        return nullptr;

    TextLine textLine;
    bool isValid = true;
    auto idx = mIdx;

    textLine.ppTokens = getPPTokens();
    if(textLine.newLine = getNewLine())
        ;
    else
        isValid = false;

    if(isValid)
        return new TextLine(textLine);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

bool PPDirectiveTokenizer::isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken ePreprocessingToken, const std::string& data) const
{
    if(idx >= mPP->mPPTokens.size())
        return false;

    if(ePreprocessingToken != mPP->mPPTokens[idx].first->ePreprocessingToken)
        return false;

    std::string str;
    TOKEN::getString(mPP->mPPTokens[idx].first, str);
    if(str == data)
        return true;
    else
        return false;
}

bool PPDirectiveTokenizer::isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken ePreprocessingToken, std::string&& data) const
{
    if(idx >= mPP->mPPTokens.size())
        return false;
    
    if(ePreprocessingToken != mPP->mPPTokens[idx].first->ePreprocessingToken)
        return false;

    std::string str;
    TOKEN::getString(mPP->mPPTokens[idx].first, str);
    if(str == std::forward<std::string>(data))
        return true;
    else
        return false;
}

bool PPDirectiveTokenizer::isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken ePreprocessingToken) const
{
    if(idx >= mPP->mPPTokens.size())
        return false;
    
    if(ePreprocessingToken == mPP->mPPTokens[idx].first->ePreprocessingToken)
        return true;
    else
        return false;
}

void PPDirectiveTokenizer::error(const char* message) const
{
    std::cerr << "pp-directove-tokenizer error:\n    "
              << message
              << std::endl;
}