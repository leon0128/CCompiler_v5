#include "preprocessing_token.hpp"

std::vector<PreprocessingToken*> PreprocessingToken::PREPROCESSING_TOKENS;

void PreprocessingToken::destroy()
{
    for(auto&& e : PREPROCESSING_TOKENS)
        delete e;
}