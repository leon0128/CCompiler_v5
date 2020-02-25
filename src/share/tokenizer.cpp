#include "tokenizer.hpp"
#include "preprocessing_token.hpp"
#include "symbol.hpp"
#include <array>
#include <iostream>

Tokenizer::Tokenizer(const std::string& src):
    mSrc(src),
    mIdx(0),
    mTokens()
{
}

void Tokenizer::execute()
{
    for(; mIdx < mSrc.size(); mIdx++)
    {
        if(mSrc.at(mIdx) != ' ')
        {
            auto idx = mIdx;
            PreprocessingToken_Symbol* preprocessingToken_Symbol = conPreprocessingToken_Symbol();
            addPreprocessingToken(preprocessingToken_Symbol, idx);
        }
    }

    for(auto&& e : mTokens)
        std::cout << "token: " << e->data << std::endl;
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

    mTokens.push_back(token);
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
        Identifier* identifier = conIdentifier();
        if(identifier)
        {
            preprocessingToken_symbol = new PreprocessingToken_Symbol();
            preprocessingToken_symbol->ePreprocessingToken = PreprocessingToken_Symbol::IDENTIFIER;
            preprocessingToken_symbol->uPreprocessingToken.sIdentifier = {identifier};
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

    if(isValid)
        return preprocessingToken_symbol;
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
       mTokens.size() <= 1)
        return nullptr;

    if(*mTokens.at(mTokens.size() - 1) != PreprocessingToken("include", PreprocessingToken::IDENTIFIER) ||
       *mTokens.at(mTokens.size() - 2) != PreprocessingToken("#", PreprocessingToken::PUNCTUATOR))
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
                ppNumber->uPpNumber.sPpNumberDot = {ppNumber};
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
        case(PreprocessingToken_Symbol::OTHER):
            process(preprocessingToken_symbol->uPreprocessingToken.sOther.other, data);
            break;

        default:
            std::cout << "tmp" << std::endl;
            break;
    }
}

void Tokenizer::process(Digit* digit, std::string& data) const
{
    data.push_back(digit->element);
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

void Tokenizer::process(Other* other, std::string& data) const
{
    data.push_back(other->element);
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