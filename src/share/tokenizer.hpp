#pragma once

#include "preprocessing_token.hpp"
#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer(const std::string& src);

    void execute();

private:
    void addPreprocessingToken(class PreprocessingToken_Symbol* preprocessingToken_symbol, std::string::size_type idx);

    class PreprocessingToken_Symbol* conPreprocessingToken_Symbol();

    class CChar* conCChar();
    class CCharSequence* conCCharSequence(class CCharSequence* bef = nullptr);
    class CharacterConstant* conCharacterConstant();
    class Digit* conDigit();
    class EscapeSequence* conEscapeSequence();
    class HChar* conHChar();
    class HCharSequence* conHCharSequence(class HCharSequence* bef = nullptr);
    class HeaderName* conHeaderName();
    class HexadecimalDigit* conHexadecimalDigit();
    class HexadecimalEscapeSequence* conHexadecimalEscapeSequence(class HexadecimalEscapeSequence* bef = nullptr);
    class HexQuad* conHexQuad();
    class Identifier* conIdentifier(class Identifier* bef = nullptr);
    class IdentifierNondigit* conIdentifierNondigit();
    class Nondigit* conNondigit();
    class OctalDigit* conOctalDigit();
    class OctalEscapeSequence* conOctalEscapeSequence();
    class Other* conOther();
    class PpNumber* conPpNumber(class PpNumber* bef = nullptr);
    class QChar* conQChar();
    class QCharSequence* conQCharSequence(class QCharSequence* bef = nullptr);
    class Sign* conSign();
    class SimpleEscapeSequence* conSimpleEscapeSequence();
    class UniversalCharacterName* conUniversalCharacterName();

    void process(class PreprocessingToken_Symbol* preprocessingToken_symbol, std::string& data) const;

    void process(class CChar* cChar, std::string& data) const;
    void process(class CCharSequence* cCharSequence, std::string& data) const;
    void process(class CharacterConstant* characterConstant, std::string& data) const;
    void process(class Digit* digit, std::string& data) const;
    void process(class EscapeSequence* escapeSequence, std::string& data) const;
    void process(class HChar* hChar, std::string& data) const;
    void process(class HCharSequence* hCharSequence, std::string& data) const;
    void process(class HeaderName* headerName, std::string& data) const;
    void process(class HexadecimalDigit* hexadecimalDigit, std::string& data) const;
    void process(class HexadecimalEscapeSequence* hexadecimalEscapeSequence, std::string& data) const;
    void process(class HexQuad* hexQuad, std::string& data) const;
    void process(class Identifier* identifier, std::string& data) const;
    void process(class IdentifierNondigit* identifierNondigit, std::string& data) const;
    void process(class Nondigit* nondigit, std::string& data) const;
    void process(class OctalDigit* octalDigit, std::string& data) const;
    void process(class OctalEscapeSequence* octalEscapeSequence, std::string& data) const;
    void process(class Other* other, std::string& data) const;
    void process(class PpNumber* ppNumber, std::string& data) const;
    void process(class QChar* qChar, std::string& data) const;
    void process(class QCharSequence* qCharSequence, std::string& data) const;
    void process(class Sign* sign, std::string& data) const;
    void process(class SimpleEscapeSequence* simpleEscapeSequence, std::string& data) const;
    void process(class UniversalCharacterName* universalCharacterName, std::string& data) const;

    void processError(const char* message, const std::string& data) const;

    const std::string& mSrc;
    std::string::size_type mIdx;
    std::vector<class PreprocessingToken*> mTokens;
};