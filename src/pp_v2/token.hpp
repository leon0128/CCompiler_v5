#pragma once

#include <vector>
#include <string>

class Token
{
private:
    static std::vector<Token*> TOKENS;

public:
    static void destroy();

    enum EClass
    {
        HEADER_NAME,
        IDENTIFIER,
        PP_NUMBER,
        CHARACTER_CONSTANT,
        STRING_LITERAL,
        PUNCTUATOR,
        OTHER
    };

    Token(const std::string& inData,
          EClass inEClass,
          std::string::size_type inPos = std::string::npos):
        data(inData),
        eClass(inEClass),
        pos(inPos)
        {TOKENS.push_back(this);}
    Token(std::string&& inData,
          EClass inEClass,
          std::string::size_type inPos = std::string::npos):
        data(std::forward<std::string>(inData)),
        eClass(inEClass),
        pos(inPos)
        {TOKENS.push_back(this);}

    friend inline bool operator==(const Token& lhs, const Token& rhs);
    friend inline bool operator==(const Token& lhs, Token&& rhs);
    friend inline bool operator==(Token&& lhs, const Token& rhs);
    friend inline bool operator==(Token&& lhs, Token&& rhs);
    friend inline bool operator!=(const Token& lhs, const Token& rhs);
    friend inline bool operator!=(const Token& lhs, Token&& rhs);
    friend inline bool operator!=(Token&& lhs, const Token& rhs);
    friend inline bool operator!=(Token&& lhs, Token&& rhs);

    std::string data;
    EClass eClass;
    std::string::size_type pos;
};

bool operator==(const Token& lhs, const Token& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(const Token& lhs, Token&& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(Token&& lhs, const Token& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(Token&& lhs, Token&& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator!=(const Token& lhs, const Token& rhs)
{
    return !(std::forward<const Token>(lhs) == std::forward<const Token>(rhs));
}

bool operator!=(const Token& lhs, Token&& rhs)
{
    return !(std::forward<const Token>(lhs) == std::forward<Token>(rhs));
}

bool operator!=(Token&& lhs, const Token& rhs)
{
    return !(std::forward<Token>(lhs) == std::forward<const Token>(rhs));
}

bool operator!=(Token&& lhs, Token&& rhs)
{
    return !(std::forward<Token>(lhs) == std::forward<Token>(rhs));
}