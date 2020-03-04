#pragma once

#include "token.hpp"
#include <string>

class Preprocessor;

class PPTokenizer
{
public:
    PPTokenizer(Preprocessor* pp);

    bool execute();

private:
    void deleteComment();

    CChar* getCChar();
    CCharSequence* getCCharSequence(CCharSequence* bef = nullptr);
    CharacterConstant* getCharacterConstant();
    Digit* getDigit();
    EncodingPrefix* getEncodingPrefix();
    EscapeSequence* getEscapeSequence();
    HChar* getHChar();
    HCharSequence* getHCharSequence(HCharSequence* bef = nullptr);
    HeaderName* getHeaderName();
    HexadecimalDigit* getHexadecimalDigit();
    HexadecimalEscapeSequence* getHexadecimalEscapeSequence(HexadecimalEscapeSequence* bef = nullptr);
    HexQuad* getHexQuad();
    Identifier* getIdentifier(Identifier* bef = nullptr);
    IdentifierNondigit* getIdentifierNondigit();
    Nondigit* getNondigit();
    OctalDigit* getOctalDigit();
    OctalEscapeSequence* getOctalEscapeSequence();
    PPNumber* getPPNumber(PPNumber* bef = nullptr);
    PreprocessingToken* getPreprocessingToken();
    Punctuator* getPunctuator();
    QChar* getQChar();
    QCharSequence* getQCharSequence(QCharSequence* bef = nullptr);
    SChar* getSChar();
    SCharSequence* getSCharSequence(SCharSequence* bef = nullptr);
    Sign* getSign();
    SimpleEscapeSequence* getSimpleEscapeSequence();
    StringLiteral* getStringLiteral();
    UniversalCharacterName* getUniversalCharacterName();

    void getString(Digit*, std::string& data) const;
    void getString(HexadecimalDigit*, std::string& data) const;
    void getString(HexQuad*, std::string& data) const;
    void getString(Identifier*, std::string& data) const;
    void getString(IdentifierNondigit*, std::string& data) const;
    void getString(Nondigit*, std::string& data) const;
    void getString(PreprocessingToken*, std::string& data) const;
    void getString(Punctuator*, std::string& data) const;
    void getString(UniversalCharacterName*, std::string& data) const;

    void getStringError(const char* message) const;

    bool isMatched(std::string::size_type idx, char c) const;
    bool isHeader() const;

    std::string::size_type mIdx;

    Preprocessor* mPP;
    bool mIsValid;
};