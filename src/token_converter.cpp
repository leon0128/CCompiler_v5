#include "token_converter.hpp"
#include <utility>

const std::unordered_map<std::string, int> TokenConverter::KEYWORD_MAP
    = {{"auto", 0},
       {"break", 0},
       {"case", 0},
       {"char", 0},
       {"const", 0},
       {"continue", 0},
       {"default", 0},
       {"do", 0},
       {"double", 0},
       {"else", 0},
       {"enum", 0},
       {"extern", 0},
       {"float", 0},
       {"for", 0},
       {"goto", 0},
       {"if", 0},
       {"inline", 0},
       {"int", 0},
       {"long", 0},
       {"register", 0},
       {"restrict", 0},
       {"return", 0},
       {"short", 0},
       {"signed", 0},
       {"sizeof", 0},
       {"static", 0},
       {"struct", 0},
       {"switch", 0},
       {"typedef", 0},
       {"union", 0},
       {"unsigned", 0},
       {"void", 0},
       {"volatile", 0},
       {"while", 0},
       {"_Alignas", 0},
       {"_Alignof", 0},
       {"_Atomic", 0},
       {"_Bool", 0},
       {"_Complex", 0},
       {"_Generic", 0},
       {"_Imaginary", 0},
       {"_Noreturn", 0},
       {"_Static_assert", 0},
       {"_Thread_local", 0}};

TokenConverter::TokenConverter():
    mIsValid(true)
{
}

bool TokenConverter::execute(const std::vector<PreprocessingToken*>& ppTokens,
                             std::vector<Token*>& tokens)
{
    mIsValid = true;

    for(auto&& e : ppTokens)
        tokens.push_back(convert(e));

    return mIsValid;
}

Token* TokenConverter::convert(CharacterConstant* characterConstant)
{
    Token* token = new Token();

    token->eToken = Token::CONSTANT;
    token->uToken.sConstant.constant = new Constant();
    token->uToken.sConstant.constant->eConstant = Constant::CHARACTER_CONSTANT;
    token->uToken.sConstant.constant->uConstant.sCharacterConstant.characterConstant = characterConstant;

    return token;
}

Token* TokenConverter::convert(Identifier* identifier)
{
    Token* token = new Token();

    std::string data;
    TOKEN::getString(identifier, data);

    auto iter = KEYWORD_MAP.find(data);
    if(iter != KEYWORD_MAP.end())
    {
        token->eToken = Token::KEYWORD;
        token->uToken.sKeyword.keyword = new Keyword();
        token->uToken.sKeyword.keyword->element = iter->first;
    }
    else
    {
        token->eToken = Token::IDENTIFIER;
        token->uToken.sIdentifier.identifier = identifier;
    }

    return token;
}

