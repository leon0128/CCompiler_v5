#include "token.hpp"
#include <iostream>

void TOKEN::error(const char* message)
{
    std::cerr << "implementation error:\n"
              << "    what: token is not setted enumeration.\n"
              << "    class: " << message
              << std::endl; 
}

void TOKEN::getString(CChar* cChar, std::string& data)
{
    switch(cChar->eCChar)
    {
        case(CChar::ANY_MEMBER):
            data.push_back(cChar->uCChar.sAnyMember.element);
            break;
        case(CChar::ESCAPE_SEQUENCE):
            getString(cChar->uCChar.sEscapeSequence.escapeSequence, data);
            break;
        
        default:
            error("CChar");
            break;
    }
}

void TOKEN::getString(CCharSequence* cCharSequence, std::string& data)
{
    switch(cCharSequence->eCCharSequence)
    {
        case(CCharSequence::C_CHAR):
            getString(cCharSequence->uCCharSequence.sCChar.cChar, data);
            break;
        case(CCharSequence::C_CHAR_SEQUENCE_C_CHAR):
            getString(cCharSequence->uCCharSequence.sCCharSequenceCChar.cCharSequence, data);
            getString(cCharSequence->uCCharSequence.sCCharSequenceCChar.cChar, data);
            break;

        default:
            error("CCharSequence");
            break;
    }
}

void TOKEN::getString(CharacterConstant* characterConstant, std::string& data)
{
    switch(characterConstant->eCharacterConstant)
    {
        case(CharacterConstant::C_CHAR_SEQUENCE):
            data.push_back('\'');
            getString(characterConstant->uCharacterConstant.sCCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::L_C_CHAR_SEQUENCE):
            data.push_back('L');
            data.push_back('\'');
            getString(characterConstant->uCharacterConstant.s_L_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::u_C_CHAR_SEQUENCE):
            data.push_back('u');
            data.push_back('\'');
            getString(characterConstant->uCharacterConstant.s_u_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;
        case(CharacterConstant::U_C_CHAR_SEQUENCE):
            data.push_back('U');
            data.push_back('\'');
            getString(characterConstant->uCharacterConstant.s_U_CCharSequence.cCharSequence, data);
            data.push_back('\'');
            break;

        default:
            error("CharacterConstant");
            break;
    }
}

void TOKEN::getString(ControlLine* controlLine, std::string& data)
{
    data.push_back('#');

    switch(controlLine->eControlLine)
    {
        case(ControlLine::INCLUDE_PP_TOKENS_NEW_LINE):
            data += "include";
            getString(controlLine->uControlLine.sIncludePPTokensNewLine.ppTokens, data);
            getString(controlLine->uControlLine.sIncludePPTokensNewLine.newLine, data);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_REPLACEMENT_LIST_NEW_LINE):
            data += "define";
            getString(controlLine->uControlLine.sDefineIdentifierReplacementListNewLine.identifier, data);
            getString(controlLine->uControlLine.sDefineIdentifierReplacementListNewLine.replacementList, data);
            getString(controlLine->uControlLine.sDefineIdentifierReplacementListNewLine.newLine, data);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            data += "define";
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifier, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.lparen, data);
            if(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifierList)
                getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.identifierList, data);
            data.push_back(')');
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.replacementList, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListRparenReplacementListNewLine.newLine, data);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            data += "define";
            getString(controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.identifier, data);
            data += "...)";
            getString(controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.replacementList, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenDotdotdotRparenReplacementListNewLine.newLine, data);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LPAREN_IDENTIFIER_LIST_COMMA_DOTDOTDOT_RPAREN_REPLACEMENT_LIST_NEW_LINE):
            data += "define";
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifier, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.lparen, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.identifierList, data);
            data += ",...)";
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.replacementList, data);
            getString(controlLine->uControlLine.sDefineIdentifierLparenIdentifierListCommaDotdotdotRparenReplacementListNewLine.newLine, data);
            break;
        case(ControlLine::UNDEF_IDENTIFIER_NEW_LINE):
            data += "undef";
            getString(controlLine->uControlLine.sUndefIdentifierNewLine.identifier, data);
            getString(controlLine->uControlLine.sUndefIdentifierNewLine.newLine, data);
            break;
        case(ControlLine::LINE_PP_TOKENS_NEW_LINE):
            data += "line";
            getString(controlLine->uControlLine.sLinePPTokensNewLine.ppTokens, data);
            getString(controlLine->uControlLine.sLinePPTokensNewLine.newLine, data);
            break;
        case(ControlLine::ERROR_PP_TOKENS_NEW_LINE):
            data += "error";
            getString(controlLine->uControlLine.sErrorPPtokensNewLine.ppTokens, data);
            getString(controlLine->uControlLine.sErrorPPtokensNewLine.newLine, data);
            break;
        case(ControlLine::PRAGMA_PP_TOKENS_NEW_LINE):
            data += "pragma";
            getString(controlLine->uControlLine.sPragmaPPTokensNewLine.ppTokens, data);
            getString(controlLine->uControlLine.sPragmaPPTokensNewLine.newLine, data);
            break;
        case(ControlLine::NEW_LINE):
            getString(controlLine->uControlLine.sNewLine.newLine, data);
            break;
        
        default:
            error("ControlLine");
            break;
    }
}

