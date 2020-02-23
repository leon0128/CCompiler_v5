#include "token.hpp"

std::vector<Token*> Token::TOKENS;

void Token::destroy()
{
    for(auto&& e : TOKENS)
        delete e;
}