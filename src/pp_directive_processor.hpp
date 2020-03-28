#pragma once

#include "token.hpp"
#include <vector>
#include <iostream>
#include <cstdint>
#include <string>

class Preprocessor;

class PPDirectiveProcessor
{
public:
    PPDirectiveProcessor(Preprocessor* pp);

    bool execute();

private:
    void process(ControlLine*);
    void process(Group*);
    void process(GroupPart*);
    void process(IfSection*);
    void process(NonDirective*){std::cout << "NonDirective-unimplementation" << std::endl;}
    void process(PreprocessingFile*);
    void process(TextLine*);

    void replaceMacro(std::vector<PreprocessingToken*>&);
    void convertPPTokens(PPTokens*, std::vector<PreprocessingToken*>&);

    // control line
    void ctrlInclude(ControlLine*);
    void ctrlDefine(ControlLine*);
    void ctrlUndef(ControlLine*);
    void ctrlLine(ControlLine*){std::cout << __func__ << " is unimplemented" << std::endl;}
    void ctrlError(ControlLine*){std::cout << __func__ << " is unimplemented" << std::endl;}
    void ctrlPragma(ControlLine*){std::cout << __func__ << " is umimplemented" << std::endl;}
    void ctrl(ControlLine*){std::cout << __func__ << " is unimplemented" << std::endl;}

    // evaluate constant expression
    bool evalAdditiveExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalANDExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalAssignmentExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalCastExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalCharacterConstant(CharacterConstant*, std::uintmax_t& val);
    bool evalConditionalExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalConstant(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalConstantExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalDecimalConstant(const std::string&, std::size_t& idx, std::uintmax_t& val);
    bool evalEqualityExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalExclusiveORExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalHexadecimalConstant(const std::string&, std::size_t& idx, std::size_t& val);
    bool evalInclusiveORExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalIntegerConstant(PPNumber*, std::uintmax_t& val);
    bool evalIntegerSuffix(const std::string&, std::size_t& idx, std::uintmax_t& val);
    bool evalLogicalANDExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalLogicalORExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalMultiplicativeExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalOctalConstant(const std::string&, std::size_t& idx, std::uintmax_t& val);
    bool evalPrimaryExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalPostfixExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalRelationalExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalShiftExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);
    bool evalUnaryExpression(const std::vector<PreprocessingToken*>&, std::size_t& idx, std::uintmax_t& val);

    bool isNonzero(PPTokens*);
    bool isDefined(Identifier*);
    bool isMatched(const std::vector<PreprocessingToken*>& vec, std::size_t idx, PreprocessingToken::EPreprocessingToken tag) const;
    bool isMatched(const std::vector<PreprocessingToken*>& vec, std::size_t idx, PreprocessingToken::EPreprocessingToken tag, std::string data) const;

    template<typename Enum>
    void unexpectedEnumError(const char* name, Enum e)
    {
        mIsValid = false;
        std::cerr << "implementation error:\n"
                  << "    what: unexpected enumeration.\n"
                  << "    class: " << name
                  << "\n    enum:" << e
                  << std::endl;
    }

    Preprocessor* mPP;
    bool mIsValid;
};