void TOKEN::getString(Digit* digit, std::string& data)
{
    data.push_back(digit->element);
}

void TOKEN::getString(ElifGroup* elifGroup, std::string& data)
{
    data += "#elif";
    getString(elifGroup->ppTokens, data);
    getString(elifGroup->newLine, data);
    if(elifGroup->group)
        getString(elifGroup->group, data);
}

void TOKEN::getString(ElifGroups* elifGroups, std::string& data)
{
    switch(elifGroups->eElifGroups)
    {
        case(ElifGroups::ELIF_GROUP):
            getString(elifGroups->uElifGroups.sElifGroup.elifGroup, data);
            break;
        case(ElifGroups::ELIF_GROUPS_ELIF_GROUP):
            getString(elifGroups->uElifGroups.sElifGroupsElifGroup.elifGroups, data);
            getString(elifGroups->uElifGroups.sElifGroupsElifGroup.elifGroup, data);
            break;
        
        default:
            error("ElifGroups");
            break;
    }
}

void TOKEN::getString(ElseGroup* elseGroup, std::string& data)
{
    data += "#else";
    getString(elseGroup->newLine, data);
    if(elseGroup->group)
        getString(elseGroup->group, data);
}

void TOKEN::getString(EndifLine* endifLine, std::string& data)
{
    data += "#endif";
    getString(endifLine->newLine, data);
}

void TOKEN::getString(EncodingPrefix* encodingPrefix, std::string& data)
{
    data += encodingPrefix->element;
}

void TOKEN::getString(EscapeSequence* escapeSequence, std::string& data)
{
    switch(escapeSequence->eEscapeSequence)
    {
        case(EscapeSequence::SIMPLE_ESCAPE_SEQUENCE):
            getString(escapeSequence->uEscapeSequence.sSimpleEscapeSequence.simpleEscapeSequence, data);
            break;
        case(EscapeSequence::OCTAL_ESCAPE_SEQUENCE):
            getString(escapeSequence->uEscapeSequence.sOctalEscapeSequence.octalEscapeSequence, data);
            break;
        case(EscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE):
            getString(escapeSequence->uEscapeSequence.sHexadecimalEscapeSequence.hexadecimalEscapeSequence, data);
            break;
        case(EscapeSequence::UNIVERSAL_CHARACTER_NAME):
            getString(escapeSequence->uEscapeSequence.sUniversalCharacterName.universalCharacterName, data);
            break;
        
        default:
            error("EscapeSequence");
            break;
    }
}

void TOKEN::getString(Group* group, std::string& data)
{
    switch(group->eGroup)
    {
        case(Group::GROUP_PART):
            getString(group->uGroup.sGroupPart.groupPart, data);
            break;
        case(Group::GROUP_GROUP_PART):
            getString(group->uGroup.sGroupGroupPart.group, data);
            getString(group->uGroup.sGroupGroupPart.groupPart, data);
            break;
        
        default:
            error("Group");
            break;
    }
}

