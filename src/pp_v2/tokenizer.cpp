#include "tokenizer.hpp"
#include "preprocessor.hpp"
#include "token.hpp"
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
    for(std::string::size_type i = 0; i < mSrc.size(); i++)
    {
        auto idx = i;
        Identifier* identifier = conIdentifier();
        if(identifier)
        {
            addToken(identifier, idx);
            continue;
        }
    }

    for(auto&& e : mTokens)
        std::cout << "token: " << e->data << std::endl;
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

void Tokenizer::addToken(Identifier* identifier, std::string::size_type idx)
{
    std::string data;
    process(identifier, data);

    Token* token = new Token(data, Token::IDENTIFIER, idx);
    mTokens.push_back(token);
}

void Tokenizer::process(Digit* digit, std::string& data) const
{
    data.push_back(digit->element);
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