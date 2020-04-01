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
    for(auto&& e : mPP->mProcessedPPTokens)
    {
        if(e->ePreprocessingToken == PreprocessingToken::CHARACTER_CONSTANT ||
           e->ePreprocessingToken == PreprocessingToken::STRING_LITERAL)
            convert(e);
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