void TOKEN::getString(GroupPart* groupPart, std::string& data)
{
    switch(groupPart->eGroupPart)
    {
        case(GroupPart::IF_SECTION):
            getString(groupPart->uGroupPart.sIfSection.ifSection, data);
            break;
        case(GroupPart::CONTROL_LINE):
            getString(groupPart->uGroupPart.sControlLine.controlLine, data);
            break;
        case(GroupPart::TEXT_LINE):
            getString(groupPart->uGroupPart.sTextLine.textLine, data);
            break;
        case(GroupPart::NON_DIRECTIVE):
            data.push_back('#');
            getString(groupPart->uGroupPart.sNonDirective.nonDirective, data);
            break;
        
        default:
            error("GroupPart");
            break;
    }
}

void TOKEN::getString(HChar* hChar, std::string& data)
{
    data.push_back(hChar->element);
}

void TOKEN::getString(HCharSequence* hCharSequence, std::string& data)
{
    switch(hCharSequence->eHCharSequence)
    {
        case(HCharSequence::H_CHAR):
            getString(hCharSequence->uHCharSequence.sHChar.hChar, data);
            break;
        case(HCharSequence::H_CHAR_SEQUENCE_H_CHAR):
            getString(hCharSequence->uHCharSequence.sHCharSequenceHChar.hCharSequence, data);
            getString(hCharSequence->uHCharSequence.sHCharSequenceHChar.hChar, data);
            break;
        
        default:
            error("HCharSequence");
            break;
    }
}

void TOKEN::getString(HeaderName* headerName, std::string& data)
{
    switch(headerName->eHeaderName)
    {
        case(HeaderName::H_CHAR_SEQUENCE):
            data.push_back('<');
            getString(headerName->uHeaderName.sHCharSequence.hCharSequence, data);
            data.push_back('>');
            break;
        case(HeaderName::Q_CHAR_SEQUENCE):
            data.push_back('"');
            getString(headerName->uHeaderName.sQCharSequence.qCharSequence, data);
            data.push_back('"');
            break;
        
        default:
            error("HeaderName");
            break;
    }
}

void TOKEN::getString(HexadecimalDigit* hexadecimalDigit, std::string& data)
{
    data.push_back(hexadecimalDigit->element);
}

void TOKEN::getString(HexadecimalEscapeSequence* hexadecimalEscapeSequence, std::string& data)
{
    switch(hexadecimalEscapeSequence->eHexadecimalEscapeSequence)
    {
        case(HexadecimalEscapeSequence::HEXADECIMAL_DIGIT):
            data += "\\x";
            getString(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalDigit.hexadecimalDigit, data);
            break;
        case(HexadecimalEscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE_HEXADECIMAL_DIGIT):
            getString(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit.hexadecimalEscapeSequence, data);
            getString(hexadecimalEscapeSequence->uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit.hexadecimalDigit, data);
            break;
        
        default:
            error("HexadecimalDigit");
            break;
    }
}

void TOKEN::getString(HexQuad* hexQuad, std::string& data)
{
    getString(hexQuad->hexadecimalDigit, data);
    getString(hexQuad->hexadecimalDigit_1, data);
    getString(hexQuad->hexadecimalDigit_2, data);
    getString(hexQuad->hexadecimalDigit_3, data);
}

void TOKEN::getString(Identifier* identifier, std::string& data)
{
    switch(identifier->eIdentifier)
    {
        case(Identifier::IDENTIFIER_NONDIGIT):
            getString(identifier->uIdentifier.sIdentifierNondigit.identifierNondigit, data);
            break;
        case(Identifier::IDENTIFIER_IDENTIFIER_NONDIGIT):
            getString(identifier->uIdentifier.sIdentifierIdentifierNondigit.identifier, data);
            getString(identifier->uIdentifier.sIdentifierIdentifierNondigit.identifierNondigit, data);
            break;
        case(Identifier::IDENTIFIER_DIGIT):
            getString(identifier->uIdentifier.sIdentifierDigit.identifier, data);
            getString(identifier->uIdentifier.sIdentifierDigit.digit, data);
            break;

        default:
            error("Identifier");
            break;
    }
}

