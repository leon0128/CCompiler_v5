#pragma once

#include "token.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>

class TokenConverter
{
private:
    static const std::unordered_map<std::string, int> KEYWORD_MAP;

public:
    TokenConverter();

    bool execute(const std::vector<PreprocessingToken*>&,
                 std::vector<Token*>&);

private:
    Token* convert(CharacterConstant*);
    Token* convert(Identifier*);
    Token* convert(PPNumber*);
    Token* convert(PreprocessingToken*);
    Token* convert(Punctuator*);
    Token* convert(StringLiteral*);

    BinaryExponentPart* getBinaryExponentPart(const std::string& data, std::size_t& idx);
    DecimalConstant* getDecimalConstant(const std::string& data, std::size_t& idx, DecimalConstant* bef = nullptr);
    DecimalFloatingConstant* getDecimalFloatingConstant(const std::string& data, std::size_t& idx);
    Digit* getDigit(const std::string& data, std::size_t& idx);
    DigitSequence* getDigitSequence(const std::string& data, std::size_t& idx, DigitSequence* bef = nullptr);
    ExponentPart* getExponentPart(const std::string& data, std::size_t& idx);
    FloatingConstant* getFloatingConstant(const std::string& data, std::size_t& idx);
    FloatingSuffix* getFloatingSuffix(const std::string& data, std::size_t& idx);
    FractionalConstant* getFractionalConstant(const std::string& data, std::size_t& idx);
    HexadecimalConstant* getHexadecimalConstant(const std::string& data, std::size_t& idx, HexadecimalConstant* bef = nullptr);
    HexadecimalDigit* getHexadecimalDigit(const std::string& data, std::size_t& idx);
    HexadecimalDigitSequence* getHexadecimalDigitSequence(const std::string& data, std::size_t& idx, HexadecimalDigitSequence* bef = nullptr);
    HexadecimalFloatingConstant* getHexadecimalFloatingConstant(const std::string& data, std::size_t& idx);
    HexadecimalFractionalConstant* getHexadecimalFractionalConstant(const std::string& data, std::size_t& idx);
    HexadecimalPrefix* getHexadecimalPrefix(const std::string& data, std::size_t& idx);
    IntegerConstant* getIntegerConstant(const std::string& data, std::size_t& idx);
    IntegerSuffix* getIntegerSuffix(const std::string& data, std::size_t& idx);
    LongSuffix* getLongSuffix(const std::string& data, std::size_t& idx);
    LongLongSuffix* getLongLongSuffix(const std::string& data, std::size_t& idx);
    NonzeroDigit* getNonzeroDigit(const std::string& data, std::size_t& idx);
    OctalConstant* getOctalConstant(const std::string& data, std::size_t& idx, OctalConstant* bef = nullptr);
    OctalDigit* getOctalDigit(const std::string& data, std::size_t& idx);
    Sign* getSign(const std::string& data, std::size_t& idx);
    UnsignedSuffix* getUnsignedSuffix(const std::string& data, std::size_t& idx);

    template<typename Enum>
    void enumerationError(const char* _class, Enum tag)
    {
        mIsValid = false;
        std::cerr << "implementation error:\n"
                  << "    what: unexpected enumeration.\n"
                  << "    class: " << _class
                  << "\n    enum: " << tag
                  << std::endl;
    }

    bool mIsValid = true;
};