#include "pp_tokenizer.hpp"
#include "preprocessor.hpp"
#include <string>
#include <utility>
#include <iostream>

PPTokenizer::PPTokenizer(Preprocessor* pp):
    mIdx(0),
    mPP(pp),
    mIsValid(true)
{
}

bool PPTokenizer::execute()
{
    while(mIdx < mPP->mSrc.size() && mIsValid)
    {
        deleteComment();

        char c = mPP->mSrc[mIdx];
        if(c != ' ' &&
           c != '\t' &&
           c != '\v' &&
           c != '\f')
        {
            auto idx = mIdx;
            PreprocessingToken* preprocessingToken = getPreprocessingToken();
            if(preprocessingToken)
                mPP->mPPTokens.emplace_back(preprocessingToken, idx);
            else
                mIsValid = false;
        }
        else
            mIdx++;
    }

    for(auto&& e : mPP->mPPTokens)
    {
        std::cout << "token:\n    "
                  << "enum: "
                  << static_cast<int>(e.first->ePreprocessingToken)
                  << std::endl;
        if(e.first->ePreprocessingToken == PreprocessingToken::IDENTIFIER ||
           e.first->ePreprocessingToken == PreprocessingToken::PUNCTUATOR)
        {
            std::string data;
            getString(e.first, data);
            std::cout << "    data: " << data << std::endl;
        }
    }
        
    return mIsValid;
}

void PPTokenizer::deleteComment()
{
    // // comment
    if(isMatched(mIdx, '/') &&
       isMatched(mIdx + 1, '/'))
    {
        mIdx += 2;
        while(mIdx < mPP->mSrc.size())
        {
            if(mPP->mSrc[mIdx] == '\n')
                break;
            else
                mIdx++;
        }
    }
    // /* */ comment
    else if(isMatched(mIdx, '/') &&
            isMatched(mIdx + 1, '*'))
    {
        auto idx = mIdx;
        bool isValid = false;

        mIdx += 2;

        while(mIdx + 1 < mPP->mSrc.size())
        {
            if(mPP->mSrc[mIdx] == '*' &&
               mPP->mSrc[mIdx + 1] == '/')
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

CChar* PPTokenizer::getCChar()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    CChar cChar;
    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c != '\'' &&
       c != '\\' &&
       c != '\n')
    {
        mIdx++;

        cChar.eCChar = CChar::ANY_MEMBER;
        cChar.uCChar.sAnyMember.element = c;
    }
    else if(cChar.uCChar.sEscapeSequence.escapeSequence = getEscapeSequence())
        cChar.eCChar = CChar::ESCAPE_SEQUENCE;
    else
        isValid = false;
    
    if(isValid)
        return new CChar(cChar);
    else
        return nullptr;
}

CCharSequence* PPTokenizer::getCCharSequence(CCharSequence* bef)
{
    CCharSequence cCharSequence;
    bool isValid = true;

    if(!bef)
    {
        if(cCharSequence.uCCharSequence.sCChar.cChar = getCChar())
            cCharSequence.eCCharSequence = CCharSequence::C_CHAR;
        else
            isValid = false;
    }
    else
    {
        if(cCharSequence.uCCharSequence.sCCharSequenceCChar.cChar = getCChar())
        {
            cCharSequence.eCCharSequence = CCharSequence::C_CHAR_SEQUENCE_C_CHAR;
            cCharSequence.uCCharSequence.sCCharSequenceCChar.cCharSequence = bef;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        CCharSequence* retVal = new CCharSequence(cCharSequence);
        CCharSequence* aft = getCCharSequence(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

CharacterConstant* PPTokenizer::getCharacterConstant()
{
    CharacterConstant characterConstant;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, '\''))
    {
        mIdx++;

        if(characterConstant.uCharacterConstant.sCCharSequence.cCharSequence = getCCharSequence())
            characterConstant.eCharacterConstant = CharacterConstant::C_CHAR_SEQUENCE;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, 'L') &&
            isMatched(mIdx + 1, '\''))
    {
        mIdx += 2;

        if(characterConstant.uCharacterConstant.s_L_CCharSequence.cCharSequence = getCCharSequence())
            characterConstant.eCharacterConstant = CharacterConstant::L_C_CHAR_SEQUENCE;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, 'u') &&
            isMatched(mIdx + 1, '\''))
    {
        mIdx += 2;

        if(characterConstant.uCharacterConstant.s_u_CCharSequence.cCharSequence = getCCharSequence())
            characterConstant.eCharacterConstant = CharacterConstant::u_C_CHAR_SEQUENCE;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, 'U') &&
            isMatched(mIdx + 1, '\''))
    {
        mIdx += 2;

        if(characterConstant.uCharacterConstant.s_U_CCharSequence.cCharSequence = getCCharSequence())
            characterConstant.eCharacterConstant = CharacterConstant::U_C_CHAR_SEQUENCE;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
    {
        if(isMatched(mIdx, '\''))
            mIdx++;
        else
            isValid = false;
    }

    if(isValid)
        return new CharacterConstant(characterConstant);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Digit* PPTokenizer::getDigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c >= '0' && c <= '9')
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        Digit* digit = new Digit();
        digit->element = c;
        return digit;
    }
    else
        return nullptr;
}

