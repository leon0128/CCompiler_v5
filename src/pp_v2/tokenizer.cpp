#include "tokenizer.hpp"
#include "preprocessor.hpp"
#include "token.hpp"
#include "symbol.hpp"
#include <array>

Tokenizer::Tokenizer(Preprocessor* pp):
    mPP(pp),
    mIdx(0)
{
}

void Tokenizer::execute() const
{

}

Digit* Tokenizer::conDigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;
    
    char c = mPP->mSrc.at(mIdx);
    if(c < '0' || c > '9')
        return nullptr;

    Digit* digit = new Digit();
    digit->element = c;
    
    mIdx++;
    return digit;
}

HexadecimalDigit* Tokenizer::conHexadecimalDigit()
{
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    char c = mPP->mSrc.at(mIdx);
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
    if(mIdx >= mPP->mSrc.size())
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
    if(mIdx >= mPP->mSrc.size())
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

        Identifier* aft = conIdentifier(identifier);
        if(aft)
            return aft;
        else
            return identifier;
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
    if(mIdx >= mPP->mSrc.size())
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
    if(mIdx >= mPP->mSrc.size())
        return nullptr;

    char c = mPP->mSrc.at(mIdx);
    if((c < '0' || c > '9') &&
       (c < 'a' || c > 'f') &&
       (c < 'A' || c > 'F'))
        return nullptr;

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
    if(mIdx + 1 >= mPP->mSrc.size())
        return nullptr;

    if(mPP->mSrc.at(mIdx) != '\\')
        return nullptr;

    auto idx = mIdx;
    bool isValid = true;
    UniversalCharacterName* universalCharacterName = nullptr;

    char c = mPP->mSrc.at(mIdx + 1);
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