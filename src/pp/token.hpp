#pragma once

#include <string>
#include <utility>

class Token
{
public:
    enum EClass
    {
        IDENTIFIER = 0,
        PREPROCESSING_NUMBER = 1,
        STRING_LITERAL = 2,
        PUNCTUATOR = 3,
        OTHER = 4
    };

    Token(const std::string& inData,
          EClass inEClass,
          std::string::size_type inPos = std::string::npos):
        data(inData),
        eClass(inEClass),
        pos(inPos){}
    Token(std::string&& inData,
          EClass inEClass):
        data(inData),
        eClass(inEClass){}

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