#include "data.hpp"
#include "token.hpp"
#include <string>
#include <vector>

std::string _filename;
std::string _source;
std::vector<Token> _tokens;

std::string& DATA::filename(){return _filename;}
std::string& DATA::source(){return _source;}
std::vector<Token>& DATA::tokens(){return _tokens;}