void TOKEN::getString(IdentifierList* identifierList, std::string& data)
{
    switch(identifierList->eIdentifierList)
    {
        case(IdentifierList::IDENTIFIER):
            getString(identifierList->uIdentifierList.sIdentifier.identifier, data);
            break;
        case(IdentifierList::IDENTIFIER_LIST_IDENTIFIER):
            getString(identifierList->uIdentifierList.sIdentifierListIdentifier.identifierList, data);
            data.push_back(',');
            getString(identifierList->uIdentifierList.sIdentifierListIdentifier.identifier, data);
            break;
        
        default:
            error("IdentifierList");
            break;
    }
}

void TOKEN::getString(IdentifierNondigit* identifierNondigit, std::string& data)
{
    switch(identifierNondigit->eIdentifierNondigit)
    {
        case(IdentifierNondigit::NONDIGIT):
            getString(identifierNondigit->uIdentifierNondigit.sNondigit.nondigit, data);
            break;
        case(IdentifierNondigit::UNIVERSAL_CHARACTER_NAME):
            getString(identifierNondigit->uIdentifierNondigit.sUniversalCharacterName.universalCharacterName, data);
            break;
        
        default:
            error("IdentifierNondigit");
            break;
    }
}

void TOKEN::getString(IfGroup* ifGroup, std::string& data)
{
    data.push_back('#');

    switch(ifGroup->eIfGroup)
    {
        case(IfGroup::IF_PP_TOKENS_NEW_LINE_GROUP):
            data += "if";
            getString(ifGroup->uIfGroup.sIfPPTokensNewLineGroup.ppTokens, data);
            getString(ifGroup->uIfGroup.sIfPPTokensNewLineGroup.newLine, data);
            if(ifGroup->uIfGroup.sIfPPTokensNewLineGroup.group)
                getString(ifGroup->uIfGroup.sIfPPTokensNewLineGroup.group, data);
            break;
        case(IfGroup::IFDEF_IDENTIFIER_NEW_LINE_GROUP):
            data += "ifdef";
            getString(ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.identifier, data);
            getString(ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.newLine, data);
            if(ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.group)
                getString(ifGroup->uIfGroup.sIfdefIdentifierNewLineGroup.group, data);
            break;
        case(IfGroup::IFNDEF_IDENTIFIER_NEW_LINE_GROUP):
            data += "ifndef";
            getString(ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.identifier, data);
            getString(ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.newLine, data);
            if(ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.group)
                getString(ifGroup->uIfGroup.sIfndefIdentifierNewLineGroup.group, data);
            break;
        
        default:
            error("IfGroup");
            break;
    }
}

void TOKEN::getString(IfSection* ifSection, std::string& data)
{
    getString(ifSection->ifGroup, data);
    if(ifSection->elifGroups)
        getString(ifSection->elifGroups, data);
    if(ifSection->elseGroup)
        getString(ifSection->elseGroup, data);
    getString(ifSection->endifLine, data);
}

void TOKEN::getString(Lparen* lparen, std::string& data)
{
    data.push_back('(');
}

void TOKEN::getString(NewLine* newLine, std::string& data)
{
    data.push_back('\n');
}

void TOKEN::getString(Nondigit* nondigit, std::string& data)
{
    data.push_back(nondigit->element);
}

void TOKEN::getString(NonDirective* nonDirective, std::string& data)
{
    getString(nonDirective->ppTokens, data);
    getString(nonDirective->newLine, data);
}

void TOKEN::getString(OctalDigit* octalDigit, std::string& data)
{
    data.push_back(octalDigit->element);
}

void TOKEN::getString(OctalEscapeSequence* octalEscapeSequence, std::string& data)
{
    data.push_back('\\');

    switch(octalEscapeSequence->eOctalEscapeSequence)
    {
        case(OctalEscapeSequence::OCTAL_DIGIT):
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigit.octalDigit, data);
            break;
        case(OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT):
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigit.octalDigit, data);
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigit.octalDigit_1, data);
            break;
        case(OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT_OCTAL_DIGIT):
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit, data);
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit_1, data);
            getString(octalEscapeSequence->uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit.octalDigit_2, data);
            break;
        
        default:
            error("OctalEscapeSequence");
            break;
    }
}

