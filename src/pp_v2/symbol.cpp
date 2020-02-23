#include "symbol.hpp"

std::vector<Symbol*> Symbol::SYMBOLS;

void Symbol::destroy()
{
    for(auto&& e : SYMBOLS)
        delete e;
}