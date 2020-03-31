#include "pp_character_converter.hpp"
#include "preprocessor.hpp"
#include <iostream>

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
                      << "    what: Character constant with prefixes are not supported."
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
                      << "    what: String literal with prefixes are not supported."
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
                          << "    what: '\\' is not evaluated."
                          << std::endl;
            }
        }
    }
}

bool PPCharacterConverter::simpleEscapeSequence(std::string& data, std::size_t& idx)
{

}

bool PPCharacterConverter::isMatched(const std::string& data,
                                     std::size_t idx,
                                     char c)
{
    if(idx >= data.size())
        return false;

    return data[idx] == c;
}