void TOKEN::getString(PPNumber* ppNumber, std::string& data)
{
    switch(ppNumber->ePPNumber)
    {
        case(PPNumber::DIGIT):
            getString(ppNumber->uPPNumber.sDigit.digit, data);
            break;
        case(PPNumber::DOT_DIGIT):
            data.push_back('.');
            getString(ppNumber->uPPNumber.sDotDigit.digit, data);
            break;
        case(PPNumber::PP_NUMBER_DIGIT):
            getString(ppNumber->uPPNumber.sPPNumberDigit.ppNumber, data);
            getString(ppNumber->uPPNumber.sPPNumberDigit.digit, data);
            break;
        case(PPNumber::PP_NUMBER_IDENTIFIER_NONDIGIT):
            getString(ppNumber->uPPNumber.sPPNumberIdentifierNondigit.ppNumber, data);
            getString(ppNumber->uPPNumber.sPPNumberIdentifierNondigit.identifierNondigit, data);
            break;
        case(PPNumber::PP_NUMBER_e_SIGN):
            getString(ppNumber->uPPNumber.sPPNumber_e_Sign.ppNumber, data);
            data.push_back('e');
            getString(ppNumber->uPPNumber.sPPNumber_e_Sign.sign, data);
            break;
        case(PPNumber::PP_NUMBER_E_SIGN):
            getString(ppNumber->uPPNumber.sPPNumber_E_Sign.ppNumber, data);
            data.push_back('E');
            getString(ppNumber->uPPNumber.sPPNumber_E_Sign.sign, data);
            break;
        case(PPNumber::PP_NUMBER_p_SIGN):
            getString(ppNumber->uPPNumber.sPPNumber_p_Sign.ppNumber, data);
            data.push_back('p');
            getString(ppNumber->uPPNumber.sPPNumber_p_Sign.sign, data);
            break;
        case(PPNumber::PP_NUMBER_P_SIGN):
            getString(ppNumber->uPPNumber.sPPNumber_P_Sign.ppNumber, data);
            data.push_back('P');
            getString(ppNumber->uPPNumber.sPPNumber_P_Sign.sign, data);
            break;
        case(PPNumber::PP_NUMBER_DOT):
            getString(ppNumber->uPPNumber.sPPNumberDot.ppNumber, data);
            data.push_back('.');
            break;

        default:
            error("PPNumber");
            break;
    }
}

void TOKEN::getString(PPTokens* ppTokens, std::string& data)
{
    switch(ppTokens->ePPTokens)
    {
        case(PPTokens::PREPROCESSING_TOKEN):
            getString(ppTokens->uPPTokens.sPreprocessingToken.preprocessingToken, data);
            break;
        case(PPTokens::PP_TOKENS_PREPROCESSING_TOKEN):
            getString(ppTokens->uPPTokens.sPPTokensPreprocessingToken.ppTokens, data);
            getString(ppTokens->uPPTokens.sPPTokensPreprocessingToken.preprocessingToken, data);
            break;
        
        default:
            error("PPTokens");
            break;
    }
}

void TOKEN::getString(PreprocessingFile* preprocessingFile, std::string& data)
{
    if(preprocessingFile->group)
        getString(preprocessingFile->group, data);
}

void TOKEN::getString(PreprocessingToken* preprocessingToken, std::string& data)
{
    switch(preprocessingToken->ePreprocessingToken)
    {
        case(PreprocessingToken::HEADER_NAME):
            getString(preprocessingToken->uPreprocessingToken.sHeaderName.headerName, data);
            break;
        case(PreprocessingToken::IDENTIFIER):
            getString(preprocessingToken->uPreprocessingToken.sIdentifier.identifier, data);
            break;
        case(PreprocessingToken::PP_NUMBER):
            getString(preprocessingToken->uPreprocessingToken.sPPNumber.ppNumber, data);
            break;
        case(PreprocessingToken::CHARACTER_CONSTANT):
            getString(preprocessingToken->uPreprocessingToken.sCharacterConstant.characterConstant, data);
            break;
        case(PreprocessingToken::STRING_LITERAL):
            getString(preprocessingToken->uPreprocessingToken.sStringLiteral.stringLiteral, data);
            break;
        case(PreprocessingToken::PUNCTUATOR):
            getString(preprocessingToken->uPreprocessingToken.sPunctuator.punctuator, data);
            break;
        case(PreprocessingToken::OTHER):
            data.push_back(preprocessingToken->uPreprocessingToken.sOther.element);
            break;

        default:
            error("PreprocessingToken");
            break;
    }
}