EncodingPrefix* PPTokenizer::getEncodingPrefix()
{
    EncodingPrefix encodingPrefix;
    bool isValid = true;

    if(isMatched(mIdx, 'u') &&
       isMatched(mIdx + 1, '8'))
        encodingPrefix.element = "u8";
    else if(isMatched(mIdx, 'u'))
        encodingPrefix.element = "u";
    else if(isMatched(mIdx, 'U'))
        encodingPrefix.element = "U";
    else if(isMatched(mIdx, 'L'))
        encodingPrefix.element = "L";
    else
        isValid = false;

    if(isValid)
    {
        mIdx += encodingPrefix.element.size();
        return new EncodingPrefix(encodingPrefix);
    }
    else
        return nullptr;
}

EscapeSequence* PPTokenizer::getEscapeSequence()
{
    EscapeSequence escapeSequence;
    bool isValid = true;

    if(escapeSequence.uEscapeSequence.sSimpleEscapeSequence.simpleEscapeSequence = getSimpleEscapeSequence())
        escapeSequence.eEscapeSequence = EscapeSequence::SIMPLE_ESCAPE_SEQUENCE;
    else if(escapeSequence.uEscapeSequence.sOctalEscapeSequence.octalEscapeSequence = getOctalEscapeSequence())
        escapeSequence.eEscapeSequence = EscapeSequence::OCTAL_ESCAPE_SEQUENCE;
    else if(escapeSequence.uEscapeSequence.sHexadecimalEscapeSequence.hexadecimalEscapeSequence = getHexadecimalEscapeSequence())
        escapeSequence.eEscapeSequence = EscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE;
    else if(escapeSequence.uEscapeSequence.sUniversalCharacterName.universalCharacterName = getUniversalCharacterName())
        escapeSequence.eEscapeSequence = EscapeSequence::UNIVERSAL_CHARACTER_NAME;
    else
        isValid = false;
    
    if(isValid)
        return new EscapeSequence(escapeSequence);
    else
        return nullptr;
}

HChar* PPTokenizer::getHChar()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c != '\n' &&
       c != '>')
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        HChar* hChar = new HChar();
        hChar->element = c;
        return hChar;
    }
    else
        return nullptr;
}