Token* TokenConverter::convert(PPNumber* ppNumber)
{
    Constant constant;

    bool isValid = true;

    std::string data;
    std::size_t idx = 0;

    TOKEN::getString(ppNumber, data);

    if(constant.uConstant.sIntegerConstant.integerConstant = getIntegerConstant(data, idx))
    {
        if(idx == data.size())
            constant.eConstant = Constant::INTEGER_CONSTANT;
        else
            isValid = false;
    }
    else if(constant.uConstant.sFloatingConstant.floatingConstant = getFloatingConstant(data, idx))
    {
        if(idx == data.size())
            constant.eConstant = Constant::FLOATING_CONSTANT;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
    {
        Token* token = new Token();
        token->eToken = Token::CONSTANT;
        token->uToken.sConstant.constant = new Constant(constant);

        return token;
    }
    else
    {
        mIsValid = false;
        std::cerr << "TokenConverter error:\n"
                  << "    what: Syntax is invalid.\n"
                  << "    data: " << data
                  << std::endl;
        
        return nullptr;
    }
}

Token* TokenConverter::convert(PreprocessingToken* preprocessingToken)
{
    Token* token = nullptr;

    switch(preprocessingToken->ePreprocessingToken)
    {
        case(PreprocessingToken::IDENTIFIER):
            token = convert(preprocessingToken->uPreprocessingToken.sIdentifier.identifier);
            break;
        case(PreprocessingToken::PP_NUMBER):
            token = convert(preprocessingToken->uPreprocessingToken.sPPNumber.ppNumber);
            break;
        case(PreprocessingToken::CHARACTER_CONSTANT):
            token = convert(preprocessingToken->uPreprocessingToken.sCharacterConstant.characterConstant);
            break;
        case(PreprocessingToken::STRING_LITERAL):
            token = convert(preprocessingToken->uPreprocessingToken.sStringLiteral.stringLiteral);
            break;
        case(PreprocessingToken::PUNCTUATOR):
            token = convert(preprocessingToken->uPreprocessingToken.sPunctuator.punctuator);
            break;
        
        default:
            enumerationError("PreprocessingToken", preprocessingToken->ePreprocessingToken);
            break;
    }

    return token;
}

Token* TokenConverter::convert(Punctuator* punctuator)
{
    Token* token = new Token();

    token->eToken = Token::PUNCTUATOR;
    token->uToken.sPunctuator.punctuator = punctuator;

    return token;
}

Token* TokenConverter::convert(StringLiteral* stringLiteral)
{
    Token* token = new Token();

    token->eToken = Token::STRING_LITERAL;
    token->uToken.sStringLiteral.stringLiteral = stringLiteral;

    return token;
}

BinaryExponentPart* TokenConverter::getBinaryExponentPart(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    BinaryExponentPart binaryExponentPart;

    auto befIdx = idx;
    bool isValid = true;

    if(data[idx] == 'p')
    {
        idx++;

        binaryExponentPart.uBinaryExponentPart.s_p_SignDigitSequence.sign = getSign(data, idx);
        
        if(binaryExponentPart.uBinaryExponentPart.s_p_SignDigitSequence.digitSequence = getDigitSequence(data, idx))
            binaryExponentPart.eBinaryExponentPart = BinaryExponentPart::p_SIGN_DIGIT_SEQUENCE;
        else
            isValid = false;
    }
    else if(data[idx] == 'P')
    {
        idx++;

        binaryExponentPart.uBinaryExponentPart.s_P_SignDigitSequence.sign = getSign(data, idx);

        if(binaryExponentPart.uBinaryExponentPart.s_P_SignDigitSequence.digitSequence = getDigitSequence(data, idx))
            binaryExponentPart.eBinaryExponentPart = BinaryExponentPart::P_SIGN_DIGIT_SEQUENCE;
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new BinaryExponentPart(binaryExponentPart);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

DecimalConstant* TokenConverter::getDecimalConstant(const std::string& data, std::size_t& idx, DecimalConstant* bef)
{
    DecimalConstant decimalConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(!bef)
    {
        if(decimalConstant.uDecimalConstant.sNonzeroDigit.nonzeroDigit = getNonzeroDigit(data, idx))
            decimalConstant.eDecimalConstant = DecimalConstant::NONZERO_DIGIT;
        else
            isValid = false;
    }
    else
    {
        if(decimalConstant.uDecimalConstant.sDecimalConstantDigit.digit = getDigit(data, idx))
        {
            decimalConstant.uDecimalConstant.sDecimalConstantDigit.decimalConstant = bef;
            decimalConstant.eDecimalConstant = DecimalConstant::DECIMAL_CONSTANT_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        DecimalConstant* retVal = new DecimalConstant(decimalConstant);
        DecimalConstant* aft = getDecimalConstant(data, idx, retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

DecimalFloatingConstant* TokenConverter::getDecimalFloatingConstant(const std::string& data, std::size_t& idx)
{
    DecimalFloatingConstant decimalFloatingConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(decimalFloatingConstant.uDecimalFloatingConstant.sFractionalConstantExponentPartFloatingSuffix.fractionalConstant = getFractionalConstant(data, idx))
    {
        decimalFloatingConstant.uDecimalFloatingConstant.sFractionalConstantExponentPartFloatingSuffix.exponentPart = getExponentPart(data, idx);
        decimalFloatingConstant.uDecimalFloatingConstant.sFractionalConstantExponentPartFloatingSuffix.floatingSuffix = getFloatingSuffix(data, idx);
        decimalFloatingConstant.eDecimalFloatingConstant = DecimalFloatingConstant::FRACTIONAL_CONSTANT_EXPONENT_PART_FLOATING_SUFFIX;
    }
    else if(decimalFloatingConstant.uDecimalFloatingConstant.sDigitSequenceExponentPartFloatingSuffix.digitSequence = getDigitSequence(data, idx))
    {
        if(decimalFloatingConstant.uDecimalFloatingConstant.sDigitSequenceExponentPartFloatingSuffix.exponentPart = getExponentPart(data, idx))
        {
            decimalFloatingConstant.uDecimalFloatingConstant.sDigitSequenceExponentPartFloatingSuffix.floatingSuffix = getFloatingSuffix(data, idx);
            decimalFloatingConstant.eDecimalFloatingConstant = DecimalFloatingConstant::DIGIT_SEQUENCE_EXPONENT_PART_FLOATING_SUFFIX;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new DecimalFloatingConstant(decimalFloatingConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

Digit* TokenConverter::getDigit(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    Digit digit;

    bool isValid = true;

    if(data[idx] >= '0' && data[idx] <= '9')
        digit.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new Digit(digit);
    else
        return nullptr;
}

DigitSequence* TokenConverter::getDigitSequence(const std::string& data, std::size_t& idx, DigitSequence* bef)
{
    DigitSequence digitSequence;

    auto befIdx = idx;
    bool isValid = true;

    if(!bef)
    {
        if(digitSequence.uDigitSequence.sDigit.digit = getDigit(data, idx))
            digitSequence.eDigitSequence = DigitSequence::DIGIT;
        else
            isValid = false;
    }
    else
    {
        if(digitSequence.uDigitSequence.sDigitSequenceDigit.digit = getDigit(data, idx))
        {
            digitSequence.uDigitSequence.sDigitSequenceDigit.digitSequence = bef;
            digitSequence.eDigitSequence = DigitSequence::DIGIT_SEQUENCE_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        DigitSequence* retVal = new DigitSequence(digitSequence);
        DigitSequence* aft = getDigitSequence(data, idx, bef);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

ExponentPart* TokenConverter::getExponentPart(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    ExponentPart exponentPart;

    auto befIdx = idx;
    bool isValid = true;

    if(data[idx] == 'e')
    {
        idx++;

        exponentPart.uExponentPart.s_e_SignDigitSequence.sign = getSign(data, idx);
        if(exponentPart.uExponentPart.s_e_SignDigitSequence.digitSequence = getDigitSequence(data, idx))
            exponentPart.eExponentPart = ExponentPart::e_SIGN_DIGIT_SEQUENCE;
        else
            isValid = false;
    }
    else if(data[idx] == 'E')
    {
        idx++;

        exponentPart.uExponentPart.s_E_SignDigitSequence.sign = getSign(data, idx);
        if(exponentPart.uExponentPart.s_E_SignDigitSequence.digitSequence = getDigitSequence(data, idx))
            exponentPart.eExponentPart = ExponentPart::E_SIGN_DIGIT_SEQUENCE;
        else
            isValid = false;
    }

    if(isValid)
        return new ExponentPart(exponentPart);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

FloatingConstant* TokenConverter::getFloatingConstant(const std::string& data, std::size_t& idx)
{
    FloatingConstant floatingConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(floatingConstant.uFloatingConstant.sDecimalFloatingConstant.decimalFloatingConstant = getDecimalFloatingConstant(data, idx))
        floatingConstant.eFloatingConstant = FloatingConstant::DECIMAL_FLOATING_CONSTANT;
    else if(floatingConstant.uFloatingConstant.sHexadecimalFloatingConstant.hexadecimalFloatingConstant = getHexadecimalFloatingConstant(data, idx))
        floatingConstant.eFloatingConstant = FloatingConstant::HEXADECIMAL_FLOATING_CONSTANT;
    else
        isValid = false;

    if(isValid)
        return new FloatingConstant(floatingConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

FloatingSuffix* TokenConverter::getFloatingSuffix(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    FloatingSuffix floatingSuffix;

    bool isValid = true;

    if(data[idx] == 'f' ||
       data[idx] == 'l' ||
       data[idx] == 'F' ||
       data[idx] == 'L')
        floatingSuffix.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new FloatingSuffix(floatingSuffix);
    else
        return nullptr;
}

FractionalConstant* TokenConverter::getFractionalConstant(const std::string& data, std::size_t& idx)
{
    FractionalConstant fractionalConstant;

    auto befIdx = idx;
    bool isValid = true;

    DigitSequence* digitSequence = getDigitSequence(data, idx);
    
    if(idx < data.size())
    {
        if(data[idx] == '.')
        {
            idx++;

            if(fractionalConstant.uFractionalConstant.sDigitSequenceDigitSequence.digitSequence_1 = getDigitSequence(data, idx))
            {
                fractionalConstant.uFractionalConstant.sDigitSequenceDigitSequence.digitSequence = digitSequence;
                fractionalConstant.eFractionalConstant = FractionalConstant::DIGIT_SEQUENCE_DIGIT_SEQUENCE;
            }
            else
            {
                if(digitSequence)
                {
                    fractionalConstant.uFractionalConstant.sDigitSequence.digitSequence = digitSequence;
                    fractionalConstant.eFractionalConstant = FractionalConstant::DIGIT_SEQUENCE;
                }
                else
                    isValid = false;
            }
        }
        else
            isValid = false;
    }
    else
        isValid = false;
    
    if(isValid)
        return new FractionalConstant(fractionalConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

HexadecimalConstant* TokenConverter::getHexadecimalConstant(const std::string& data, std::size_t& idx, HexadecimalConstant* bef)
{
    HexadecimalConstant hexadecimalConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(!bef)
    {
        if(hexadecimalConstant.uHexadecimalConstant.sHexadecimalPrefixHexadecimalDigit.hexadecimalPrefix = getHexadecimalPrefix(data, idx))
        {
            if(hexadecimalConstant.uHexadecimalConstant.sHexadecimalPrefixHexadecimalDigit.hexadecimalDigit = getHexadecimalDigit(data, idx))
                hexadecimalConstant.eHexadecimalConstant = HexadecimalConstant::HEXADECIMAL_PREFIX_HEXADECIMAL_DIGIT;
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        if(hexadecimalConstant.uHexadecimalConstant.sHexadecimalConstantHexadecimalDigit.hexadecimalDigit = getHexadecimalDigit(data, idx))
        {
            hexadecimalConstant.uHexadecimalConstant.sHexadecimalConstantHexadecimalDigit.hexadecimalConstant = bef;
            hexadecimalConstant.eHexadecimalConstant = HexadecimalConstant::HEXADECIMAL_CONSTANT_HEXADECIMAL_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        HexadecimalConstant* retVal = new HexadecimalConstant(hexadecimalConstant);
        HexadecimalConstant* aft = getHexadecimalConstant(data, idx, retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

HexadecimalDigit* TokenConverter::getHexadecimalDigit(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    HexadecimalDigit hexadecimalDigit;

    bool isValid = true;

    if((data[idx] >= '0' && data[idx] <= '9') ||
       (data[idx] >= 'a' && data[idx] <= 'f') ||
       (data[idx] >= 'A' && data[idx] <= 'F'))
        hexadecimalDigit.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new HexadecimalDigit(hexadecimalDigit);
    else
        return nullptr;
}

HexadecimalDigitSequence* TokenConverter::getHexadecimalDigitSequence(const std::string& data, std::size_t& idx, HexadecimalDigitSequence* bef)
{
    HexadecimalDigitSequence hexadecimalDigitSequence;

    auto befIdx = idx;
    bool isValid = true;

    if(!bef)
    {
        if(hexadecimalDigitSequence.uHexadecimalDigitSequence.sHexadecimalDigit.hexadecimalDigit = getHexadecimalDigit(data, idx))
            hexadecimalDigitSequence.eHexadecimalDigitSequence = HexadecimalDigitSequence::HEXADECIMAL_DIGIT;
        else
            isValid = false;
    }
    else
    {
        if(hexadecimalDigitSequence.uHexadecimalDigitSequence.sHexadecimalDigitSequenceHexadecimalDigit.hexadecimalDigit = getHexadecimalDigit(data, idx))
        {
            hexadecimalDigitSequence.uHexadecimalDigitSequence.sHexadecimalDigitSequenceHexadecimalDigit.hexadecimalDigitSequence = bef;
            hexadecimalDigitSequence.eHexadecimalDigitSequence = HexadecimalDigitSequence::HEXADECIMAL_DIGIT_SEQUENCE_HEXADECIMAL_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        HexadecimalDigitSequence* retVal = new HexadecimalDigitSequence(hexadecimalDigitSequence);
        HexadecimalDigitSequence* aft = getHexadecimalDigitSequence(data, idx, retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

HexadecimalFloatingConstant* TokenConverter::getHexadecimalFloatingConstant(const std::string& data, std::size_t& idx)
{
    HexadecimalFloatingConstant hexadecimalFloatingConstant;

    auto befIdx = idx;
    bool isValid = true;

    HexadecimalPrefix* hexadecimalPrefix = getHexadecimalPrefix(data, idx);
    if(hexadecimalPrefix)
    {
        if(hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix.hexadecimalFractionalConstant = getHexadecimalFractionalConstant(data, idx))
        {
            if(hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix.binaryExponentPart = getBinaryExponentPart(data, idx))
            {
                hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix.hexadecimalPrefix = hexadecimalPrefix;
                hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalFractionalConstantBinaryExponentPartFloatingSuffix.floatingSuffix = getFloatingSuffix(data, idx);
                hexadecimalFloatingConstant.eHexadecimalFloatingConstant = HexadecimalFloatingConstant::HEXADECIMAL_PREFIX_HEXADECIMAL_FRACTIONAL_CONSTANT_BINARY_EXPONENT_PART_FLOATING_SUFFIX;
            }
            else
                isValid = false;
        }
        else if(hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix.hexadecimalDigitSequence = getHexadecimalDigitSequence(data, idx))
        {
            if(hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix.binaryExponentPart = getBinaryExponentPart(data, idx))
            {
                hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix.hexadecimalPrefix = hexadecimalPrefix;
                hexadecimalFloatingConstant.uHexadecimalFloatingConstant.sHexadecimalPrefixHexadecimalDigitSequenceBinaryExponentPartFloatingSuffix.floatingSuffix = getFloatingSuffix(data, idx);
                hexadecimalFloatingConstant.eHexadecimalFloatingConstant = HexadecimalFloatingConstant::HEXADECIMAL_PREFIX_HEXADECIMAL_DIGIT_SEQUENCE_BINARY_EXPONENT_PART_FLOATING_SUFFIX;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }

    if(isValid)
        return new HexadecimalFloatingConstant(hexadecimalFloatingConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

HexadecimalFractionalConstant* TokenConverter::getHexadecimalFractionalConstant(const std::string& data, std::size_t& idx)
{
    HexadecimalFractionalConstant hexadecimalFractionalConstant;

    auto befIdx = idx;
    bool isValid = true;

    HexadecimalDigitSequence* hexadecimalDigitSequence = getHexadecimalDigitSequence(data, idx);

    if(idx < data.size())
    {
        if(data[idx] == '.')
        {
            idx++;

            if(hexadecimalFractionalConstant.uHexadecimalFractionalConstant.sHexadecimalDigitSequenceHexadecimalDigitSequence.hexadecimalDigitSequence_1 = getHexadecimalDigitSequence(data, idx))
            {
                hexadecimalFractionalConstant.uHexadecimalFractionalConstant.sHexadecimalDigitSequenceHexadecimalDigitSequence.hexadecimalDigitSequence = hexadecimalDigitSequence;
                hexadecimalFractionalConstant.eHexadecimalFractionalConstant = HexadecimalFractionalConstant::HEXADECIMAL_DIGIT_SEQUENCE_HEXADECIMAL_DIGIT_SEQUENCE;
            }
            else
            {
                if(hexadecimalDigitSequence)
                {
                    hexadecimalFractionalConstant.uHexadecimalFractionalConstant.sHexadecimalDigitSequence.hexadecimalDigitSequence = hexadecimalDigitSequence;
                    hexadecimalFractionalConstant.eHexadecimalFractionalConstant = HexadecimalFractionalConstant::HEXADECIMAL_DIGIT_SEQUENCE;
                }
                else
                    isValid = false;
            }
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
        return new HexadecimalFractionalConstant(hexadecimalFractionalConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

HexadecimalPrefix* TokenConverter::getHexadecimalPrefix(const std::string& data, std::size_t& idx)
{
    if(idx + 1 >= data.size())
        return nullptr;

    HexadecimalPrefix hexadecimalPrefix;

    bool isValid = true;

    if(data[idx] == '0' &&
       (data[idx + 1] == 'x' || data[idx + 1] == 'X'))
    {
        hexadecimalPrefix.element = std::move(data.substr(idx, 2));
        idx += 2;
    }
    else
        isValid = false;

    if(isValid)
        return new HexadecimalPrefix(hexadecimalPrefix);
    else
        return nullptr;
}

IntegerConstant* TokenConverter::getIntegerConstant(const std::string& data, std::size_t& idx)
{
    IntegerConstant integerConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(integerConstant.uIntegerConstant.sDecimalConstantIntegerSuffix.decimalConstant = getDecimalConstant(data, idx))
    {
        integerConstant.uIntegerConstant.sDecimalConstantIntegerSuffix.integerSuffix = getIntegerSuffix(data, idx);
        integerConstant.eIntegerConstant = IntegerConstant::DECIMAL_CONSTANT_INTEGER_SUFFIX;
    }
    else if(integerConstant.uIntegerConstant.sOctalConstantIntegerSuffix.octalConstant = getOctalConstant(data, idx))
    {
        integerConstant.uIntegerConstant.sOctalConstantIntegerSuffix.integerSuffix = getIntegerSuffix(data, idx);
        integerConstant.eIntegerConstant = IntegerConstant::OCTAL_CONSTANT_INTEGER_SUFFIX;
    }
    else if(integerConstant.uIntegerConstant.sHexadecimalConstantIntegerSuffix.hexadecimalConstant = getHexadecimalConstant(data, idx))
    {
        integerConstant.uIntegerConstant.sHexadecimalConstantIntegerSuffix.integerSuffix = getIntegerSuffix(data, idx);
        integerConstant.eIntegerConstant = IntegerConstant::HEXADECIMAL_CONSTANT_INTEGER_SUFFIX;
    }
    else
        isValid = false;

    if(isValid)
        return new IntegerConstant(integerConstant);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

IntegerSuffix* TokenConverter::getIntegerSuffix(const std::string& data, std::size_t& idx)
{
    IntegerSuffix integerSuffix;

    auto befIdx = idx;
    bool isValid = true;

    UnsignedSuffix* unsignedSuffix = getUnsignedSuffix(data, idx);
    if(unsignedSuffix)
    {
        if(integerSuffix.uIntegerSuffix.sUnsignedSuffixLongLongSuffix.longLongSuffix = getLongLongSuffix(data, idx))
        {
            integerSuffix.uIntegerSuffix.sUnsignedSuffixLongLongSuffix.unsignedSuffix = unsignedSuffix;
            integerSuffix.eIntegerSuffix = IntegerSuffix::UNSIGNED_SUFFIX_LONG_LONG_SUFFIX;
        }
        else
        {
            integerSuffix.uIntegerSuffix.sUnsignedSuffixLongSuffix.longSuffix = getLongSuffix(data, idx);
            integerSuffix.uIntegerSuffix.sUnsignedSuffixLongSuffix.unsignedSuffix = unsignedSuffix;
            integerSuffix.eIntegerSuffix = IntegerSuffix::UNSIGNED_SUFFIX_LONG_SUFFIX;
        }
    }
    else if(integerSuffix.uIntegerSuffix.sLongLongSuffixUnsignedSuffix.longLongSuffix = getLongLongSuffix(data, idx))
    {
        integerSuffix.uIntegerSuffix.sLongLongSuffixUnsignedSuffix.unsignedSuffix = getUnsignedSuffix(data, idx);
        integerSuffix.eIntegerSuffix = IntegerSuffix::LONG_LONG_SUFFIX_UNSIGNED_SUFFIX;
    }
    else if(integerSuffix.uIntegerSuffix.sLongSuffixUnsignedSuffix.longSuffix = getLongSuffix(data, idx))
    {
        integerSuffix.uIntegerSuffix.sLongSuffixUnsignedSuffix.unsignedsuffix = getUnsignedSuffix(data, idx);
        integerSuffix.eIntegerSuffix = IntegerSuffix::LONG_SUFFIX_UNSIGNED_SUFFIX;
    }
    else
        isValid = false;

    if(isValid)
        return new IntegerSuffix(integerSuffix);
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

LongSuffix* TokenConverter::getLongSuffix(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    LongSuffix longSuffix;

    bool isValid = true;

    if(data[idx] == 'l' ||
       data[idx] == 'L')
        longSuffix.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new LongSuffix(longSuffix);
    else
        return nullptr;
}

LongLongSuffix* TokenConverter::getLongLongSuffix(const std::string& data, std::size_t& idx)
{
    if(idx + 1 >= data.size())
        return nullptr;

    LongLongSuffix longLongSuffix;

    bool isValid = true;

    if((data[idx] == 'l' && data[idx + 1] == 'l') ||
       (data[idx] == 'L' && data[idx + 1] == 'L'))
    {
        longLongSuffix.element = std::move(data.substr(idx, 2));
        idx += 2;
    }
    else
        isValid = false;

    if(isValid)
        return new LongLongSuffix(longLongSuffix);
    else
        return nullptr;
}

NonzeroDigit* TokenConverter::getNonzeroDigit(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    NonzeroDigit nonzeroDigit;

    bool isValid = true;

    if(data[idx] >= '1' && data[idx] <= '9')
        nonzeroDigit.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new NonzeroDigit(nonzeroDigit);
    else
        return nullptr;
}

OctalConstant* TokenConverter::getOctalConstant(const std::string& data, std::size_t& idx, OctalConstant* bef)
{
    OctalConstant octalConstant;

    auto befIdx = idx;
    bool isValid = true;

    if(!bef)
    {
        if(idx < data.size())
        {
            if(data[idx] == '0')
            {
                octalConstant.uOctalConstant.sZero.element = data[idx++];
                octalConstant.eOctalConstant = OctalConstant::ZERO;
            }
            else
                isValid = false;
        }
        else
            isValid = false;
    }
    else
    {
        if(octalConstant.uOctalConstant.sOctalConstantOctalDigit.octalDigit = getOctalDigit(data, idx))
        {
            octalConstant.uOctalConstant.sOctalConstantOctalDigit.octalConstant = bef;
            octalConstant.eOctalConstant = OctalConstant::OCTAL_CONSTANT_OCTAL_DIGIT;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        OctalConstant* retVal = new OctalConstant(octalConstant);
        OctalConstant* aft = getOctalConstant(data, idx, retVal);
        if(aft)
            return aft;
        else
            return retVal;
    }
    else
    {
        idx = befIdx;
        return nullptr;
    }
}

OctalDigit* TokenConverter::getOctalDigit(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    OctalDigit octalDigit;

    bool isValid = true;

    if(data[idx] >= '0' && data[idx] <= '7')
        octalDigit.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new OctalDigit(octalDigit);
    else
        return nullptr;
}

Sign* TokenConverter::getSign(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    Sign sign;

    bool isValid = true;

    if(data[idx] == '+' ||
       data[idx] == '-')
        sign.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new Sign(sign);
    else
        return nullptr;
}

UnsignedSuffix* TokenConverter::getUnsignedSuffix(const std::string& data, std::size_t& idx)
{
    if(idx >= data.size())
        return nullptr;

    UnsignedSuffix unsignedSuffix;

    bool isValid = true;

    if(data[idx] == 'u' ||
       data[idx] == 'U')
        unsignedSuffix.element = data[idx++];
    else
        isValid = false;

    if(isValid)
        return new UnsignedSuffix(unsignedSuffix);
    else
        return nullptr;
}