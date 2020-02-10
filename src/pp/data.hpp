#pragma once

#include "token.hpp"
#include <string>
#include <vector>

namespace DATA
{
    std::string& filename();
    std::string& source();
    std::vector<Token>& tokens();
};