HCharSequence* PPTokenizer::getHCharSequence(HCharSequence* bef)
{
    HCharSequence hCharSequence;
    bool isValid = true;

    if(!bef)
    {
        if(hCharSequence.uHCharSequence.sHChar.hChar = getHChar())
            hCharSequence.eHCharSequence = HCharSequence::H_CHAR;
        else
            isValid = false;
    }
    else
    {
        if(hCharSequence.uHCharSequence.sHCharSequenceHChar.hChar = getHChar())
        {
            hCharSequence.eHCharSequence = HCharSequence::H_CHAR_SEQUENCE_H_CHAR;
            hCharSequence.uHCharSequence.sHCharSequenceHChar.hCharSequence = bef;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        HCharSequence* retVal = new HCharSequence(hCharSequence);
        HCharSequence* aft = getHCharSequence(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

HeaderName* PPTokenizer::getHeaderName()
{
    if(!isHeader())
        return nullptr;
    
    HeaderName headerName;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, '<'))
    {
        mIdx++;

        if(headerName.uHeaderName.sHCharSequence.hCharSequence = getHCharSequence())
        {
            headerName.eHeaderName = HeaderName::H_CHAR_SEQUENCE;
            if(isMatched(mIdx, '>'))
                mIdx++;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else if(isMatched(mIdx, '"'))
    {
        mIdx++;

        if(headerName.uHeaderName.sQCharSequence.qCharSequence = getQCharSequence())
        {
            headerName.eHeaderName = HeaderName::Q_CHAR_SEQUENCE;
            if(isMatched(mIdx, '"'))
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
        return new HeaderName(headerName);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HexadecimalDigit* PPTokenizer::getHexadecimalDigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;
    
    bool isValid = true;

    char c =mPP->mSrc[mIdx];
    if((c >= '0' && c <= '9') ||
       (c >= 'a' && c <= 'f') ||
       (c >= 'A' && c <= 'F'))
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        HexadecimalDigit* hexadecimalDigit = new HexadecimalDigit();
        hexadecimalDigit->element = c;
        return hexadecimalDigit;
    }
    else
        return nullptr;
}

HexadecimalEscapeSequence* PPTokenizer::getHexadecimalEscapeSequence(HexadecimalEscapeSequence* bef)
{
    HexadecimalEscapeSequence hexadecimalEscapeSequence;
    bool isValid = true;
    auto idx = mIdx;

    if(!bef)
    {
        if(isMatched(mIdx, '\\') &&
           isMatched(mIdx + 1, 'x'))
        {
            mIdx += 2;

            if(hexadecimalEscapeSequence.uHexadecimalEscapeSequence.sHexadecimalDigit.hexadecimalDigit = getHexadecimalDigit())
                hexadecimalEscapeSequence.eHexadecimalEscapeSequence = HexadecimalEscapeSequence::HEXADECIMAL_DIGIT;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        HexadecimalDigit* hexadecimalDigit = getHexadecimalDigit();
        if(hexadecimalDigit)
        {
            hexadecimalEscapeSequence.uHexadecimalEscapeSequence.sHexadecimalEscapeSequenceHexadecimalDigit = {bef, hexadecimalDigit};
            hexadecimalEscapeSequence.eHexadecimalEscapeSequence = HexadecimalEscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE_HEXADECIMAL_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        HexadecimalEscapeSequence* retVal = new HexadecimalEscapeSequence(hexadecimalEscapeSequence);
        HexadecimalEscapeSequence* aft = getHexadecimalEscapeSequence(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

HexQuad* PPTokenizer::getHexQuad()
{
    HexQuad hexQuad;
    bool isValid = false;
    auto idx = mIdx;

    if(hexQuad.hexadecimalDigit = getHexadecimalDigit())
    {
        if(hexQuad.hexadecimalDigit_1 = getHexadecimalDigit())
        {
            if(hexQuad.hexadecimalDigit_2 = getHexadecimalDigit())
            {
                if(hexQuad.hexadecimalDigit_3 = getHexadecimalDigit())
                    ;
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
        return new HexQuad(hexQuad);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

Identifier* PPTokenizer::getIdentifier(Identifier* bef)
{
    Identifier identifier;
    auto isValid = true;

    if(!bef)
    {
        if(identifier.uIdentifier.sIdentifierNondigit.identifierNondigit = getIdentifierNondigit())
            identifier.eIdentifier = Identifier::IDENTIFIER_NONDIGIT;
        else
            isValid = false;
    }
    else
    {
        if(identifier.uIdentifier.sIdentifierIdentifierNondigit.identifierNondigit = getIdentifierNondigit())
        {
            identifier.uIdentifier.sIdentifierIdentifierNondigit.identifier = bef;
            identifier.eIdentifier = Identifier::IDENTIFIER_IDENTIFIER_NONDIGIT;
        }
        else if(identifier.uIdentifier.sIdentifierDigit.digit = getDigit())
        {
            identifier.uIdentifier.sIdentifierDigit.identifier = bef;
            identifier.eIdentifier = Identifier::IDENTIFIER_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        Identifier* retVal = new Identifier(identifier);
        Identifier* aft = getIdentifier(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

IdentifierNondigit* PPTokenizer::getIdentifierNondigit()
{
    IdentifierNondigit identifierNondigit;
    bool isValid = true;

    if(identifierNondigit.uIdentifierNondigit.sNondigit.nondigit = getNondigit())
        identifierNondigit.eIdentifierNondigit = IdentifierNondigit::NONDIGIT;
    else if(identifierNondigit.uIdentifierNondigit.sUniversalCharacterName.universalCharacterName = getUniversalCharacterName())
        identifierNondigit.eIdentifierNondigit = IdentifierNondigit::UNIVERSAL_CHARACTER_NAME;
    else
        isValid = false;

    if(isValid)
        return new IdentifierNondigit(identifierNondigit);
    else
        return nullptr;
}

Nondigit* PPTokenizer::getNondigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if((c >= 'a' && c <= 'z') ||
       (c >= 'A' && c <= 'Z') ||
       c == '_')
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        Nondigit* nondigit = new Nondigit();
        nondigit->element = c;
        return nondigit;
    }
    else
        return nullptr;
}

OctalDigit* PPTokenizer::getOctalDigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c >= '0' && c <= 7)
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        OctalDigit* octalDigit = new OctalDigit();
        octalDigit->element = c;
        return octalDigit;
    }
    else
        return nullptr;
}

OctalEscapeSequence* PPTokenizer::getOctalEscapeSequence()
{
    OctalEscapeSequence octalEscapeSequence;
    bool isValid = true;

    OctalDigit* octalDigit = getOctalDigit();
    if(octalDigit)
    {
        OctalDigit* octalDigit_1 = getOctalDigit();
        if(octalDigit_1)
        {
            OctalDigit* octalDigit_2 = getOctalDigit();
            if(octalDigit_2)
            {
                octalEscapeSequence.eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT_OCTAL_DIGIT;
                octalEscapeSequence.uOctalEscapeSequence.sOctalDigitOctalDigitOctalDigit = {octalDigit, octalDigit_1, octalDigit_2};
            }
            else
            {
                octalEscapeSequence.eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT_OCTAL_DIGIT;
                octalEscapeSequence.uOctalEscapeSequence.sOctalDigitOctalDigit = {octalDigit, octalDigit_1};
            }
        }
        else
        {
            octalEscapeSequence.eOctalEscapeSequence = OctalEscapeSequence::OCTAL_DIGIT;
            octalEscapeSequence.uOctalEscapeSequence.sOctalDigit = {octalDigit};
        }
    }
    else
        isValid = false;

    if(isValid)
        return new OctalEscapeSequence(octalEscapeSequence);
    else
        return nullptr;
}

PPNumber* PPTokenizer::getPPNumber(PPNumber* bef)
{
    PPNumber ppNumber;
    bool isValid = true;
    auto idx = mIdx;

    if(!bef)
    {
        if(ppNumber.uPPNumber.sDigit.digit = getDigit())
            ppNumber.ePPNumber = PPNumber::DIGIT;
        else if(isMatched(mIdx, '.'))
        {
            mIdx++;

            if(ppNumber.uPPNumber.sDotDigit.digit = getDigit())
                ppNumber.ePPNumber = PPNumber::DOT_DIGIT;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        if(ppNumber.uPPNumber.sPPNumberDigit.digit = getDigit())
        {
            ppNumber.uPPNumber.sPPNumberDigit.ppNumber = bef;
            ppNumber.ePPNumber = PPNumber::PP_NUMBER_DIGIT;
        }
        else if(isMatched(mIdx, '.'))
        {
            mIdx++;
            ppNumber.ePPNumber = PPNumber::PP_NUMBER_DOT;
        }
        else
        {
            auto isCreated = false;

            if(isMatched(mIdx, 'e'))
            {
                mIdx++;

                if(ppNumber.uPPNumber.sPPNumber_e_Sign.sign = getSign())
                {
                    isCreated = true;

                    ppNumber.uPPNumber.sPPNumber_e_Sign.ppNumber = bef;
                    ppNumber.ePPNumber = PPNumber::PP_NUMBER_e_SIGN;
                }
                else
                    mIdx--;
            }
            else if(isMatched(mIdx, 'E'))
            {
                mIdx++;

                if(ppNumber.uPPNumber.sPPNumber_E_Sign.sign = getSign())
                {
                    isCreated = true;
                
                    ppNumber.uPPNumber.sPPNumber_E_Sign.ppNumber = bef;
                    ppNumber.ePPNumber = PPNumber::PP_NUMBER_E_SIGN;
                }
                else
                    mIdx--;
            }
            else if(isMatched(mIdx, 'p'))
            {
                mIdx++;

                if(ppNumber.uPPNumber.sPPNumber_p_Sign.sign = getSign())
                {
                    isCreated = true;

                    ppNumber.uPPNumber.sPPNumber_p_Sign.ppNumber = bef;
                    ppNumber.ePPNumber = PPNumber::PP_NUMBER_p_SIGN;
                }
                else
                    mIdx--;
            }
            else if(isMatched(mIdx, 'P'))
            {
                mIdx++;

                if(ppNumber.uPPNumber.sPPNumber_P_Sign.sign = getSign())
                {
                    isCreated = true;

                    ppNumber.uPPNumber.sPPNumber_P_Sign.ppNumber = bef;
                    ppNumber.ePPNumber = PPNumber::PP_NUMBER_P_SIGN;
                }
                else
                    mIdx--;
            }
            
            if(!isCreated)
            {
                if(ppNumber.uPPNumber.sPPNumberIdentifierNondigit.identifierNondigit = getIdentifierNondigit())
                {
                    ppNumber.uPPNumber.sPPNumberIdentifierNondigit.ppNumber = bef;
                    ppNumber.ePPNumber = PPNumber::PP_NUMBER_IDENTIFIER_NONDIGIT;
                }
                else
                    isValid = false;
            }
        }
    }

    if(isValid)
    {
        PPNumber* retVal = new PPNumber(ppNumber);
        PPNumber* aft = getPPNumber(retVal);
        if(aft)
            return aft;
        else
            return nullptr;
    }
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

PreprocessingToken* PPTokenizer::getPreprocessingToken()
{
    PreprocessingToken preprocessingToken;

    if(preprocessingToken.uPreprocessingToken.sHeaderName.headerName = getHeaderName())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::HEADER_NAME;
    else if(preprocessingToken.uPreprocessingToken.sCharacterConstant.characterConstant = getCharacterConstant())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::CHARACTER_CONSTANT;
    else if(preprocessingToken.uPreprocessingToken.sStringLiteral.stringLiteral = getStringLiteral())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::STRING_LITERAL;
    else if(preprocessingToken.uPreprocessingToken.sPPNumber.ppNumber = getPPNumber())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::PP_NUMBER;
    else if(preprocessingToken.uPreprocessingToken.sIdentifier.identifier = getIdentifier())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::IDENTIFIER;
    else if(preprocessingToken.uPreprocessingToken.sPunctuator.punctuator = getPunctuator())
        preprocessingToken.ePreprocessingToken = PreprocessingToken::PUNCTUATOR;
    else
    {
        preprocessingToken.ePreprocessingToken = PreprocessingToken::OTHER;
        preprocessingToken.uPreprocessingToken.sOther.element = mPP->mSrc[mIdx++];
    }
    return new PreprocessingToken(preprocessingToken);
}

Punctuator* PPTokenizer::getPunctuator()
{
    Punctuator punctuator;
    
    if(mIdx + 3 < mPP->mSrc.size())
    {
        std::string sub = mPP->mSrc.substr(mIdx, 4);
        if(sub == "%:%:")
            punctuator.element = std::move(sub);
    }
    if(mIdx + 2 < mPP->mSrc.size() &&
       punctuator.element.empty())
    {
        std::string sub = mPP->mSrc.substr(mIdx, 3);
        if(sub == "..." ||
           sub == "<<=" ||
           sub == ">>=")
            punctuator.element = std::move(sub);
    }
    if(mIdx + 1 < mPP->mSrc.size() &&
       punctuator.element.empty())
    {
        std::string sub = mPP->mSrc.substr(mIdx, 2);
        if(sub == "->" ||
           sub == "++" ||
           sub == "--" ||
           sub == "<<" ||
           sub == ">>" ||
           sub == "<=" ||
           sub == ">=" ||
           sub == "==" ||
           sub == "!=" ||
           sub == "&&" ||
           sub == "||" ||
           sub == "*=" ||
           sub == "/=" ||
           sub == "%=" ||
           sub == "+=" ||
           sub == "-=" ||
           sub == "&=" ||
           sub == "^=" ||
           sub == "|=" ||
           sub == "##" ||
           sub == "<:" ||
           sub == ":>" ||
           sub == "<%" ||
           sub == "%>" ||
           sub == "%:")
            punctuator.element = std::move(sub);
    }
    if(mIdx < mPP->mSrc.size() &&
       punctuator.element.empty())
    {
        std::string sub = mPP->mSrc.substr(mIdx, 1);
        if(sub == "[" ||
           sub == "]" ||
           sub == "(" ||
           sub == ")" ||
           sub == "{" ||
           sub == "}" ||
           sub == "." ||
           sub == "&" ||
           sub == "*" ||
           sub == "+" ||
           sub == "-" ||
           sub == "~" ||
           sub == "!" ||
           sub == "/" ||
           sub == "%" ||
           sub == "<" ||
           sub == ">" ||
           sub == "^" ||
           sub == "|" ||
           sub == "?" ||
           sub == ":" ||
           sub == ";" ||
           sub == "=" ||
           sub == "," ||
           sub == "#")
            punctuator.element = std::move(sub);
    }

    if(!punctuator.element.empty())
    {
        mIdx += punctuator.element.size();
        return new Punctuator(punctuator);
    }
    else
        return nullptr;
}

QChar* PPTokenizer::getQChar()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;
    
    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c != '\n' &&
       c != '"')
        mIdx++;
    else
        isValid = false;

    if(isValid)
    {
        QChar* qChar = new QChar();
        qChar->element = c;
        return qChar;
    }
    else
        return nullptr;
}

QCharSequence* PPTokenizer::getQCharSequence(QCharSequence* bef)
{
    QCharSequence qCharSequence;
    bool isValid = false;

    if(!bef)
    {
        if(qCharSequence.uQCharSequence.sQChar.qChar = getQChar())
            qCharSequence.eQCharSequence = QCharSequence::Q_CHAR;
        else
            isValid = false;
    }
    else
    {
        if(qCharSequence.uQCharSequence.sQCharSequenceQChar.qChar = getQChar())
        {
            qCharSequence.eQCharSequence = QCharSequence::Q_CHAR_SEQUENCE_Q_CHAR;
            qCharSequence.uQCharSequence.sQCharSequenceQChar.qCharSequence = bef;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        QCharSequence* retVal = new QCharSequence(qCharSequence);
        QCharSequence* aft = getQCharSequence(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

SChar* PPTokenizer::getSChar()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    SChar sChar;
    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c != '"' &&
       c != '\\' &&
       c != '\n')
    {
        mIdx++;

        sChar.eSChar = SChar::ANY_MEMBER;
        sChar.uSChar.sAnyMember = {c};
    }
    else if(sChar.uSChar.sEscapeSequence.escapeSequence = getEscapeSequence())
        sChar.eSChar = SChar::ESCAPE_SEQUENCE;
    else
        isValid = false;

    if(isValid)
        return new SChar(sChar);
    else
        return nullptr;
}

SCharSequence* PPTokenizer::getSCharSequence(SCharSequence* bef)
{
    SCharSequence sCharSequence;
    bool isValid = true;

    if(!bef)
    {
        if(sCharSequence.uSCharSequence.sSChar.sChar = getSChar())
            sCharSequence.eSCharSequence = SCharSequence::S_CHAR;
        else
            isValid = false;
    }
    else
    {
        SChar* sChar = getSChar();
        if(sChar)
        {
            sCharSequence.uSCharSequence.sSCharSequenceSChar = {bef, sChar};
            sCharSequence.eSCharSequence = SCharSequence::S_CHAR_SEQUENCE_S_CHAR;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        SCharSequence* retVal = new SCharSequence(sCharSequence);
        SCharSequence* aft = getSCharSequence(retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
        return nullptr;
}

Sign* PPTokenizer::getSign()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    bool isValid = true;

    char c = mPP->mSrc[mIdx];
    if(c == '+' ||
       c == '-')
        mIdx++;
    else
        isValid = false;
    
    if(isValid)
    {
        Sign* sign = new Sign();
        sign->element = c;
        return sign;
    }
    else
        return nullptr;
}

SimpleEscapeSequence* PPTokenizer::getSimpleEscapeSequence()
{
    if(mIdx + 1 >= mPP->mSrc.size())
        return nullptr;

    SimpleEscapeSequence simpleEscapeSequence;
    bool isValid = true;

    if(isMatched(mIdx, '\\'))
    {
        char c = mPP->mSrc[mIdx + 1];
        if(c == '\'' ||
           c == '"' ||
           c == '?' ||
           c == '\\' ||
           c == 'a' ||
           c == 'b' ||
           c == 'f' ||
           c == 'n' ||
           c == 'r' ||
           c == 't' ||
           c == 'v')
        {
            mIdx += 2;
            simpleEscapeSequence.element = c;          
        }
        else
           isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new SimpleEscapeSequence(simpleEscapeSequence);
    else
        return nullptr;
}

StringLiteral* PPTokenizer::getStringLiteral()
{
    StringLiteral stringLiteral;
    bool isValid = true;
    auto idx = mIdx;

    stringLiteral.encodingPrefix = getEncodingPrefix();
    
    if(isMatched(mIdx, '"'))
    {
        mIdx++;
        stringLiteral.sCharSequence = getSCharSequence();

        if(isMatched(mIdx, '"'))
            mIdx++;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new StringLiteral(stringLiteral);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

UniversalCharacterName* PPTokenizer::getUniversalCharacterName()
{
    UniversalCharacterName universalCharacterName;
    bool isValid = true;
    auto idx = mIdx;

    if(isMatched(mIdx, '\\') &&
       isMatched(mIdx + 1, 'u'))
    {
        mIdx += 2;

        if(universalCharacterName.uUniversalCharacterName.sHexQuad.hexQuad = getHexQuad())
            universalCharacterName.eUniversalCharacterName = UniversalCharacterName::HEX_QUAD;
        else
            isValid = false;
    }
    else if(isMatched(mIdx, '\\') &&
            isMatched(mIdx + 1, 'U'))
    {
        mIdx += 2;

        if(universalCharacterName.uUniversalCharacterName.sHexQuadHexQuad.hexQuad = getHexQuad())
        {
            if(universalCharacterName.uUniversalCharacterName.sHexQuadHexQuad.hexQuad_1 = getHexQuad())
                universalCharacterName.eUniversalCharacterName = UniversalCharacterName::HEX_QUAD_HEX_QUAD;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new UniversalCharacterName(universalCharacterName);
    else
    {
        mIdx = idx;
        return nullptr;
    }
}

void PPTokenizer::getString(Digit* digit, std::string& data) const
{
    data.push_back(digit->element);
}

void PPTokenizer::getString(HexadecimalDigit* hexadecimalDigit, std::string& data) const
{
    data.push_back(hexadecimalDigit->element);
}

void PPTokenizer::getString(HexQuad* hexQuad, std::string& data) const
{
    getString(hexQuad->hexadecimalDigit, data);
    getString(hexQuad->hexadecimalDigit_1, data);
    getString(hexQuad->hexadecimalDigit_2, data);
    getString(hexQuad->hexadecimalDigit_3, data);
}

void PPTokenizer::getString(Identifier* identifier, std::string& data) const
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
            getStringError("Identifier");
            break;
    }
}

void PPTokenizer::getString(IdentifierNondigit* identifierNondigit, std::string& data) const
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
            getStringError("IdentifierNondigit");
            break;
    }
}

void PPTokenizer::getString(Nondigit* nondigit, std::string& data) const
{
    data.push_back(nondigit->element);
}

void PPTokenizer::getString(PreprocessingToken* preprocessingToken, std::string& data) const
{
    switch(preprocessingToken->ePreprocessingToken)
    {
        case(PreprocessingToken::IDENTIFIER):
            getString(preprocessingToken->uPreprocessingToken.sIdentifier.identifier, data);
            break;
        case(PreprocessingToken::PUNCTUATOR):
            getString(preprocessingToken->uPreprocessingToken.sPunctuator.punctuator, data);
            break;
        
        default:
            getStringError("PreprocessingToken");
            break;
    }
}

void PPTokenizer::getString(Punctuator* punctuator, std::string& data) const
{
    data.append(punctuator->element);
}

void PPTokenizer::getString(UniversalCharacterName* universalCharacterName, std::string& data) const
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
            getStringError("UniversalCharacterName");
            break;
    }
}

void PPTokenizer::getStringError(const char* message) const
{
    std::cerr << "impl-error:"
              << "\n    class: "
              << message
              << std::endl;
}

bool PPTokenizer::isMatched(std::string::size_type idx, char c) const
{
    if(idx < mPP->mSrc.size())
        return mPP->mSrc[idx] == c;
    else
        return false;
}

bool PPTokenizer::isHeader() const
{
    if(mPP->mPPTokens.size() < 2)
        return false;
    
    if(mPP->mPPTokens[mPP->mPPTokens.size() - 2].first->ePreprocessingToken != PreprocessingToken::PUNCTUATOR ||
       mPP->mPPTokens[mPP->mPPTokens.size() - 1].first->ePreprocessingToken != PreprocessingToken::IDENTIFIER)
        return false;
    
    std::string puncStr, idenStr;
    getString(mPP->mPPTokens[mPP->mPPTokens.size() - 2].first, puncStr);
    getString(mPP->mPPTokens[mPP->mPPTokens.size() - 1].first, idenStr);

    if(puncStr != "#" ||
        idenStr != "include")
        return false;
    
    return true;
}