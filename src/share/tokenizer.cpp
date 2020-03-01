#include "tokenizer.hpp"
#include "preprocessing_token.hpp"
#include "symbol.hpp"
#include <array>
#include <utility>
#include <string>
#include <iostream>

Tokenizer::Tokenizer(const std::string& src,
                     std::vector<PreprocessingToken*>& ppTokens):
    mSrc(src),
    mIdx(0),
    mPpTokens(ppTokens)
{
}

void Tokenizer::execute()
{
    // tokenize to pp-token from source
    while(mIdx < mSrc.size())
    {
        deleteComment();

        if(mSrc[mIdx] != ' ' &&
           mSrc[mIdx] != '\t' &&
           mSrc[mIdx] != '\v' &&
           mSrc[mIdx] != '\f')
        {
            auto idx = mIdx;
            PreprocessingToken_Symbol* symbol = conPreprocessingToken_Symbol();
            addPreprocessingToken(symbol, idx);
        }
        else
            mIdx++;
    }

    // tokenize to pp-file to pp-token
    mIdx = 0;
    mPreprocessingFile = conPreprocessingFile();
}

void Tokenizer::deleteComment()
{
    // // comment
    if(mSrc[mIdx] == '/' &&
       mSrc[mIdx + 1] == '/' &&
       mIdx + 1 < mSrc.size())
    {
        mIdx += 2;
        while(mIdx < mSrc.size())
        {
            if(mSrc[mIdx] == '\n')
                break;
            else
                mIdx++;
        }
    }
    // /* */ comment
    else if(mSrc[mIdx] == '/' &&
            mSrc[mIdx + 1] == '*' &&
            mIdx + 1 < mSrc.size())
    {
        auto idx = mIdx;
        bool isValid = false;

        mIdx += 2;

        while(mIdx + 1 < mSrc.size())
        {
            if(mSrc[mIdx] == '*' &&
               mSrc[mIdx + 1] == '/')
            {
                mIdx += 2;

                isValid = true;
                break;
            }
            else
                mIdx++;
        }

        if(!isValid)
            mIdx = idx;
    }
}

void Tokenizer::addPreprocessingToken(PreprocessingToken_Symbol* symbol, std::string::size_type idx)
{
    std::string data;
    process(symbol, data);

    PreprocessingToken* token = nullptr;

    switch(symbol->ePreprocessingToken)
    {
        case(PreprocessingToken_Symbol::HEADER_NAME):
            token = new PreprocessingToken(data, PreprocessingToken::HEADER_NAME, idx);
            break;
        case(PreprocessingToken_Symbol::IDENTIFIER):
            token = new PreprocessingToken(data, PreprocessingToken::IDENTIFIER, idx);
            break;
        case(PreprocessingToken_Symbol::PP_NUMBER):
            token = new PreprocessingToken(data, PreprocessingToken::PP_NUMBER, idx);
            break;
        case(PreprocessingToken_Symbol::CHARACTER_CONSTANT):
            token = new PreprocessingToken(data, PreprocessingToken::CHARACTER_CONSTANT, idx);
            break;
        case(PreprocessingToken_Symbol::STRING_LITERAL):
            token = new PreprocessingToken(data, PreprocessingToken::STRING_LITERAL, idx);
            break;
        case(PreprocessingToken_Symbol::PUNCTUATOR):
            token = new PreprocessingToken(data, PreprocessingToken::PUNCTUATOR, idx);
            break;
        case(PreprocessingToken_Symbol::OTHER):
            token = new PreprocessingToken(data, PreprocessingToken::OTHER, idx);
            break;
        
        case(PreprocessingToken_Symbol::NONE):
            processError("PreprocessingToken_Symbol", data);
            break;
    }

    token->symbol = symbol;
    token->push();
    mPpTokens.push_back(token);
}

PreprocessingFile* Tokenizer::conPreprocessingFile()
{
    PreprocessingFile* preprocessingFile = new PreprocessingFile();
    preprocessingFile->group = conGroup();
    return preprocessingFile;
}

PreprocessingToken_Symbol* Tokenizer::conPreprocessingToken_Symbol()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    PreprocessingToken_Symbol* preprocessingToken_symbol = nullptr;

    HeaderName* headerName = conHeaderName();
    if(headerName)
    {
        preprocessingToken_symbol = new PreprocessingToken_Symbol();
        preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::HEADER_NAME;
        preprocessingToken_symbol->uPreprocessingToken.sHeaderName = {headerName};
    }
    else
    {
        CharacterConstant* characterConstant = conCharacterConstant();
        if(characterConstant)
        {
            preprocessingToken_symbol = new PreprocessingToken_Symbol();
            preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::CHARACTER_CONSTANT;
            preprocessingToken_symbol->uPreprocessingToken.sCharacterConstant = {characterConstant};
        }
        else
        {
            StringLiteral* stringLiteral = conStringLiteral();
            if(stringLiteral)
            {
                preprocessingToken_symbol = new PreprocessingToken_Symbol();
                preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::STRING_LITERAL;
                preprocessingToken_symbol->uPreprocessingToken.sStringLiteral = {stringLiteral};
            }
            else
            {
                PpNumber* ppNumber = conPpNumber();
                if(ppNumber)
                {
                    preprocessingToken_symbol = new PreprocessingToken_Symbol();
                    preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::PP_NUMBER;
                    preprocessingToken_symbol->uPreprocessingToken.sPpNumber = {ppNumber};
                }
                else
                {
                    Identifier* identifier = conIdentifier();
                    if(identifier)
                    {
                        preprocessingToken_symbol = new PreprocessingToken_Symbol();
                        preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::IDENTIFIER;
                        preprocessingToken_symbol->uPreprocessingToken.sIdentifier = {identifier};
                    }
                    else
                    {
                        Punctuator* punctuator = conPunctuator();
                        if(punctuator)
                        {
                            preprocessingToken_symbol = new PreprocessingToken_Symbol();
                            preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::PUNCTUATOR;
                            preprocessingToken_symbol->uPreprocessingToken.sPunctuator = {punctuator};
                        }
                        else
                        {
                            Other* other = conOther();
                            if(other)
                            {
                                preprocessingToken_symbol = new PreprocessingToken_Symbol();
                                preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::OTHER;
                                preprocessingToken_symbol->uPreprocessingToken.sOther = {other};
                            }
                            else
                                isValid = false;
                        }
                    }
                }
            }
        }
    }

    if(isValid)
        return preprocessingToken_symbol;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

CChar* Tokenizer::conCChar()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    CChar* cChar = nullptr;

    char c = mSrc.at(mIdx);
    if(c != '\'' &&
       c != '\\' &&
       c != '\n')
    {
        mIdx++;

        cChar = new CChar();
        cChar->eCChar = CChar::ANY_MEMBER;
        cChar->uCChar.sAnyMember = {c};
    }
    else
    {
        EscapeSequence* escapeSequence = conEscapeSequence();
        if(escapeSequence)
        {
            cChar = new CChar();
            cChar->eCChar = CChar::ESCAPE_CEQUENCE;
            cChar->uCChar.sEscapeSequence = {escapeSequence};
        }
        else
            isValid = false;
    }

    if(isValid)
        return cChar;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

