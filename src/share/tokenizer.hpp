#pragma once

#include "preprocessing_token.hpp"
#include <string>
#include <vector>

class Tokenizer
{
public:
    Tokenizer(const std::string& src,
              std::vector<class PreprocessingToken*>& ppTokens);

    void execute();

private:
    void deleteComment();

    void addPreprocessingToken(class PreprocessingToken_Symbol* preprocessingToken_symbol, std::string::size_type idx);

    class PreprocessingFile* conPreprocessingFile();
    class PreprocessingToken_Symbol* conPreprocessingToken_Symbol();

    class CChar* conCChar();
    class CCharSequence* conCCharSequence(class CCharSequence* bef = nullptr);
    class CharacterConstant* conCharacterConstant();
    class ConditionalExpression* conConditionalExpression();
    class ConstantExpression* conConstantExpression();
    class ControlLine* conControlLine();
    class Digit* conDigit();
    class ElifGroup* conElifGroup();
    class ElifGroups* conElifGroups(class ElifGroups* bef = nullptr);
    class ElseGroup* conElseGroup();
    class EncodingPrefix* conEncodingPrefix();
    class EndifLine* conEndifLine();
    class EscapeSequence* conEscapeSequence();
    class Group* conGroup(class Group* bef = nullptr);
    class GroupPart* conGroupPart();
    class HChar* conHChar();
    class HCharSequence* conHCharSequence(class HCharSequence* bef = nullptr);
    class HeaderName* conHeaderName();
    class HexadecimalDigit* conHexadecimalDigit();
    class HexadecimalEscapeSequence* conHexadecimalEscapeSequence(class HexadecimalEscapeSequence* bef = nullptr);
    class HexQuad* conHexQuad();
    class Identifier* conIdentifier(class Identifier* bef = nullptr);
    class IdentifierList* conIdentifierList(class IdentifierList* bef = nullptr);
    class IdentifierNondigit* conIdentifierNondigit();
    class IfGroup* conIfGroup();
    class IfSection* conIfSection();
    class Lparen* conLparen();
    class NewLine* conNewLine();
    class Nondigit* conNondigit();
    class NonDirective* conNonDirective();
    class OctalDigit* conOctalDigit();
    class OctalEscapeSequence* conOctalEscapeSequence();
    class Other* conOther();
    class PpNumber* conPpNumber(class PpNumber* bef = nullptr);
    class PpTokens* conPpTokens(class PpTokens* bef = nullptr);
    class Punctuator* conPunctuator();
    class QChar* conQChar();
    class QCharSequence* conQCharSequence(class QCharSequence* bef = nullptr);
    class ReplacementList* conReplacementList();
    class SChar* conSChar();
    class SCharSequence* conSCharSequence(class SCharSequence* bef = nullptr);
    class Sign* conSign();
    class SimpleEscapeSequence* conSimpleEscapeSequence();
    class StringLiteral* conStringLiteral();
    class TextLine* conTextLine();
    class UniversalCharacterName* conUniversalCharacterName();

    void process(class PreprocessingToken_Symbol* preprocessingToken_symbol, std::string& data) const;

    void process(class CChar* cChar, std::string& data) const;
    void process(class CCharSequence* cCharSequence, std::string& data) const;
    void process(class CharacterConstant* characterConstant, std::string& data) const;
    void process(class Digit* digit, std::string& data) const;
    void process(class EncodingPrefix* encodingPrefix, std::string& data) const;
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
    void process(class Punctuator* punctuator, std::string& data) const;
    void process(class QChar* qChar, std::string& data) const;
    void process(class QCharSequence* qCharSequence, std::string& data) const;
    void process(class SChar* sChar, std::string& data) const;
    void process(class SCharSequence* sCharSequence, std::string& data) const;
    void process(class Sign* sign, std::string& data) const;
    void process(class SimpleEscapeSequence* simpleEscapeSequence, std::string& data) const;
    void process(class StringLiteral* stringLiteral, std::string& data) const;
    void process(class UniversalCharacterName* universalCharacterName, std::string& data) const;

    void processError(const char* message, const std::string& data) const;

    bool isEquality(std::size_t idx, PreprocessingToken&& ppToken) const;
    bool isEquality(std::size_t idx, PreprocessingToken::EClass eClass) const;

    const std::string& mSrc;
    std::string::size_type mIdx;
    class PreprocessingFile* mPreprocessingFile;
    std::vector<class PreprocessingToken*>& mPpTokens;
};