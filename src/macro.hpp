#pragma once

#include <vector>
#include <string>

class PreprocessingToken;

struct Macro
{
    static std::vector<Macro> MACROS;
    
    std::string identifier;
    
    std::vector<std::string> arguments;
    std::vector<PreprocessingToken*> replacements;

    bool isFunction;
    bool isVariable;
};