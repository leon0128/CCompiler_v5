#pragma once

#include <string>

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
          EClass inEClass):
        data(inData),
        eClass(inEClass){}
    Token(std::string&& inData,
          EClass inEClass):
        data(inData),
        eClass(inEClass){}

    std::string data;
    EClass eClass;
};