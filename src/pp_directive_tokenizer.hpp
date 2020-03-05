#pragma once

#include "token.hpp"
#include <cstddef>

class Preprocessor;

class PPDirectiveTokenizer
{
public:
    PPDirectiveTokenizer(Preprocessor* pp);

    bool execute();

private:
    ControlLine* getControlLine();
    ElifGroup* getElifGroup();
    ElifGroups* getElifGroups(ElifGroups* bef = nullptr);
    ElseGroup* getElseGroup();
    EndifLine* getEndifLine();
    Group* getGroup(Group* bef = nullptr);
    GroupPart* getGroupPart();
    IdentifierList* getIdentifierList(IdentifierList* bef = nullptr);
    IfGroup* getIfGroup();
    IfSection* getIfSection();
    Lparen* getLparen();
    NewLine* getNewLine();
    NonDirective* getNonDirective();
    PPTokens* getPPTokens(PPTokens* bef = nullptr);
    PreprocessingFile* getPreprocessingFile();
    ReplacementList* getReplacementList();
    TextLine* getTextLine();

    bool isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken, const std::string& data) const;
    bool isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken, std::string&& data) const;
    bool isMatched(std::size_t idx, PreprocessingToken::EPreprocessingToken) const;

    void error(const char* message) const;

    Preprocessor* mPP;
    std::size_t mIdx;

    bool mIsValid;
};