CCharSequence* Tokenizer::conCCharSequence(CCharSequence* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    CCharSequence* cCharSequence = nullptr;

    CChar* cChar = conCChar();
    if(cChar)
    {
        cCharSequence = new CCharSequence();
        if(!bef)
        {
            cCharSequence->eCCharSequence = CCharSequence::C_CHAR;
            cCharSequence->uCCharSequence.sCChar = {cChar};
        }
        else
        {
            cCharSequence->eCCharSequence = CCharSequence::C_CHAR_SEQUENCE_C_CHAR;
            cCharSequence->uCCharSequence.sCCharSequenceCChar = {bef, cChar};
        }
    }
    else
        isValid = false;

    if(isValid)
    {
        CCharSequence* aft = conCCharSequence(cCharSequence);
        if(aft)
            return aft;
        else
            return cCharSequence;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

CharacterConstant* Tokenizer::conCharacterConstant()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    CharacterConstant* characterConstant = nullptr;

    CharacterConstant::ECharacterConstant eCharacterConstant = CharacterConstant::C_CHAR_SEQUENCE;
    char c = mSrc.at(mIdx);
    mIdx++;
    if(c == 'L')
        eCharacterConstant = CharacterConstant::L_C_CHAR_SEQUENCE;
    else if(c == 'u')
        eCharacterConstant = CharacterConstant::u_C_CHAR_SEQUENCE;
    else if(c == 'U')
        eCharacterConstant = CharacterConstant::U_C_CHAR_SEQUENCE;
    else
        mIdx--;
    
    if(mSrc[mIdx] == '\'' &&
       mIdx < mSrc.size())
    {
        mIdx++;
        CCharSequence* cCharSequence = conCCharSequence();
        if(cCharSequence)
        {
            if(mSrc[mIdx] == '\'' &&
               mIdx < mSrc.size())
            {
                mIdx++;

                characterConstant = new CharacterConstant();
                characterConstant->eCharacterConstant = eCharacterConstant;
                switch(eCharacterConstant)
                {
                    case(CharacterConstant::C_CHAR_SEQUENCE):
                        characterConstant->uCharacterConstant.sCCharSequence = {cCharSequence};
                        break;
                    case(CharacterConstant::L_C_CHAR_SEQUENCE):
                        characterConstant->uCharacterConstant.s_L_CCharSequence = {cCharSequence};
                        break;
                    case(CharacterConstant::u_C_CHAR_SEQUENCE):
                        characterConstant->uCharacterConstant.s_u_CCharSequence = {cCharSequence};
                        break;
                    case(CharacterConstant::U_C_CHAR_SEQUENCE):
                        characterConstant->uCharacterConstant.s_U_CCharSequence = {cCharSequence};
                    
                    case(CharacterConstant::NONE):
                        break;
                }
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
        return characterConstant;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ConditionalExpression* Tokenizer::conConditionalExpression()
{
    return nullptr;
}

ConstantExpression* Tokenizer::conConstantExpression()
{
    auto idx = mIdx;
    bool isValid = true;
    ConstantExpression* constantExpression = nullptr;

    PpTokens* ppTokens = conPpTokens();

    if(ppTokens)
    {
        constantExpression = new ConstantExpression();
    }
    else
        isValid = false;

    if(isValid)
        return constantExpression;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ControlLine* Tokenizer::conControlLine()
{
    auto idx = mIdx;
    bool isValid = true;
    ControlLine* controlLine = nullptr;

    std::cout << "idx: " << mIdx << std::endl;

    if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)))
    {
        mIdx++;

        if(isEquality(mIdx, PreprocessingToken("include", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PpTokens* ppTokens = conPpTokens();
            if(ppTokens)
            {
                NewLine* newLine = conNewLine();
                if(newLine)
                {
                    controlLine = new ControlLine();
                    controlLine->eControlLine = ControlLine::INCLUDE_PP_TOKENS_NEW_LINE;
                    controlLine->uControlLine.sIncludePpTokensNewLine = {ppTokens, newLine};
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("define", PreprocessingToken::IDENTIFIER)) &&
                isEquality(mIdx + 1, PreprocessingToken::IDENTIFIER))
        {
            PreprocessingToken* identifier = mPpTokens[mIdx + 1];

            mIdx += 2;

            Lparen* lparen = conLparen();
            if(lparen)
            {
                if(isEquality(mIdx, PreprocessingToken("...", PreprocessingToken::PUNCTUATOR)) &&
                   isEquality(mIdx + 1, PreprocessingToken(")", PreprocessingToken::PUNCTUATOR)))
                {
                    mIdx += 2;

                    ReplacementList* replacementList = conReplacementList();
                    if(replacementList)
                    {
                        NewLine* newLine = conNewLine();
                        if(newLine)
                        {
                            controlLine = new ControlLine();
                            controlLine->eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                            controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine = {identifier, lparen, replacementList, newLine};
                        }
                        else
                            isValid = false;
                    }
                    else
                        isValid =false;
                }
                else
                {
                    IdentifierList* identifierList = conIdentifierList();
                    if(identifierList &&
                       isEquality(mIdx, PreprocessingToken(",", PreprocessingToken::PUNCTUATOR)) &&
                       isEquality(mIdx + 1, PreprocessingToken("...", PreprocessingToken::PUNCTUATOR)) &&
                       isEquality(mIdx + 2, PreprocessingToken(")", PreprocessingToken::PUNCTUATOR)))
                    {
                        mIdx += 3;

                        ReplacementList* replacementList = conReplacementList();
                        if(replacementList)
                        {
                            NewLine* newLine = conNewLine();
                            if(newLine)
                            {
                                controlLine = new ControlLine();
                                controlLine->eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                                controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine = {identifier, lparen, identifierList, replacementList, newLine};
                            }
                            else
                                isValid = false;
                        }
                        else
                            isValid = false;
                    }
                    else if(isEquality(mIdx, PreprocessingToken(")", PreprocessingToken::PUNCTUATOR)))
                    {
                        mIdx++;

                        ReplacementList* replacementList = conReplacementList();
                        if(replacementList)
                        {
                            NewLine* newLine = conNewLine();
                            if(newLine)
                            {
                                controlLine = new ControlLine();
                                controlLine->eControlLine = ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE;
                                controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine = {identifier, lparen, identifierList, replacementList, newLine};
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
            {
                ReplacementList* replacementList = conReplacementList();
                if(replacementList)
                {
                    NewLine* newLine = conNewLine();
                    if(newLine)
                    {
                        controlLine = new ControlLine();
                        controlLine->eControlLine = ControlLine::DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE;
                        controlLine->uControlLine.sDefineIdentifierReplacementListNewLine = {identifier, replacementList, newLine};
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
        }
        else if(isEquality(mIdx, PreprocessingToken("undef", PreprocessingToken::IDENTIFIER)) &&
                isEquality(mIdx + 1, PreprocessingToken::IDENTIFIER))
        {
            PreprocessingToken* identifier = mPpTokens[mIdx + 1];

            mIdx += 2;

            NewLine* newLine = conNewLine();
            if(newLine)
            {
                controlLine = new ControlLine();
                controlLine->eControlLine = ControlLine::UNDEF_IDENTIFIER_NEW_LINE;
                controlLine->uControlLine.sUndefIdentifierNewLine = {identifier, newLine};
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("line", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PpTokens* ppTokens = conPpTokens();
            if(ppTokens)
            {
                NewLine* newLine = conNewLine();
                if(newLine)
                {
                    controlLine = new ControlLine();
                    controlLine->eControlLine = ControlLine::LINE_PP_TOKENS_NEW_LINE;
                    controlLine->uControlLine.sLinePpTokensNewLine = {ppTokens, newLine};
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("error", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PpTokens* ppTokens = conPpTokens();
            NewLine* newLine = conNewLine();
            if(newLine)
            {
                controlLine = new ControlLine();
                controlLine->eControlLine = ControlLine::ERROR_PP_TOKENS_NEW_LINE;
                controlLine->uControlLine.sErrorPptokensNewLine = {ppTokens, newLine};
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("line", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PpTokens* ppTokens = conPpTokens();
            NewLine* newLine = conNewLine();
            if(newLine)
            {
                controlLine = new ControlLine();
                controlLine->eControlLine = ControlLine::PRAGMA_PP_TOKENS_NEW_LINE;
                controlLine->uControlLine.sPragmaPpTokensNewLine = {ppTokens, newLine};
            }
            else
                isValid = false;
        }
        else
        {
            NewLine* newLine = conNewLine();
            if(newLine)
            {
                controlLine = new ControlLine();
                controlLine->eControlLine = ControlLine::NEW_LINE;
                controlLine->uControlLine.sNewLine = {newLine};
            }
            else
                isValid = false;
        }
    }
    else
        isValid = false;

    if(isValid)
        return controlLine;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Digit* Tokenizer::conDigit()
{
    if(mIdx >= mSrc.size())
        return nullptr;
    
    char c = mSrc.at(mIdx);
    if(c < '0' || c > '9')
        return nullptr;

    Digit* digit = new Digit();
    digit->element = c;
    
    mIdx++;
    return digit;
}

ElifGroup* Tokenizer::conElifGroup()
{
    auto idx = mIdx;
    bool isValid = true;
    ElifGroup* elifGroup = nullptr;

    if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)) &&
       isEquality(mIdx + 1, PreprocessingToken("elif", PreprocessingToken::IDENTIFIER)))
    {
        mIdx += 2;

        ConstantExpression* constantExpression = conConstantExpression();
        if(constantExpression)
        {
            NewLine* newLine = conNewLine();
            if(newLine)
            {
                Group* group = conGroup();
                
                elifGroup = new ElifGroup();
                elifGroup->constantExpression = constantExpression;
                elifGroup->newLine = newLine;
                elifGroup->group = group;
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
        return elifGroup;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ElifGroups* Tokenizer::conElifGroups(ElifGroups* bef)
{
    auto idx = mIdx;
    bool isValid = true;
    ElifGroups* elifGroups = nullptr;

    if(!bef)
    {
        ElifGroup* elifGroup = conElifGroup();
        if(elifGroup)
        {
            elifGroups = new ElifGroups();
            elifGroups->eElifGroups = ElifGroups::ELIF_GROUP;
            elifGroups->uElifGroups.sElifGroup = {elifGroup};
        }
        else
            isValid = false;
    }
    else
    {
        ElifGroup* elifGroup = conElifGroup();
        if(elifGroup)
        {
            elifGroups = new ElifGroups();
            elifGroups->eElifGroups = ElifGroups::ELIF_GROUPS_ELIF_GROUP;
            elifGroups->uElifGroups.sElifGroupsElifGroup = {bef, elifGroup};
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        ElifGroups* aft = conElifGroups(elifGroups);
        if(aft)
            return aft;
        else
            return elifGroups;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ElseGroup* Tokenizer::conElseGroup()
{
    auto idx = mIdx;
    bool isValid = true;
    ElseGroup* elseGroup = nullptr;

    if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)) &&
       isEquality(mIdx + 1, PreprocessingToken("else", PreprocessingToken::IDENTIFIER)))
    {
        mIdx += 2;

        NewLine* newLine = conNewLine();
        if(newLine)
        {
            Group* group = conGroup();

            elseGroup = new ElseGroup();
            elseGroup->newLine = newLine;
            elseGroup->group = group;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return elseGroup;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

EncodingPrefix* Tokenizer::conEncodingPrefix()
{
    if(mIdx >= mSrc.size())
        return nullptr;
    
    auto idx = mIdx;
    bool isValid = true;
    std::string element;

    char c = mSrc.at(mIdx);
    if(c == 'u')
    {
        mIdx++;

        element.push_back(c);
        
        if(mSrc[mIdx] == '8' &&
           mIdx < mSrc.size())
        {
            mIdx++;
            element.push_back('8');
        }    
    }
    else if(c == 'U' ||
            c == 'L')
    {
        mIdx++;
        element.push_back(c);
    }
    else
        isValid = false;

    if(isValid)
    {
        EncodingPrefix* encodingPrefix = new EncodingPrefix();
        encodingPrefix->element = std::move(element);
        return encodingPrefix;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

EndifLine* Tokenizer::conEndifLine()
{
    auto idx = mIdx;
    bool isValid = true;
    EndifLine* endifLine = nullptr;

    if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)) &&
       isEquality(mIdx + 1, PreprocessingToken("endif", PreprocessingToken::IDENTIFIER)))
    {
        mIdx += 2;

        NewLine* newLine = conNewLine();
        if(newLine)
        {
            endifLine = new EndifLine();
            endifLine->newLine = newLine;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return endifLine;
    else
    {
        mIdx = idx;
        return nullptr;
    }
} 

EscapeSequence* Tokenizer::conEscapeSequence()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    EscapeSequence* escapeSequence = nullptr;

    SimpleEscapeSequence* simpleEscapeSequence = conSimpleEscapeSequence();
    if(simpleEscapeSequence)
    {
        escapeSequence = new EscapeSequence();
        escapeSequence->eEscapeSequence = EscapeSequence::SIMPLE_ESCAPE_SEQUENCE;
        escapeSequence->uEscapeSequence.sSimpleEscapeSequence = {simpleEscapeSequence};
    }
    else
    {
        OctalEscapeSequence* octalEscapeSequence = conOctalEscapeSequence();
        if(octalEscapeSequence)
        {
            escapeSequence = new EscapeSequence();
            escapeSequence->eEscapeSequence = EscapeSequence::OCTAL_ESCAPE_SEQUENCE;
            escapeSequence->uEscapeSequence.sOctalEscapeSequence = {octalEscapeSequence};
        }
        else
        {
            HexadecimalEscapeSequence* hexadecimalEscapeSequence = conHexadecimalEscapeSequence();
            if(hexadecimalEscapeSequence)
            {
                escapeSequence = new EscapeSequence();
                escapeSequence->eEscapeSequence = EscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE;
                escapeSequence->uEscapeSequence.sHexadecimalEscapeSequence = {hexadecimalEscapeSequence};
            }
            else
            {
                UniversalCharacterName* universalCharacterName = conUniversalCharacterName();
                if(universalCharacterName)
                {
                    escapeSequence = new EscapeSequence();
                    escapeSequence->eEscapeSequence = EscapeSequence::UNIVERSAL_CHARACTER_NAME;
                    escapeSequence->uEscapeSequence.sUniversalCharacterName = {universalCharacterName};
                }
                else
                    isValid = false;
            }
        }
    }

    if(isValid)
        return escapeSequence;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Group* Tokenizer::conGroup(Group* bef)
{
    auto idx = mIdx;
    bool isValid = true;
    Group* group = nullptr;

    if(!bef)
    {
        GroupPart* groupPart = conGroupPart();
        if(groupPart)
        {
            group = new Group();
            group->eGroup = Group::GROUP_PART;
            group->uGroup.sGroupPart = {groupPart};
        }
        else
            isValid = false;
    }
    else
    {
        GroupPart* groupPart = conGroupPart();
        if(groupPart)
        {
            group = new Group();
            group->eGroup = Group::GROUP_GROUP_PART;
            group->uGroup.sGroupGroupPart = {bef, groupPart};
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        Group* aft = conGroup(group);
        if(aft)
            return aft;
        else
            return group;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

GroupPart* Tokenizer::conGroupPart()
{
    auto idx = mIdx;
    bool isValid = true;
    GroupPart* groupPart = nullptr;

    IfSection* ifSection = conIfSection();
    if(ifSection)
    {
        groupPart = new GroupPart();
        groupPart->eGroupPart = GroupPart::IF_SECTION;
        groupPart->uGroupPart.sIfSection = {ifSection};
    }
    else
    {
        ControlLine* controlLine = conControlLine();
        if(controlLine)
        {
            groupPart = new GroupPart();
            groupPart->eGroupPart = GroupPart::CONTROL_LINE;
            groupPart->uGroupPart.sControlLine = {controlLine};
        }
        else
        {
            TextLine* textLine = conTextLine();
            if(textLine)
            {
                groupPart = new GroupPart();
                groupPart->eGroupPart = GroupPart::TEXT_LINE;
                groupPart->uGroupPart.sTextLine = {textLine};
            }
            else
            {
                if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)))
                {
                    mIdx++;
                    NonDirective* nonDirective = conNonDirective();
                    if(nonDirective)
                    {
                        groupPart = new GroupPart();
                        groupPart->eGroupPart = GroupPart::NON_DIRECTIVE;
                        groupPart->uGroupPart.sNonDirective = {nonDirective};
                    }
                    else
                        isValid = false;
                }
                else
                    isValid = false;
            }
        }
    }

    if(isValid)
        return groupPart;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HChar* Tokenizer::conHChar()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if(c == '\n' ||
       c == '>')
        return nullptr;

    HChar* hChar = new HChar();
    hChar->element = c;

    mIdx++;
    return hChar;
}

HCharSequence* Tokenizer::conHCharSequence(HCharSequence* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    HCharSequence* hCharSequence = nullptr;

    HChar* hChar = conHChar();
    if(hChar)
    {
        if(!bef)
        {
            hCharSequence = new HCharSequence();
            hCharSequence->eHCharSequence = HCharSequence::H_CHAR;
            hCharSequence->uHCharSequence.sHChar = {hChar};
        }
        else
        {
            hCharSequence = new HCharSequence();
            hCharSequence->eHCharSequence = HCharSequence::H_CHAR_SEQUENCE_H_CHAR;
            hCharSequence->uHCharSequence.sHCharSequenceHChar = {bef, hChar};
        }
    }
    else
        isValid = false;

    if(isValid)
    {
        HCharSequence* aft = conHCharSequence(hCharSequence);
        if(aft)
            return aft;
        else
            return hCharSequence;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HeaderName* Tokenizer::conHeaderName()
{
    if(mIdx >= mSrc.size() ||
       mPpTokens.size() <= 1)
        return nullptr;

    if(*mPpTokens.at(mPpTokens.size() - 1) != PreprocessingToken("include", PreprocessingToken::IDENTIFIER) ||
       *mPpTokens.at(mPpTokens.size() - 2) != PreprocessingToken("#", PreprocessingToken::PUNCTUATOR))
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    HeaderName* headerName = nullptr;

    char c = mSrc.at(mIdx);
    if(c == '<')
    {
        mIdx++;
        HCharSequence* hCharSequence = conHCharSequence();
        if(hCharSequence)
        {
            if(mIdx < mSrc.size() &&
               mSrc[mIdx] == '>')
            {
                mIdx++;

                headerName = new HeaderName();
                headerName->eHeaderName = HeaderName::H_CHAR_SEQUENCE;
                headerName->uHeaderName.sHCharSequence = {hCharSequence};
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(c == '"')
    {
        mIdx++;
        QCharSequence* qCharSequence = conQCharSequence();
        if(qCharSequence)
        {
            if(mIdx < mSrc.size() &&
               mSrc[mIdx] == '"')
            {
                mIdx++;

                headerName = new HeaderName();
                headerName->eHeaderName = HeaderName::Q_CHAR_SEQUENCE;
                headerName->uHeaderName.sQCharSequence = {qCharSequence};
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
        return headerName;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HexadecimalDigit* Tokenizer::conHexadecimalDigit()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if((c < '0' || c > '9') &&
       (c < 'a' || c > 'f') &&
       (c < 'A' || c > 'F'))
        return nullptr;
    
    HexadecimalDigit* hexadecimalDigit = new HexadecimalDigit();
    hexadecimalDigit->element = c;

    mIdx++;
    return hexadecimalDigit;
}

HexadecimalEscapeSequence* Tokenizer::conHexadecimalEscapeSequence(HexadecimalEscapeSequence* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    HexadecimalEscapeSequence* hexadecimalEscapeSequence = nullptr;

    if(!bef)
    {
        if(mSrc.at(mIdx) == '\\' &&
           mSrc[mIdx + 1] == 'x' &&
           mIdx + 1 < mSrc.size())
        {
            mIdx += 2;

            HexadecimalDigit* hexadecimalDigit = conHexadecimalDigit();
            if(hexadecimalDigit)
            {
                hexadecimalEscapeSequence = new HexadecimalEscapeSequence();
                hexadecimalEscapeSequence->eHexadecimalEscapeSequence = HexadecimalEscapeSequence::HEXADECIMAL_DIGIT;
                hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalDigit = {hexadecimalDigit};
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        HexadecimalDigit* hexadecimalDigit = conHexadecimalDigit();
        if(hexadecimalDigit)
        {
            hexadecimalEscapeSequence = new HexadecimalEscapeSequence();
            hexadecimalEscapeSequence->eHexadecimalEscapeSequence = HexadecimalEscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE_HEXADECIMAL_DIGIT;
            hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit = {bef, hexadecimalDigit};
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        HexadecimalEscapeSequence* aft = conHexadecimalEscapeSequence(hexadecimalEscapeSequence);
        if(aft)
            return aft;
        else
            return hexadecimalEscapeSequence;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HexQuad* Tokenizer::conHexQuad()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;

    std::array<HexadecimalDigit*, 4> hexadecimalDigits;
    for(auto&& e : hexadecimalDigits)
    {
        e = conHexadecimalDigit();
        if(!e)
        {
            mIdx = idx;
            return nullptr;
        }
    }

    HexQuad* hexQuad = new HexQuad();
    for(std::size_t i = 0; i < hexadecimalDigits.size(); i++)
        hexQuad->hexadecimalDigits[i] = hexadecimalDigits[i];
    
    return hexQuad;
}

Identifier* Tokenizer::conIdentifier(Identifier* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    Identifier* identifier = nullptr;

    if(!bef)
    {
        IdentifierNondigit* identifierNondigit = conIdentifierNondigit();
        if(identifierNondigit)
        {
            identifier = new Identifier();
            identifier->eIdentifier = Identifier::IDENTIFIER_NONDIGIT;
            identifier->uIdentifier.sIdentifierNondigit = {identifierNondigit};
        }
        else
            isValid = false;
    }
    else
    {
        IdentifierNondigit* identifierNondigit = conIdentifierNondigit();
        if(identifierNondigit)
        {
            identifier = new Identifier();
            identifier->eIdentifier = Identifier::IDENTIFIER_IDENTIFIER_NODIGIT;
            identifier->uIdentifier.sIdentifierIdentifierNondigit = {bef, identifierNondigit};
        }
        else
        {
            Digit* digit = conDigit();
            if(digit)
            {
                identifier = new Identifier();
                identifier->eIdentifier = Identifier::IDENTIFIER_DIGIT;
                identifier->uIdentifier.sIdentifierDigit = {bef, digit};
            }
            else
                isValid = false;
        }
    }

    if(isValid)
    {
        Identifier* aft = conIdentifier(identifier);
        if(aft)
            return aft;
        else
            return identifier;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IdentifierList* Tokenizer::conIdentifierList(IdentifierList* bef)
{
    auto idx = mIdx;
    bool isValid = true;
    IdentifierList* identifierList = nullptr;

    if(!bef)
    {
        if(mPpTokens[mIdx]->eClass == PreprocessingToken::IDENTIFIER &&
           mIdx < mPpTokens.size())
        {
            mIdx++;

            identifierList = new IdentifierList();
            identifierList->eIdentifierList = IdentifierList::IDENTIFIER;
            identifierList->uIdentifierList.sIdentifier = {mPpTokens[mIdx - 1]};
        }
        else
            isValid = false;
    }
    else
    {
        if((*mPpTokens[mIdx]) == PreprocessingToken(",", PreprocessingToken::PUNCTUATOR) &&
           mPpTokens[mIdx + 1]->eClass == PreprocessingToken::IDENTIFIER &&
           mIdx + 1 < mPpTokens.size())
        {
            mIdx += 2;

            identifierList = new IdentifierList();
            identifierList->eIdentifierList = IdentifierList::IDENTIFIER_LIST_IDENTIFIER;
            identifierList->uIdentifierList.sIdentifierListIdentifier = {bef, mPpTokens[mIdx - 1]};
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        IdentifierList* aft = conIdentifierList(identifierList);
        if(aft)
            return aft;
        else
            return identifierList;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IdentifierNondigit* Tokenizer::conIdentifierNondigit()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    IdentifierNondigit* identifierNondigit = nullptr;

    Nondigit* nondigit = conNondigit();
    if(nondigit)
    {
        identifierNondigit = new IdentifierNondigit();
        identifierNondigit->eIdentifierNondigit = IdentifierNondigit::NONDIGIT;
        identifierNondigit->uIdentifierNondigit.sNondigit = {nondigit};
    }
    else
    {
        UniversalCharacterName* universalCharacterName = conUniversalCharacterName();
        if(universalCharacterName)
        {
            identifierNondigit = new IdentifierNondigit();
            identifierNondigit->eIdentifierNondigit = IdentifierNondigit::UNIVERSAL_CHARACTER_NAME;
            identifierNondigit->uIdentifierNondigit.sUniversalCharacterName = {universalCharacterName};
        }
        else
            isValid = false;
    }

    if(isValid)
        return identifierNondigit;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IfGroup* Tokenizer::conIfGroup()
{
    auto idx = mIdx;
    bool isValid = true;
    IfGroup* ifGroup = nullptr;

    if(isEquality(mIdx, PreprocessingToken("#", PreprocessingToken::PUNCTUATOR)))
    {
        mIdx++;
        
        if(isEquality(mIdx, PreprocessingToken("if", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            ConstantExpression* constantExpression = conConstantExpression();
            if(constantExpression)
            {
                NewLine* newLine = conNewLine();
                if(newLine)
                {
                    Group* group = conGroup();
                    
                    ifGroup = new IfGroup();
                    ifGroup->eIfGroup = IfGroup::IF_CONSTANT_EXPRESSION_NEW_LINE_GROUP;
                    ifGroup->uIfGroup.sIfConstantExpressionNewLineGroup = {constantExpression, newLine, group};
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("ifdef", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PreprocessingToken* identifier = nullptr;
            if(isEquality(mIdx, PreprocessingToken::IDENTIFIER))
            {
                identifier = mPpTokens[mIdx];
                mIdx++;

                NewLine* newLine = conNewLine();
                if(newLine)
                {
                    Group* group = conGroup();

                    ifGroup = new IfGroup();
                    ifGroup->eIfGroup = IfGroup::IFDEF_IDENTIFIER_NEW_LINE_GROUP;
                    ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup = {identifier, newLine, group};
                }
                else
                    isValid = false;
            }
            else
                isValid = false;
        }
        else if(isEquality(mIdx, PreprocessingToken("ifndef", PreprocessingToken::IDENTIFIER)))
        {
            mIdx++;

            PreprocessingToken* identifier = nullptr;
            if(isEquality(mIdx, PreprocessingToken::IDENTIFIER))
            {
                identifier = mPpTokens[mIdx];
                mIdx++;

                NewLine* newLine = conNewLine();
                if(newLine)
                {
                    Group* group = conGroup();

                    ifGroup = new IfGroup();
                    ifGroup->eIfGroup = IfGroup::IFNDEF_IDENTIFIER_NEW_LINE_GROUP;
                    ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup = {identifier, newLine, group};
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
        return ifGroup;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

IfSection* Tokenizer::conIfSection()
{
    auto idx = mIdx;
    bool isValid = true;
    IfSection* ifSection = nullptr;

    IfGroup* ifGroup = conIfGroup();
    if(ifGroup)
    {
        ElifGroups* elifGroups = conElifGroups();
        ElseGroup* elseGroup = conElseGroup();
        EndifLine* endifLine = conEndifLine();
        if(endifLine)
        {
            ifSection = new IfSection();
            ifSection->elifGroups = elifGroups;
            ifSection->elseGroup = elseGroup;
            ifSection->endifLine = endifLine;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return ifSection;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Lparen* Tokenizer::conLparen()
{
    auto idx = mIdx;
    bool isValid = true;
    Lparen* lparen = nullptr;

    if(mIdx >= 1 &&
       isEquality(mIdx, PreprocessingToken("(", PreprocessingToken::PUNCTUATOR)))
    {
        if(mPpTokens[mIdx - 1]->pos + mPpTokens[mIdx - 1]->data.size() ==
           mPpTokens[mIdx]->pos)
        {
            mIdx++;

            lparen = new Lparen();
            lparen->element = '(';
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return lparen;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

NewLine* Tokenizer::conNewLine()
{
    auto idx = mIdx;
    bool isValid = true;
    NewLine* newLine = nullptr;

    if(isEquality(mIdx, PreprocessingToken("\n", PreprocessingToken::OTHER)))
    {
        mIdx++;

        newLine = new NewLine();
        newLine->element = '\n';
    }
    else
        isValid = false;

    if(isValid)
        return newLine;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Nondigit* Tokenizer::conNondigit()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if((c < 'a' || c > 'z') &&
       (c < 'A' || c > 'Z') &&
       (c != '_'))
        return nullptr;

    Nondigit* nondigit = new Nondigit();
    nondigit->element = c;

    mIdx++;
    return nondigit;
}

NonDirective* Tokenizer::conNonDirective()
{
    auto idx = mIdx;
    bool isValid = true;
    NonDirective* nonDirective = nullptr;

    PpTokens* ppTokens = conPpTokens();
    if(ppTokens)
    {
        NewLine* newLine = conNewLine();
        if(newLine)
        {
            nonDirective = new NonDirective();
            nonDirective->ppTokens = ppTokens;
            nonDirective->newLine = newLine;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return nonDirective;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

OctalDigit* Tokenizer::conOctalDigit()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if(c < '0' || c > '7')
        return nullptr;

    OctalDigit* octalDigit = new OctalDigit();
    octalDigit->element = c;

    mIdx++;
    return octalDigit;
}

OctalEscapeSequence* Tokenizer::conOctalEscapeSequence()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    if(mSrc.at(mIdx) != '\\')
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    OctalEscapeSequence* octalEscapeSequence = nullptr;

    OctalDigit* octalDigit = conOctalDigit();
    if(octalDigit)
    {
        OctalDigit* octalDigit_1 = conOctalDigit();
        if(octalDigit_1)
        {
            OctalDigit* octalDigit_2 = conOctalDigit();
            if(octalDigit_2)
            {
                octalEscapeSequence = new OctalEscapeSequence();
                octalEscapeSequence->eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT_OCTAL_DIGIT;
                octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit = {octalDigit, octalDigit_1, octalDigit_2};
            }
            else
            {
                octalEscapeSequence = new OctalEscapeSequence();
                octalEscapeSequence->eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT;
                octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigit = {octalDigit, octalDigit_1};
            }
        }
        else
        {
            octalEscapeSequence = new OctalEscapeSequence();
            octalEscapeSequence->eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT;
            octalEscapeSequence->uOctalEscapeSequence.sOctalDigit = {octalDigit};
        }
    }
    else
        isValid = false;

    if(isValid)
        return octalEscapeSequence;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Other* Tokenizer::conOther()
{
    if(mIdx >= mSrc.size())
        return nullptr;
    
    Other* other = new Other();
    other->element = mSrc.at(mIdx);
    
    mIdx++;
    return other;
}

PpNumber* Tokenizer::conPpNumber(PpNumber* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    PpNumber* ppNumber = nullptr;

    if(!bef)
    {
        Digit* digit = conDigit();
        if(digit)
        {
            ppNumber = new PpNumber();
            ppNumber->ePpNumber = PpNumber::DIGIT;
            ppNumber->uPpNumber.sDigit = {digit};
        }
        else if(mSrc.at(mIdx) == '.')
        {
            digit = conDigit();
            if(digit)
            {
                ppNumber = new PpNumber();
                ppNumber->ePpNumber = PpNumber::DOT_DIGIT;
                ppNumber->uPpNumber.sDotDigit = {digit};
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        Digit* digit = conDigit();
        if(digit)
        {
            ppNumber = new PpNumber();
            ppNumber->ePpNumber = PpNumber::PP_NUMBER_DIGIT;
            ppNumber->uPpNumber.sPpNumberDigit = {bef, digit};
        }
        else
        {
            char c = mSrc.at(mIdx);

            if(c == '.')
            {
                mIdx++;
                ppNumber = new PpNumber();
                ppNumber->ePpNumber = PpNumber::PP_NUMBER_DOT;
                ppNumber->uPpNumber.sPpNumberDot = {bef};
            }
            else if(c == 'e')
            {
                mIdx++;
                Sign* sign = conSign();
                if(sign)
                {
                    ppNumber = new PpNumber();
                    ppNumber->ePpNumber = PpNumber::PP_NUMBER_e_SIGN;
                    ppNumber->uPpNumber.sPpNumber_e_Sign = {bef, sign};
                }
                else
                    isValid = false;
            }
            else if(c == 'E')
            {
                mIdx++;
                Sign* sign = conSign();
                if(sign)
                {
                    ppNumber = new PpNumber();
                    ppNumber->ePpNumber = PpNumber::PP_NUMBER_E_SIGN;
                    ppNumber->uPpNumber.sPpNumber_E_Sign = {bef, sign};
                }
                else
                    isValid = false;
            }
            else if(c == 'p')
            {
                mIdx++;
                Sign* sign = conSign();
                if(sign)
                {
                    ppNumber = new PpNumber();
                    ppNumber->ePpNumber = PpNumber::PP_NUMBER_p_SIGN;
                    ppNumber->uPpNumber.sPpNumber_p_Sign = {bef, sign};
                }
                else
                    isValid = false;
            }
            else if(c == 'P')
            {
                mIdx++;
                Sign* sign = conSign();
                if(sign)
                {
                    ppNumber = new PpNumber();
                    ppNumber->ePpNumber = PpNumber::PP_NUMBER_P_SIGN;
                    ppNumber->uPpNumber.sPpNumber_P_Sign = {bef, sign};
                }
                else
                    isValid = false;
            }
            else
                isValid = false;

            if(!isValid)
            {
                mIdx = idx;
                IdentifierNondigit* identifierNondigit = conIdentifierNondigit();
                if(identifierNondigit)
                {
                    isValid = true;

                    ppNumber = new PpNumber();
                    ppNumber->ePpNumber = PpNumber::PP_NUMBER_IDENTIFIER_NONDIGIT;
                    ppNumber->uPpNumber.sPpNumberIdentifierNondigit = {bef, identifierNondigit};
                }
                else
                    isValid = false;
            }
        }
    }

    if(isValid)
    {
        PpNumber* aft = conPpNumber(ppNumber);
        if(aft)
            return aft;
        else
            return ppNumber;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

PpTokens* Tokenizer::conPpTokens(PpTokens* bef)
{
    auto idx = mIdx;
    bool isValid = true;
    PpTokens* ppTokens = nullptr;

    if(!bef)
    {
        if(!isEquality(mIdx, PreprocessingToken("\n", PreprocessingToken::OTHER)))
        {
            ppTokens = new PpTokens();
            ppTokens->ePpTokens = PpTokens::PREPROCESSING_TOKEN;
            ppTokens->uPpTokens.sPreprocessingToken = {mPpTokens[mIdx]};

            mIdx++;
        }
        else
            isValid = false;
    }
    else
    {
        if(!isEquality(mIdx, PreprocessingToken("\n", PreprocessingToken::OTHER)))
        {
            ppTokens = new PpTokens();
            ppTokens->ePpTokens = PpTokens::PP_TOKENS_PREPROCESSING_TOKEN;
            ppTokens->uPpTokens.sPpTokensPreprocessingToken = {bef, mPpTokens[mIdx]};
        
            mIdx++;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        PpTokens* aft = conPpTokens(ppTokens);
        if(aft)
            return aft;
        else
            return ppTokens;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Punctuator* Tokenizer::conPunctuator()
{
    if(mIdx >= mSrc.size())
        return nullptr;
    
    std::string data;
    if(mIdx + 3 < mSrc.size())
    {
        if(mSrc[mIdx] == '%' &&
           mSrc[mIdx + 1] == ':' &&
           mSrc[mIdx + 2] == '%' &&
           mSrc[mIdx + 3] == ':')
            data = "##";

        if(!data.empty())
            mIdx += 4;
    }
    if(mIdx + 2 < mSrc.size() &&
       data.empty())
    {
        if(mSrc[mIdx] == '.' &&
           mSrc[mIdx + 1] == '.' &&
           mSrc[mIdx + 2] == '.')
            data = "...";
        else if(mSrc[mIdx] == '<' &&
                mSrc[mIdx + 1] == '<' &&
                mSrc[mIdx + 2] == '=')
            data = "<<=";
        else if(mSrc[mIdx] == '>' &&
                mSrc[mIdx + 1] == '>' &&
                mSrc[mIdx + 2] == '=')
            data = ">>=";

        if(!data.empty())
            mIdx += 3;
    }
    if(mIdx + 1 < mSrc.size() &&
       data.empty())
    {
        if(mSrc[mIdx] == '-' &&
           mSrc[mIdx + 1] == '>')
            data = "->";
        else if(mSrc[mIdx] == '+' &&
                mSrc[mIdx + 1] == '+')
            data = "++";
        else if(mSrc[mIdx] == '-' &&
                mSrc[mIdx + 1] == '-')
            data = "--";
        else if(mSrc[mIdx] == '<' &&
                mSrc[mIdx + 1] == '<')
            data = "<<";
        else if(mSrc[mIdx] == '>' &&
                mSrc[mIdx + 1] == '>')
            data = ">>";
        else if(mSrc[mIdx] == '<' &&
                mSrc[mIdx + 1] == '=')
            data = "<=";
        else if(mSrc[mIdx] == '>' &&
                mSrc[mIdx + 1] == '=')
            data = ">=";
        else if(mSrc[mIdx] == '=' &&
                mSrc[mIdx + 1] == '=')
            data = "==";
        else if(mSrc[mIdx] == '!' &&
                mSrc[mIdx + 1] == '=')
            data = "!=";
        else if(mSrc[mIdx] == '&' &&
                mSrc[mIdx + 1] == '&')
            data = "&&";
        else if(mSrc[mIdx] == '|' &&
                mSrc[mIdx + 1] == '|')
            data = "||";
        else if(mSrc[mIdx] == '*' &&
                mSrc[mIdx + 1] == '=')
            data = "*=";
        else if(mSrc[mIdx] == '/' &&
                mSrc[mIdx + 1] == '=')
            data = "/=";
        else if(mSrc[mIdx] == '%' &&
                mSrc[mIdx + 1] == '=')
            data = "%=";
        else if(mSrc[mIdx] == '+' &&
                mSrc[mIdx + 1] == '=')
            data = "+=";
        else if(mSrc[mIdx] == '-' &&
                mSrc[mIdx + 1] == '=')
            data = "-=";
        else if(mSrc[mIdx] == '&' &&
                mSrc[mIdx + 1] == '=')
            data = "&=";
        else if(mSrc[mIdx] == '^' &&
                mSrc[mIdx + 1] == '=')
            data = "^=";
        else if(mSrc[mIdx] == '|' &&
                mSrc[mIdx + 1] == '=')
            data = "|=";
        else if(mSrc[mIdx] == '#' &&
                mSrc[mIdx + 1] == '#')
            data = "##";
        else if(mSrc[mIdx] == '<' &&
                mSrc[mIdx + 1] == ':')
            data = "[";
        else if(mSrc[mIdx] == ':' &&
                mSrc[mIdx + 1] == '>')
            data = "]";
        else if(mSrc[mIdx] == '<' &&
                mSrc[mIdx + 1] == '%')
            data = "{";
        else if(mSrc[mIdx] == '%' &&
                mSrc[mIdx + 1] == '>')
            data = "}";
        else if(mSrc[mIdx] == '%' &&
                mSrc[mIdx + 1] == ':')
            data = "#";

        if(!data.empty())
            mIdx += 2;
    }
    if(data.empty())
    {
        char c = mSrc[mIdx];
        if(c == '[' ||
           c == ']' ||
           c == '(' ||
           c == ')' ||
           c == '{' ||
           c == '}' ||
           c == '.' ||
           c == '&' ||
           c == '*' ||
           c == '+' ||
           c == '-' ||
           c == '~' ||
           c == '!' ||
           c == '/' ||
           c == '%' ||
           c == '<' ||
           c == '>' ||
           c == '^' ||
           c == '|' ||
           c == '?' ||
           c == ':' ||
           c == ';' ||
           c == '=' ||
           c == ',' ||
           c == '#')
            data = c;

        if(!data.empty())
            mIdx += 1;
    }

    if(!data.empty())
    {
        Punctuator* punctuator = new Punctuator();
        punctuator->element = std::move(data);
        return punctuator;
    }
    else
        return nullptr;
}

QChar* Tokenizer::conQChar()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if(c == '\n' ||
       c == '"')
        return nullptr;

    QChar* qChar = new QChar();
    qChar->element = c;

    mIdx++;
    return qChar;
}

QCharSequence* Tokenizer::conQCharSequence(QCharSequence* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    QCharSequence* qCharSequence = nullptr;

    QChar* qChar = conQChar();
    if(qChar)
    {
        if(!bef)
        {
            qCharSequence = new QCharSequence();
            qCharSequence->eQCharSequence = QCharSequence::Q_CHAR;
            qCharSequence->uQCharSequence.sQChar = {qChar};
        }
        else
        {
            qCharSequence = new QCharSequence();
            qCharSequence->eQCharSequence = QCharSequence::Q_CHAR_SEQUENCE_Q_CHAR;
            qCharSequence->uQCharSequence.sQCharSequenceQChar = {bef, qChar};
        }
    }
    else
        isValid = false;

    if(isValid)
    {
        QCharSequence* aft = conQCharSequence(qCharSequence);
        if(aft)
            return aft;
        else
            return qCharSequence;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

ReplacementList* Tokenizer::conReplacementList()
{
    PpTokens* ppTokens = conPpTokens();
    ReplacementList* replacementList = new ReplacementList();
    replacementList->ppTokens = ppTokens;

    return replacementList;
}

SChar* Tokenizer::conSChar()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    SChar* sChar = nullptr;

    char c = mSrc.at(mIdx);
    if(c != '"' &&
       c != '\\' &&
       c != '\n')
    {
        mIdx++;

        sChar = new SChar();
        sChar->eSChar = SChar::ANY_MEMBER;
        sChar->uSChar.sAnyMember = {c};
    }
    else
    {
        EscapeSequence* escapeSequence = conEscapeSequence();
        if(escapeSequence)
        {
            sChar = new SChar();
            sChar->eSChar = SChar::ESCAPE_SEQUENCE;
            sChar->uSChar.sEscapeSequence = {escapeSequence};
        }
        else
            isValid = false;
    }

    if(isValid)
        return sChar;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

SCharSequence* Tokenizer::conSCharSequence(SCharSequence* bef)
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    SCharSequence* sCharSequence = nullptr;

    if(!bef)
    {
        SChar* sChar = conSChar();
        if(sChar)
        {
            sCharSequence = new SCharSequence();
            sCharSequence->eSCharSequence = SCharSequence::S_CHAR;
            sCharSequence->uSCharSequence.sSChar = {sChar};
        }
        else
            isValid = false;
    }
    else
    {
        SChar* sChar = conSChar();
        if(sChar)
        {
            sCharSequence = new SCharSequence();
            sCharSequence->eSCharSequence = SCharSequence::S_CHAR_SEQUENCE_S_CHAR;
            sCharSequence->uSCharSequence.sSCharSequenceSChar = {bef, sChar}; 
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        SCharSequence* aft = conSCharSequence(sCharSequence);
        if(aft)
            return aft;
        else
            return sCharSequence;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Sign* Tokenizer::conSign()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    char c = mSrc.at(mIdx);
    if(c != '+' ||
       c != '-')
        return nullptr;
    
    Sign* sign = new Sign();
    sign->element = c;

    mIdx++;
    return sign;
}

SimpleEscapeSequence* Tokenizer::conSimpleEscapeSequence()
{
    if(mIdx + 1 >= mSrc.size())
        return nullptr;
    
    if(mSrc.at(mIdx) != '\\')
        return nullptr;

    char c = mSrc.at(mIdx + 1);
    if(c != '\'' &&
       c != '"' &&
       c != '?' &&
       c != '\\' &&
       c != 'a' &&
       c != 'b' &&
       c != 'f' &&
       c != 'n' &&
       c != 'r' &&
       c != 't' &&
       c != 'v')
        return nullptr;

    SimpleEscapeSequence* simpleEscapeSequence = new SimpleEscapeSequence();
    simpleEscapeSequence->element = c;

    mIdx += 2;
    return simpleEscapeSequence;
}

StringLiteral* Tokenizer::conStringLiteral()
{
    if(mIdx >= mSrc.size())
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    StringLiteral* stringLiteral = nullptr;

    EncodingPrefix* encodingPrefix = conEncodingPrefix();
    
    if(mSrc[mIdx] == '"' &&
       mIdx < mSrc.size())
    {
        mIdx++;

        SCharSequence* sCharSequence = conSCharSequence();
        
        if(mSrc[mIdx] == '"' &&
           mIdx < mSrc.size())
        {
            mIdx++;

            stringLiteral = new StringLiteral();
            stringLiteral->encodingPrefix = encodingPrefix;
            stringLiteral->sCharSequence = sCharSequence;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return stringLiteral;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

TextLine* Tokenizer::conTextLine()
{
    auto idx = mIdx;
    bool isValid = true;
    TextLine* textLine = nullptr;

    PpTokens* ppTokens = conPpTokens();
    NewLine* newLine = conNewLine();
    if(newLine)
    {
        textLine = new TextLine();
        textLine->ppTokens = ppTokens;
        textLine->newLine = newLine;
    }
    else
        isValid = false;

    if(isValid)
        return textLine;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

UniversalCharacterName* Tokenizer::conUniversalCharacterName()
{
    if(mIdx + 1 >= mSrc.size())
        return nullptr;

    if(mSrc.at(mIdx) != '\\')
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    UniversalCharacterName* universalCharacterName = nullptr;

    char c = mSrc.at(mIdx + 1);
    if(c == 'u')
    {
        mIdx += 2;

        HexQuad* hexQuad = conHexQuad();
        if(hexQuad)
        {
            universalCharacterName = new UniversalCharacterName();
            universalCharacterName->eUniversalCharacterName = UniversalCharacterName::HEX_QUAD;
            universalCharacterName->uUniversalCharacterName.sHexQuad = {hexQuad};
        }
        else
            isValid = false;
    }
    else if(c == 'U')
    {
        mIdx += 2;

        HexQuad* hexQuad = conHexQuad();
        if(hexQuad)
        {
            HexQuad* hexQuad_1 = conHexQuad();
            if(hexQuad_1)
            {
                universalCharacterName = new UniversalCharacterName();
                universalCharacterName->eUniversalCharacterName = UniversalCharacterName::HEX_QUAD_HEX_QUAD;
                universalCharacterName->uUniversalCharacterName.sHexQuadHexQuad = {hexQuad, hexQuad_1};
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
        return universalCharacterName;
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

void Tokenizer::process(PreprocessingToken_Symbol* preprocessingToken_symbol, std::string& data) const
{
    switch(preprocessingToken_symbol->ePreprocessingToken)
    {
        case(PreprocessingToken_Symbol::HEADER_NAME):
            process(preprocessingToken_symbol->uPreprocessingToken.sHeaderName.headerName, data);
            break;
        case(PreprocessingToken_Symbol::IDENTIFIER):
            process(preprocessingToken_symbol->uPreprocessingToken.sIdentifier.identifier, data);
            break;
        case(PreprocessingToken_Symbol::PP_NUMBER):
            process(preprocessingToken_symbol->uPreprocessingToken.sPpNumber.ppNumber, data);
            break;
        case(PreprocessingToken_Symbol::CHARACTER_CONSTANT):
            process(preprocessingToken_symbol->uPreprocessingToken.sCharacterConstant.characterConstant, data);
            break;
        case(PreprocessingToken_Symbol::STRING_LITERAL):
            process(preprocessingToken_symbol->uPreprocessingToken.sStringLiteral.stringLiteral, data);
            break;
        case(PreprocessingToken_Symbol::PUNCTUATOR):
            process(preprocessingToken_symbol->uPreprocessingToken.sPunctuator.punctuator, data);
            break;
        case(PreprocessingToken_Symbol::OTHER):
            process(preprocessingToken_symbol->uPreprocessingToken.sOther.other, data);
            break;

        case(PreprocessingToken_Symbol::NONE):
            processError("PreprocessingToken_Symbol", data);
            break;
    }
}

void Tokenizer::process(CChar* cChar, std::string& data) const
{
    switch(cChar->eCChar)
    {
        case(CChar::ANY_MEMBER):
            data.push_back(cChar->uCChar.sAnyMember.element);
            break;
        case(CChar::ESCAPE_CEQUENCE):
            process(cChar->uCChar.sEscapeSequence.escapeSequence, data);
            break;
        
        case(CChar::NONE):
            processError("CChar", data);
            break;
    }
}

void Tokenizer::process(CCharSequence* cCharSequence, std::string& data) const
{
    switch(cCharSequence->eCCharSequence)
    {
        case(CCharSequence::C_CHAR):
            process(cCharSequence->uCCharSequence.sCChar.cChar, data);
            break;
        case(CCharSequence::C_CHAR_SEQUENCE_C_CHAR):
            process(cCharSequence->uCCharSequence.sCCharSequenceCChar.cCharSequence, data);
            process(cCharSequence->uCCharSequence.sCCharSequenceCChar.cChar, data);
            break;

        case(CCharSequence::NONE):
            processError("CCharSequence", data);
            break;
    }
}

void Tokenizer::process(CharacterConstant* characterConstant, std::string& data) const
{
    switch(characterConstant->eCharacterConstant)
    {
        case(CharacterConstant::C_CHAR_SEQUENCE):
            data.push_back('\'');
            process(characterConstant->uCharacterConstant.sCCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::L_C_CHAR_SEQUENCE):
            data.push_back('L');
            data.push_back('\'');
            process(characterConstant->uCharacterConstant.s_L_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::u_C_CHAR_SEQUENCE):
            data.push_back('u');
            data.push_back('\'');
            process(characterConstant->uCharacterConstant.s_u_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::U_C_CHAR_SEQUENCE):
            data.push_back('U');
            data.push_back('\'');
            process(characterConstant->uCharacterConstant.s_U_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        
        case(CharacterConstant::NONE):
            processError("CharacterConstant", data);
            break;
    }
}

void Tokenizer::process(Digit* digit, std::string& data) const
{
    data.push_back(digit->element);
}

void Tokenizer::process(EncodingPrefix* encodingPrefix, std::string& data) const
{
    data.append(encodingPrefix->element);
}

void Tokenizer::process(EscapeSequence* escapeSequence, std::string& data) const
{
    switch(escapeSequence->eEscapeSequence)
    {
        case(EscapeSequence::SIMPLE_ESCAPE_SEQUENCE):
            process(escapeSequence->uEscapeSequence.sSimpleEscapeSequence.simpleEscapeSequence, data);
            break;
        case(EscapeSequence::OCTAL_ESCAPE_SEQUENCE):
            process(escapeSequence->uEscapeSequence.sOctalEscapeSequence.octalEscapeSequence, data);
            break;
        case(EscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE):
            process(escapeSequence->uEscapeSequence.sHexadecimalEscapeSequence.hexadecimalEscapeSequence, data);
            break;
        case(EscapeSequence::UNIVERSAL_CHARACTER_NAME):
            process(escapeSequence->uEscapeSequence.sUniversalCharacterName.universalCharacterName, data);
            break;
        
        case(EscapeSequence::NONE):
            processError("EscapeSequence", data);
            break;
    }
}

void Tokenizer::process(HChar* hChar, std::string& data) const
{
    data.push_back(hChar->element);
}

void Tokenizer::process(HCharSequence* hCharSequence, std::string& data) const
{
    switch(hCharSequence->eHCharSequence)
    {
        case(HCharSequence::H_CHAR):
            process(hCharSequence->uHCharSequence.sHChar.hChar, data);
            break;
        case(HCharSequence::H_CHAR_SEQUENCE_H_CHAR):
            process(hCharSequence->uHCharSequence.sHCharSequenceHChar.hCharSequence, data);
            process(hCharSequence->uHCharSequence.sHCharSequenceHChar.hChar, data);
            break;

        case(HCharSequence::NONE):
            processError("HCharSequence", data);
            break;
    }
}

void Tokenizer::process(HeaderName* headerName, std::string& data) const
{
    switch(headerName->eHeaderName)
    {
        case(HeaderName::H_CHAR_SEQUENCE):
            data.push_back('<');
            process(headerName->uHeaderName.sHCharSequence.hCharSequence, data);
            data.push_back('>');
            break;
        case(HeaderName::Q_CHAR_SEQUENCE):
            data.push_back('"');
            process(headerName->uHeaderName.sQCharSequence.qCharSequence, data);
            data.push_back('"');
            break;

        case(HeaderName::NONE):
            processError("HeaderName", data);
            break;
    }
}

void Tokenizer::process(HexadecimalDigit* hexadecimalDigit, std::string& data) const
{
    data.push_back(hexadecimalDigit->element);
}

void Tokenizer::process(HexadecimalEscapeSequence* hexadecimalEscapeSequence, std::string& data) const
{
    switch(hexadecimalEscapeSequence->eHexadecimalEscapeSequence)
    {
        case(HexadecimalEscapeSequence::HEXADECIMAL_DIGIT):
            data.push_back('\\');
            data.push_back('x');
            process(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalDigit.hexadecimalDigit, data);
            break;
        case(HexadecimalEscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE_HEXADECIMAL_DIGIT):
            process(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit.hexadecimalEscapeSequence, data);
            process(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit.hexadecimalDigit, data);
            break;

        case(HexadecimalEscapeSequence::NONE):
            processError("HexadecimalEscapeSequence", data);
            break;
    }
}

void Tokenizer::process(HexQuad* hexQuad, std::string& data) const
{
    for(auto&& hexadecimalDigit : hexQuad->hexadecimalDigits)
        process(hexadecimalDigit, data);
}

void Tokenizer::process(Identifier* identifier, std::string& data) const
{
    switch(identifier->eIdentifier)
    {
        case(Identifier::IDENTIFIER_NONDIGIT):
            process(identifier->uIdentifier.sIdentifierNondigit.identifierNondigit, data);
            break;
        case(Identifier::IDENTIFIER_IDENTIFIER_NODIGIT):
            process(identifier->uIdentifier.sIdentifierIdentifierNondigit.identifier, data);
            process(identifier->uIdentifier.sIdentifierIdentifierNondigit.identifierNondigit, data);
            break;
        case(Identifier::IDENTIFIER_DIGIT):
            process(identifier->uIdentifier.sIdentifierDigit.identifier, data);
            process(identifier->uIdentifier.sIdentifierDigit.digit, data);
            break;

        case(Identifier::NONE):
            processError("Identifier", data);
            break;
    }
}

void Tokenizer::process(IdentifierNondigit* identifierNondigit, std::string& data) const
{
    switch(identifierNondigit->eIdentifierNondigit)
    {
        case(IdentifierNondigit::NONDIGIT):
            process(identifierNondigit->uIdentifierNondigit.sNondigit.nondigit, data);
            break;
        case(IdentifierNondigit::UNIVERSAL_CHARACTER_NAME):
            process(identifierNondigit->uIdentifierNondigit.sUniversalCharacterName.universalCharacterName, data);
            break;

        case(IdentifierNondigit::NONE):
            processError("IdentifierNondigit", data);
            break;
    }
}

void Tokenizer::process(Nondigit* nondigit, std::string& data) const
{
    data.push_back(nondigit->element);
}

void Tokenizer::process(OctalDigit* octalDigit, std::string& data) const
{
    data.push_back(octalDigit->element);
}

void Tokenizer::process(OctalEscapeSequence* octalEscapeSequence, std::string& data) const
{
    data.push_back('\\');

    switch(octalEscapeSequence->eOctalEscapeSequence)
    {
        case(OctalEscapeSequence::OCTAL_DIGIT):
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigit.octalDigit, data);
            break;
        case(OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT):
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigit.octalDigit, data);
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigit.octalDigit_1, data);
            break;
        case(OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT_OCTAL_DIGIT):
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit, data);
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit_1, data);
            process(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit_2, data);
            break;

        case(OctalEscapeSequence::NONE):
            processError("OctalEscapeSequence", data);
            break;
    }
}

void Tokenizer::process(Other* other, std::string& data) const
{
    data.push_back(other->element);
}

void Tokenizer::process(PpNumber* ppNumber, std::string& data) const
{
    switch(ppNumber->ePpNumber)
    {
        case(PpNumber::DIGIT):
            process(ppNumber->uPpNumber.sDigit.digit, data);
            break;
        case(PpNumber::DOT_DIGIT):
            data.push_back('.');
            process(ppNumber->uPpNumber.sDotDigit.digit, data);
            break;
        case(PpNumber::PP_NUMBER_DIGIT):
            process(ppNumber->uPpNumber.sPpNumberDigit.ppNumber, data);
            process(ppNumber->uPpNumber.sPpNumberDigit.digit, data);
            break;
        case(PpNumber::PP_NUMBER_IDENTIFIER_NONDIGIT):
            process(ppNumber->uPpNumber.sPpNumberIdentifierNondigit.ppNumber, data);
            process(ppNumber->uPpNumber.sPpNumberIdentifierNondigit.identifierNondigit, data);
            break;
        case(PpNumber::PP_NUMBER_e_SIGN):
            process(ppNumber->uPpNumber.sPpNumber_e_Sign.ppNumber, data);
            data.push_back('e');
            process(ppNumber->uPpNumber.sPpNumber_e_Sign.sign, data);
            break;
        case(PpNumber::PP_NUMBER_E_SIGN):
            process(ppNumber->uPpNumber.sPpNumber_E_Sign.ppNumber, data);
            data.push_back('E');
            process(ppNumber->uPpNumber.sPpNumber_E_Sign.sign, data);
            break;
        case(PpNumber::PP_NUMBER_p_SIGN):
            process(ppNumber->uPpNumber.sPpNumber_p_Sign.ppNumber, data);
            data.push_back('p');
            process(ppNumber->uPpNumber.sPpNumber_p_Sign.sign, data);
            break;
        case(PpNumber::PP_NUMBER_P_SIGN):
            process(ppNumber->uPpNumber.sPpNumber_P_Sign.ppNumber, data);
            data.push_back('P');
            process(ppNumber->uPpNumber.sPpNumber_P_Sign.sign, data);
            break;
        case(PpNumber::PP_NUMBER_DOT):
            process(ppNumber->uPpNumber.sPpNumberDot.ppNumber, data);
            data.push_back('.');
            break;
        
        case(PpNumber::NONE):
            processError("PpNumber", data);
            break;
    }
}

void Tokenizer::process(Punctuator* punctuator, std::string& data) const
{
    data.append(punctuator->element);
}

void Tokenizer::process(QChar* qChar, std::string& data) const
{
    data.push_back(qChar->element);
}

void Tokenizer::process(QCharSequence* qCharSequence, std::string& data) const
{
    switch(qCharSequence->eQCharSequence)
    {
        case(QCharSequence::Q_CHAR):
            process(qCharSequence->uQCharSequence.sQChar.qChar, data);
            break;
        case(QCharSequence::Q_CHAR_SEQUENCE_Q_CHAR):
            process(qCharSequence->uQCharSequence.sQCharSequenceQChar.qCharSequence, data);
            process(qCharSequence->uQCharSequence.sQCharSequenceQChar.qChar, data);
            break;
        
        case(QCharSequence::NONE):
            processError("QCharSequence", data);
            break;
    }
}

void Tokenizer::process(SChar* sChar, std::string& data) const
{
    switch(sChar->eSChar)
    {
        case(SChar::ANY_MEMBER):
            data.push_back(sChar->uSChar.sAnyMember.element);
            break;
        case(SChar::ESCAPE_SEQUENCE):
            process(sChar->uSChar.sEscapeSequence.escapeSequence, data);
            break;
        
        case(SChar::NONE):
            processError("SChar", data);
            break;
    }
}

void Tokenizer::process(SCharSequence* sCharSequence, std::string& data) const
{
    switch(sCharSequence->eSCharSequence)
    {
        case(SCharSequence::S_CHAR):
            process(sCharSequence->uSCharSequence.sSChar.sChar, data);
            break;
        case(SCharSequence::S_CHAR_SEQUENCE_S_CHAR):
            process(sCharSequence->uSCharSequence.sSCharSequenceSChar.sCharSequence, data);
            process(sCharSequence->uSCharSequence.sSCharSequenceSChar.sChar, data);
            break;

        case(SCharSequence::NONE):
            processError("SCharSequence", data);
            break;
    }
}

void Tokenizer::process(Sign* sign, std::string& data) const
{
    data.push_back(sign->element);
}

void Tokenizer::process(SimpleEscapeSequence* simpleEscapeSequence, std::string& data) const
{
    data.push_back('\\');
    data.push_back(simpleEscapeSequence->element);
}

void Tokenizer::process(StringLiteral* stringLiteral, std::string& data) const
{
    if(stringLiteral->encodingPrefix)
        process(stringLiteral->encodingPrefix, data);
    
    data.push_back('"');

    if(stringLiteral->sCharSequence)
        process(stringLiteral->sCharSequence, data);
    
    data.push_back('"');
}

void Tokenizer::process(UniversalCharacterName* universalCharacterName, std::string& data) const
{
    switch(universalCharacterName->eUniversalCharacterName)
    {
        case(UniversalCharacterName::HEX_QUAD):
            data.append("\\u");
            process(universalCharacterName->uUniversalCharacterName.sHexQuad.hexQuad, data);
            break;
        case(UniversalCharacterName::HEX_QUAD_HEX_QUAD):
            data.append("\\U");
            process(universalCharacterName->uUniversalCharacterName.sHexQuadHexQuad.hexQuad, data);
            process(universalCharacterName->uUniversalCharacterName.sHexQuadHexQuad.hexQuad_1, data);
            break;

        case(UniversalCharacterName::NONE):
            processError("UniversalCharacterName", data);
            break;
    }
}

void Tokenizer::processError(const char* message, const std::string& data) const
{
    std::cerr << "impl-error: symbol enumeration is not setting.\n"
              << "    class: "
              << message
              << "\n    data: "
              << data
              << std::endl;
}

bool Tokenizer::isEquality(std::size_t idx, PreprocessingToken&& ppToken) const
{
    if(idx < mPpTokens.size())
        return (*mPpTokens[mIdx]) == std::forward<PreprocessingToken&&>(ppToken);
    else
        return false;
}

bool Tokenizer::isEquality(std::size_t idx, PreprocessingToken::EClass eClass) const
{
    if(idx < mPpTokens.size())
        return mPpTokens[mIdx]->eClass == eClass;
    else
        return false;
}