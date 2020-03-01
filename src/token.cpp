#include "token.hpp"

std::vector<BaseToken*> BaseToken::TOKENS;

BaseToken::BaseToken()
{
    TOKENS.push_back(this);
}

void BaseToken::destroy()
{
    for(auto&& e : TOKENS)
        delete e;
}