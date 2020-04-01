#include "pp_character_converter.hpp"
#include "preprocessor.hpp"
#include <iostream>
#include <climits>

PPCharacterConverter::PPCharacterConverter(Preprocessor* pp):
    mPP(pp),
    mIsValid(true)
{
}

bool PPCharacterConverter::execute()
{
    // convert to character from escapesequence
    for(auto&& e : mPP->mProcessedPPTokens)
    {
        if(e->ePreprocessingToken == PreprocessingToken::CHARACTER_CONSTANT ||
           e->ePreprocessingToken == PreprocessingToken::STRING_LITERAL)
            convert(e);
    }

    // concatenate stringliterals
    for(std::size_t i = 0; i + 1 < mPP->mProcessedPPTokens.size(); i++)
    {
        if(mPP->mProcessedPPTokens[i]->ePreprocessingToken == PreprocessingToken::STRING_LITERAL &&
           mPP->mProcessedPPTokens[i + 1]->ePreprocessingToken == PreprocessingToken::STRING_LITERAL)
        {
            StringLiteral* stringLiteral = concatenateStringLiteral(mPP->mProcessedPPTokens[i]->uPreprocessingToken.sStringLiteral.stringLiteral,
                                                                    mPP->mProcessedPPTokens[i + 1]->uPreprocessingToken.sStringLiteral.stringLiteral);
            
            mPP->mProcessedPPTokens.erase(mPP->mProcessedPPTokens.begin() + i);
            mPP->mProcessedPPTokens[i]->uPreprocessingToken.sStringLiteral.stringLiteral = stringLiteral;
        
            i--;
        }
    }

    return mIsValid;
}

void PPCharacterConverter::convert(PreprocessingToken* preprocessingToken)
{
    std::string data;
    if(preprocessingToken->ePreprocessingToken == PreprocessingToken::CHARACTER_CONSTANT)
    {
        if(preprocessingToken->uPreprocessingToken.sCharacterConstant.characterConstant->eCharacterConstant == CharacterConstant::C_CHAR_SEQUENCE)
            TOKEN::getString(preprocessingToken, data);
        else
        {
            mIsValid = false;
            std::cerr << "PPCharacterConverter error:\n"
                      << "    what: Character constant with prefixes are not supported.\n"
                      << "    data: " << data
                      << std::endl;
        }
    }
    else if(preprocessingToken->ePreprocessingToken == PreprocessingToken::STRING_LITERAL)
    {
        if(!preprocessingToken->uPreprocessingToken.sStringLiteral.stringLiteral->encodingPrefix)
            TOKEN::getString(preprocessingToken, data);
        else
        {
            mIsValid = false;
            std::cerr << "PPCharacterConverter error:\n"
                      << "    what: String literal with prefixes are not supported.\n"
                      << "    data: " << data
                      << std::endl;
        }
    }

    if(!mIsValid)
        return;

    data.erase(data.end() - 1);
    data.erase(data.begin());

    for(std::size_t i = 0; i < data.size(); i++)
    {
        if(data[i] == '\\')
        {
            if(simpleEscapeSequence(data, i));
            else if(octalEscapeSequence(data, i));
            else if(hexadecimalEscapeSequence(data, i));
            else if(universalCharacterName(data, i));
            else
            {
                data.erase(data.begin() + i);
                i--;
                std::cerr << "PPCharacterConverter caution:\n"
                          << "    what: '\\' is not evaluated.\n"
                          << "    data: " << data
                          << std::endl;
            }
        }
    }
}

