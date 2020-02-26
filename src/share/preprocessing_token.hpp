#pragma once

#include <vector>
#include <string>

class PreprocessingToken
{
private:
    static std::vector<PreprocessingToken*> PREPROCESSING_TOKENS;

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

    PreprocessingToken(const std::string& inData,
                       EClass inEClass,
                       std::string::size_type inPos = std::string::npos):
        data(inData),
        eClass(inEClass),
        pos(inPos){}
    PreprocessingToken(std::string&& inData,
                       EClass inEClass,
                       std::string::size_type inPos = std::string::npos):
        data(std::forward<std::string>(inData)),
        eClass(inEClass),
        pos(inPos){}

    void push(){PREPROCESSING_TOKENS.push_back(this);}

    friend inline bool operator==(const PreprocessingToken& lhs, const PreprocessingToken& rhs);
    friend inline bool operator==(const PreprocessingToken& lhs, PreprocessingToken&& rhs);
    friend inline bool operator==(PreprocessingToken&& lhs, const PreprocessingToken& rhs);
    friend inline bool operator==(PreprocessingToken&& lhs, PreprocessingToken&& rhs);
    friend inline bool operator!=(const PreprocessingToken& lhs, const PreprocessingToken& rhs);
    friend inline bool operator!=(const PreprocessingToken& lhs, PreprocessingToken&& rhs);
    friend inline bool operator!=(PreprocessingToken&& lhs, const PreprocessingToken& rhs);
    friend inline bool operator!=(PreprocessingToken&& lhs, PreprocessingToken&& rhs);

    std::string data;
    EClass eClass;
    std::string::size_type pos;
};

bool operator==(const PreprocessingToken& lhs, const PreprocessingToken& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(const PreprocessingToken& lhs, PreprocessingToken&& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(PreprocessingToken&& lhs, const PreprocessingToken& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator==(PreprocessingToken&& lhs, PreprocessingToken&& rhs)
{
    if(lhs.eClass != rhs.eClass)
        return false;
    if(lhs.data == rhs.data)
        return true;
    else
        return false;
}

bool operator!=(const PreprocessingToken& lhs, const PreprocessingToken& rhs)
{
    return !(std::forward<const PreprocessingToken>(lhs) == std::forward<const PreprocessingToken>(rhs));
}

bool operator!=(const PreprocessingToken& lhs, PreprocessingToken&& rhs)
{
    return !(std::forward<const PreprocessingToken>(lhs) == std::forward<PreprocessingToken>(rhs));
}

bool operator!=(PreprocessingToken&& lhs, const PreprocessingToken& rhs)
{
    return !(std::forward<PreprocessingToken>(lhs) == std::forward<const PreprocessingToken>(rhs));
}

bool operator!=(PreprocessingToken&& lhs, PreprocessingToken&& rhs)
{
    return !(std::forward<PreprocessingToken>(lhs) == std::forward<PreprocessingToken>(rhs));
}