void TOKEN::getString(Punctuator* punctuator, std::string& data)
{
    data += punctuator->element;
}

void TOKEN::getString(QChar* qChar, std::string& data)
{
    data.push_back(qChar->element);
}

void TOKEN::getString(QCharSequence* qCharSequence, std::string& data)
{
    switch(qCharSequence->eQCharSequence)
    {
        case(QCharSequence::Q_CHAR):
            getString(qCharSequence->uQCharSequence.sQChar.qChar, data);
            break;
        case(QCharSequence::Q_CHAR_SEQUENCE_Q_CHAR):
            getString(qCharSequence->uQCharSequence.sQCharSequenceQChar.qCharSequence, data);
            getString(qCharSequence->uQCharSequence.sQCharSequenceQChar.qChar, data);
            break;
        
        default:
            error("QCharSequence");
    }
}

void TOKEN::getString(ReplacementList* replacementList, std::string& data)
{
    if(replacementList->ppTokens)
        getString(replacementList->ppTokens, data);
}

void TOKEN::getString(SChar* sChar, std::string& data)
{
    switch(sChar->eSChar)
    {
        case(SChar::ANY_MEMBER):
            data.push_back(sChar->uSChar.sAnyMember.element);
            break;
        case(SChar::ESCAPE_SEQUENCE):
            getString(sChar->uSChar.sEscapeSequence.escapeSequence, data);
            break;
        
        default:
            error("SChar");
            break;
    }
}

void TOKEN::getString(SCharSequence* sCharSequence, std::string& data)
{
    switch(sCharSequence->eSCharSequence)
    {
        case(SCharSequence::S_CHAR):
            getString(sCharSequence->uSCharSequence.sSChar.sChar, data);
            break;
        case(SCharSequence::S_CHAR_SEQUENCE_S_CHAR):
            getString(sCharSequence->uSCharSequence.sSCharSequenceSChar.sCharSequence, data);
            getString(sCharSequence->uSCharSequence.sSCharSequenceSChar.sChar, data);
            break;

        default:
            error("SCharSequence");
            break;
    }
}

void TOKEN::getString(Sign* sign, std::string& data)
{
    data.push_back(sign->element);
}

void TOKEN::getString(SimpleEscapeSequence* simpleEscapeSequence, std::string& data)
{
    data.push_back('\\');
    data.push_back(simpleEscapeSequence->element);
}

void TOKEN::getString(StringLiteral* stringLiteral, std::string& data)
{
    if(stringLiteral->encodingPrefix)
        getString(stringLiteral->encodingPrefix, data);
    data.push_back('"');
    if(stringLiteral->sCharSequence)
        getString(stringLiteral->sCharSequence, data);
    data.push_back('"');
}

void TOKEN::getString(TextLine* textLine, std::string& data)
{
    if(textLine->ppTokens)
        getString(textLine->ppTokens, data);
    getString(textLine->newLine, data);
}

void TOKEN::getString(UniversalCharacterName* universalCharacterName, std::string& data)
{
    switch(universalCharacterName->eUniversalCharacterName)
    {
        case(UniversalCharacterName::HEX_QUAD):
            data += "\\u";
            getString(universalCharacterName->uUniversalCharacterName.sHexQuad.hexQuad, data);
            break;
        case(UniversalCharacterName::HEX_QUAD_HEX_QUAD):
            data += "\\U";
            getString(universalCharacterName->uUniversalCharacterName.sHexQuadHexQuad.hexQuad, data);
            getString(universalCharacterName->uUniversalCharacterName.sHexQuadHexQuad.hexQuad_1, data);
            break;
        
        default:
            error("UniversalCharacterName");
            break;
    }
}