StringLiteral* PPCharacterConverter::concatenateStringLiteral(StringLiteral* bef, StringLiteral* aft)
{
    if(bef->encodingPrefix ||
       aft->encodingPrefix)
    {
        std::string d;
        if(bef->encodingPrefix)
            TOKEN::getString(bef, d);
        else
            TOKEN::getString(aft, d);

        mIsValid = false;
        std::cerr << "PPCharacterConverter error:\n"
                  << "    what: does not support string literals with encoding prefix.\n"
                  << "    data: " << d
                  << std::endl;

        return bef; 
    }

    if(!bef->sCharSequence)
        return aft;
    else if(!aft->sCharSequence)
        return bef;
    
    SCharSequence* aftLast = aft->sCharSequence;
    
    bool isContinued = true;
    while(isContinued)
    {
        switch(aftLast->eSCharSequence)
        {
            case(SCharSequence::S_CHAR):
                isContinued = false;
                break;
            case(SCharSequence::S_CHAR_SEQUENCE_S_CHAR):
                aftLast = aftLast->uSCharSequence.sSCharSequenceSChar.sCharSequence;
                break;
            
            default:
                mIsValid = false;
                isContinued = false;
                std::cerr << "implementation error:\n"
                          << "    what: unexpected enumuration.\n"
                          << "    enum: " << aftLast->eSCharSequence
                          << std::endl;
                break;
        }
    }

    SChar* tmp = aftLast->uSCharSequence.sSChar.sChar;

    aftLast->eSCharSequence = SCharSequence::S_CHAR_SEQUENCE_S_CHAR;
    aftLast->uSCharSequence.sSCharSequenceSChar = {bef->sCharSequence, tmp};

    return aft;
}

bool PPCharacterConverter::simpleEscapeSequence(std::string& data, std::size_t idx)
{
    char c = 0;
    if(isMatched(data, idx + 1, '\''))
        c = '\'';
    else if(isMatched(data, idx + 1, '"'))
        c = '"';
    else if(isMatched(data, idx + 1, '?'))
        c = '\?';
    else if(isMatched(data, idx + 1, '\\'))
        c = '\\';
    else if(isMatched(data, idx + 1, 'a'))
        c = '\a';
    else if(isMatched(data, idx + 1, 'b'))
        c = '\b';
    else if(isMatched(data, idx + 1, 'f'))
        c = '\f';
    else if(isMatched(data, idx + 1, 'n'))
        c = '\n';
    else if(isMatched(data, idx + 1, 'r'))
        c = '\r';
    else if(isMatched(data, idx + 1, 't'))
        c = '\t';
    else if(isMatched(data, idx + 1, 'v'))
        c = '\v';
    else
        return false;

    data.erase(data.begin() + idx);
    data[idx] = c;

    return true;
}

bool PPCharacterConverter::octalEscapeSequence(std::string& data, std::size_t idx)
{
    unsigned char uc = 0;
    std::string sub(std::move(data.substr(idx + 1, 3)));

    int numCon = 0;
    for(auto&& e : sub)
    {
        if(e >= '0' && e <= '7')
        {
            unsigned int v = uc * 8 + (e - '0');
            if(v <= UCHAR_MAX)
            {
                uc = static_cast<unsigned char>(v);
                numCon++;
            }
            else
                break;
        }
        else
            break;
    }

    if(numCon == 0)
        return false;

    data.erase(data.begin() + idx,
               data.begin() + idx + numCon);
    data[idx] = static_cast<char>(uc);

    return true;
}

bool PPCharacterConverter::hexadecimalEscapeSequence(std::string& data, std::size_t idx)
{
    if(data.size() <= idx + 2)
        return false;

    if(data[idx + 1] != 'x')
        return false;

    unsigned char uc = 0;
    std::string sub(data.substr(idx + 2, 2));

    int numCon;

    for(auto&& e : sub)
    {
        if(e >= '0' && e <= '9')
            uc = uc * 16 + (e - '0');
        else if(e >= 'a' && e <= 'f')
            uc = uc * 16 + (e - 'a' + 10);
        else if(e >= 'A' && e <= 'F')
            uc = uc * 16 + (e - 'A' + 10);
        else
            break;

        numCon++;
    }

    if(numCon == 0)
        return false;

    data.erase(data.begin() + idx,
               data.begin() + idx + numCon + 1);
    data[idx] = static_cast<unsigned char>(uc);

    return true;
}

bool PPCharacterConverter::universalCharacterName(std::string& data, std::size_t idx)
{
    if(data.size() <= idx + 1)
        return false;

    if(data[idx + 1] == 'u' ||
       data[idx + 1] == 'U')
    {
        mIsValid = false;
        std::cerr << "PPCharacterConverter error:\n"
                  << "    what: Universal character name is not supported.\n"
                  << "    data: " << data
                  << std::endl;
    }

    return false;
}

bool PPCharacterConverter::isMatched(const std::string& data,
                                     std::size_t idx,
                                     char c)
{
    if(idx >= data.size())
        return false;

    return data[idx] == c;
}