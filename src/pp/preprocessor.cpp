#include "preprocessor.hpp"
#include "token.hpp"
#include "macro.hpp"
#include "data.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>

const std::unordered_map<std::string, char> Preprocessor::TRIGRAPH_MAP
    = {{"?\?(", '['},
       {"?\?)", ']'},
       {"?\?<", '{'},
       {"?\?>", '}'},
       {"?\?=", '#'},
       {"?\?/", '\\'},
       {"?\?\'", '^'},
       {"?\?!", '|'},
       {"?\?-", '~'}};
const std::unordered_map<std::string, std::string> Preprocessor::DIGRAPH_MAP
    = {{"<%", "{"},
       {"%>", "}"},
       {"<:", "["},
       {":>", "]"},
       {"%:%:", "##"},
       {"%:", "#"}};
const std::unordered_map<std::string, Preprocessor::EDirective> Preprocessor::DIRECTIVE_MAP
    = {{"include", INCLUDE},
       {"define", DEFINE}};
const std::vector<std::string> Preprocessor::PUNCTUATOR_VEC
    = {"%:%:",
       ">>=", "<<=",
       "<%", "%>", "<:", ":>",
       "%:", "++", "--", "->",
       "<<", ">>", "<=", ">=",
       "==", "!=", "&&", "||",
       "+=", "-=", "*=", "/=",
       "%=", "&=", "^=", "|=",
       "##"
       "!","\"", "#", "%", "&", "'",
       "(", ")", "*", "+", ",", "-",
       ".", "/", ":", ";", "<", "=",
       ">", "?", "[", "]", "^", "{",
       "|", "}", "~"};

std::unordered_map<std::string, Macro> Preprocessor::MACRO_MAP;

Preprocessor::Preprocessor():
    mFilename(),
    mSource(),
    mTokens(),
    mESearch(CURRENT_ONLY),
    mIsValid(true)
{
}

Preprocessor::~Preprocessor()
{
}

bool Preprocessor::operator()(int argc,
                              const char* const * const argv,
                              const char* directory,
                              Preprocessor::ESearch eSearch)
{
    confirmArguments(argc, argv, directory, eSearch);
    if(mIsValid)
        replaceTrigraphs();
    if(mIsValid)
        joinBackslash();
    if(mIsValid)
        deleteComment();
    if(mIsValid)
        tokenization();
    if(mIsValid)
        processPreprocessingLanguage();

    for(auto&& e : mTokens)
        std::cout << e.data;

    return mIsValid;
}

void Preprocessor::confirmArguments(int argc,
                                    const char* const * const argv,
                                    const char* directory,
                                    Preprocessor::ESearch eSearch)
{
    mESearch = eSearch;

    // confirm argc
    if(argc < 2)
    {
        std::cerr << "error: the number of command-line arguments must be greater than 2."
                  << std::endl;
        mIsValid = false;
        return;
    }

    // confirm argv
    bool isExisted = true;
    switch(mESearch)
    {
        case(CURRENT_ONLY):
            mFilename = argv[1];
            isExisted = FileManager::isExisted(mFilename.c_str());
            break;
        
        case(CURRENT_SYSTEM):
            mFilename = directory;
            mFilename += "/";
            mFilename += argv[1];
            if(!FileManager::isExisted(mFilename.c_str()))
            {
                for(std::size_t i = 0;
                    i < config().sys_include_pathname_vec.size();
                    i++)
                {
                    mFilename = config().sys_include_pathname_vec.at(i)
                              + "/";
                    mFilename += argv[1];
                    if(FileManager::isExisted(mFilename.c_str()))
                        break;
                    else if(i + 1 == config().sys_include_pathname_vec.size())
                        isExisted = false;
                }
            }
            break;
        
        case(SYSTEM_ONLY):
            for(std::size_t i = 0;
                i < config().sys_include_pathname_vec.size();
                i++)
            {
                mFilename = config().sys_include_pathname_vec.at(i)
                          + "/";
                mFilename += argv[1];
                if(FileManager::isExisted(mFilename.c_str()))
                    break;
                else if(i + 1 == config().sys_include_pathname_vec.size())
                    isExisted = false;
            }
            break;
    }

    if(isExisted)
        FileManager::read(mFilename.c_str(), mSource);
    else
    {
        std::cerr << "error: cannot open file; \""
                  << mFilename
                  << "\""
                  << std::endl;
        mIsValid = false;
    }
}

void Preprocessor::replaceTrigraphs()
{
    if(!config().pp_is_replaced_trigraph)
        return;

    for(auto pos = mSource.find("??");
        pos != std::string::npos;
        pos = mSource.find("??", pos + 1))
    {
        if(pos + 2 >= mSource.size())
            break;

        auto iter = TRIGRAPH_MAP.find(mSource.substr(pos, 3));
        if(iter != TRIGRAPH_MAP.end())
            mSource.replace(pos, 3, 1, iter->second);
    }
}

void Preprocessor::joinBackslash()
{
    for(auto pos = mSource.find('\\');
        pos != std::string::npos;
        pos = mSource.find('\\', pos))
    {
        auto endPos = pos + 1;
        bool isValid = false;

        if(config().pp_is_ignored_space)
        {
            for(; endPos < mSource.size(); endPos++)
            {
                if(mSource.at(endPos) == ' ')
                    continue;
                else if(mSource.at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }
        else
        {
            for(; endPos < mSource.size(); endPos++)
            {
                if(mSource.at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }

        if(isValid)
            mSource.replace(pos, endPos - pos + 1, "");
        else
            pos++;
    }
}

void Preprocessor::deleteComment()
{
    for(auto pos = mSource.find('/');
        pos != std::string::npos;
        pos = mSource.find('/', pos + 1))
    {
        if(pos + 1 >= mSource.size())
            break;

        if(mSource.at(pos + 1) == '/')
        {
            auto p = mSource.find('\n', pos + 2);
            if(p != std::string::npos)
                mSource.replace(pos, p - pos + 1, " ");
            else
                mSource.replace(pos, mSource.size() - pos, " ");
        }
        else if(mSource.at(pos + 1) == '*')
        {
            auto p = mSource.find("*/", pos + 2);
            if(p != std::string::npos)
                mSource.replace(pos, p - pos + 2, " ");
        }
    }
}

void Preprocessor::tokenization()
{
    auto iterator = PUNCTUATOR_VEC.begin(); // for punctuator

    auto isNumber = [](const char c)
    {
        if(c >= '0' && c <= '9')
            return true;
        else
            return false;
    };
    auto isAlphabet = [](const char c)
    {
        if((c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c == '_'))
            return true;
        else
            return false;
    };
    auto isPunctuator = [&iterator](const std::string& src, std::string::size_type pos)
    {
        for(iterator = PUNCTUATOR_VEC.begin();
            iterator != PUNCTUATOR_VEC.end();
            iterator++)
        {
            std::string sub = src.substr(pos, iterator->size());
            if((*iterator) == sub)
                break;
        }

        return iterator;
    };

    for(std::string::size_type i = 0;
        i < mSource.size() && mIsValid;
        i++)
    {
        auto iterator = PUNCTUATOR_VEC.begin(); // for punctuator

        // whitespace
        if(mSource.at(i) == ' ' ||
           mSource.at(i) == '\0')
            continue;
        // identifier
        else if(isAlphabet(mSource.at(i)))
        {
            auto end = i + 1;
            for(; end < mSource.size(); end++)
            {
                if(!isAlphabet(mSource.at(end)) &&
                   !isNumber(mSource.at(end)))
                    break;
            }
            
            mTokens.
                emplace_back(mSource.substr(i, end - i),
                             Token::IDENTIFIER,
                             i);
            i = end - 1;
        }
        // preprocessing number
        else if(isNumber(mSource.at(i)))
        {
            auto end = i + 1;
            for(; end < mSource.size(); end++)
            {
                if(!isNumber(mSource.at(end)))
                    break;
            }

            mTokens.
                emplace_back(mSource.substr(i, end - i),
                             Token::PREPROCESSING_NUMBER,
                             i);
            i = end - 1;
        }
        // string literal ('')
        else if(mSource.at(i) == '\'')
        {
            auto end = i + 1;
            for(; end < mSource.size(); end++)
            {
                if(mSource.at(end) == '\'' &&
                   !(mSource.at(end - 1) == '\\'))
                    break;
                else if(mSource.at(end) == '\n')
                {
                    mIsValid = false;
                    std::cerr << "error: End token of string literal corresponding to start token does not exist (single-quotation)."
                              << std::endl;
                    break;
                }
            }

            mTokens.
                emplace_back(mSource.substr(i, 1),
                             Token::PUNCTUATOR,
                             i);
            mTokens.
                emplace_back(mSource.substr(i + 1, end - i - 1),
                             Token::STRING_LITERAL,
                             i + 1);
            mTokens.
                emplace_back(mSource.substr(end, 1),
                             Token::PUNCTUATOR,
                             end);
            i = end;
        }
        // string literal ("")
        else if(mSource.at(i) == '"')
        {
            auto end = i + 1;
            for(; end < mSource.size(); end++)
            {
                if(mSource.at(end) == '"' &&
                   !(mSource.at(end - 1) == '\\'))
                    break;
                else if(mSource.at(end) == '\n')
                {
                    mIsValid = false;
                    std::cerr << "error: End token of string literal corresponding to start token does not exist (double-quotation)."
                              << std::endl;
                    break;
                }
            }

            mTokens.
                emplace_back(mSource.substr(i, 1),
                             Token::PUNCTUATOR,
                             i);
            mTokens.
                emplace_back(mSource.substr(i + 1, end - i - 1),
                             Token::STRING_LITERAL,
                             i + 1);
            mTokens.
                emplace_back(mSource.substr(end, 1),
                             Token::PUNCTUATOR,
                             end);
            i = end;
        }
        // punctuator
        else if((iterator = isPunctuator(mSource, i)) != PUNCTUATOR_VEC.end())
        {
            auto iter = DIGRAPH_MAP.find((*iterator));
            if(iter != DIGRAPH_MAP.end())
            {
                mTokens.
                    emplace_back(iter->second,
                                 Token::PUNCTUATOR,
                                 i);
            }
            else
            {
                mTokens.
                    emplace_back(*iterator,
                                 Token::PUNCTUATOR,
                                 i);
            }

            i += iterator->size() - 1;
        }
        // other
        else
        {
            mTokens.
                emplace_back(std::string(1, mSource.at(i)),
                             Token::OTHER,
                             i);
        }
    }
}

void Preprocessor::processPreprocessingLanguage()
{
    for(std::size_t i = 0; i < mTokens.size(); i++)
    {
        if(isEquality(i, Token("#", Token::PUNCTUATOR)))
        {
            if(i + 1 >= mTokens.size())
                break;
            if(mTokens.at(i + 1).eClass != Token::IDENTIFIER)
                continue;
            auto iter = DIRECTIVE_MAP.find(mTokens.at(i + 1).data);
            if(iter == DIRECTIVE_MAP.end())
                continue;

            switch(iter->second)
            {
                case(INCLUDE):
                    includeFile(i);
                    break;
                case(DEFINE):
                    defineMacro(i);
                    break;
                
                default:
                    std::cerr << "error: not implement directive"
                              << std::endl;
                    break;
            }
        }
        else
            expandMacro(i);
    }
}

void Preprocessor::includeFile(std::size_t index)
{
    if(isEquality(index + 2, Token("<", Token::PUNCTUATOR)))
    {
        for(std::size_t i = index + 3; i < mTokens.size(); i++)
        {
            if(isEquality(i, Token(">", Token::PUNCTUATOR)))
            {
                for(std::size_t j = index + 4; j < i; j++)
                    mTokens.at(index + 3).data += mTokens.at(j).data;
                mTokens.at(index + 3).eClass = Token::STRING_LITERAL;
                
                mTokens.erase(mTokens.begin() + index + 4,
                              mTokens.begin() + i);
                break;
            }
            else if(isEquality(i, Token("\n", Token::OTHER)) ||
                    i + 1 == mTokens.size())
            {
                std::cerr << "error: closing parenthesis of include directive does not exist."
                          << std::endl;
                return;
            }
        }
    }
    
    std::string includeFilename;
    std::string includePathname;
    ESearch eSearch = CURRENT_ONLY;
    // <> filename
    if(isEquality(index + 2, Token("<", Token::PUNCTUATOR)) &&
       mTokens.at(index + 3).eClass == Token::STRING_LITERAL &&
       isEquality(index + 4, Token(">", Token::PUNCTUATOR)))
    {
        eSearch = SYSTEM_ONLY;
        includeFilename = mTokens.at(index + 3).data;
    }
    // "" filename
    else if(isEquality(index + 2, Token("\"", Token::PUNCTUATOR)) &&
            mTokens.at(index + 3).eClass == Token::STRING_LITERAL &&
            isEquality(index + 4, Token("\"", Token::PUNCTUATOR)))
    {
        eSearch = CURRENT_SYSTEM;
        auto pos = mFilename.rfind('/');
        if(pos != std::string::npos)
            includePathname = mFilename.substr(0, pos + 1);
        
        includeFilename = mTokens.at(index + 3).data;
    }
    else
    {
        std::cerr << "error: arguments of include directive is invalid."
                  << std::endl;
        return;
    }

    const char* tmpArgs[] = {nullptr, includeFilename.c_str()};
    Preprocessor pp;
    if(pp(2, tmpArgs, includePathname.c_str(), eSearch))
    {
        mTokens.erase(mTokens.begin() + index,
                      mTokens.begin() + index + 5);
        mTokens.insert(mTokens.begin() + index,
                       pp.mTokens.begin(),
                       pp.mTokens.end());
    }
    else
    {
        std::cerr << "error: include file is invalid."
                  << std::endl;
    }
}

void Preprocessor::defineMacro(std::size_t index)
{
    if(mTokens.at(index + 2).eClass != Token::IDENTIFIER)
    {
        mIsValid = false;
        std::cerr << "error: define directive must follow by a identifier."
                  << std::endl;
        return;
    }

    Macro macro;
    auto first = mTokens.begin();
    auto last = mTokens.begin();

    // function-like
    if(isEquality(index + 3, Token("(", Token::PUNCTUATOR)) &&
       mTokens.at(index + 2).pos + mTokens.at(index + 2).data.size() == mTokens.at(index + 3).pos)
    {
        macro.eKind = Macro::FUNCTION;

        bool isValid = true;
        auto iter = mTokens.begin() + 4;
        for(; iter != mTokens.end() && isValid; iter++)
        {
            if(iter->eClass == Token::IDENTIFIER)
            {
                macro.args.emplace_back(*iter);
                iter++;
                if((*iter) != Token(",", Token::PUNCTUATOR))
                    break;
            }
            else
                isValid = false;
        }

        if(iter == mTokens.end())
            isValid = false;
        else if(isValid)
        {
            if((*iter) == Token(")", Token::PUNCTUATOR))
                first = iter + 1;
            else
                isValid = false;
        }

        if(!isValid)
        {
            mIsValid = false;
            std::cerr << "error: function-like macro arguments is invalid."
                      << std::endl;
            return;
        }
    }
    // object-like
    else
    {
        macro.eKind = Macro::OBJECT;
        first = mTokens.begin() + index + 3;
    }

    for(last = mTokens.begin() + index + 3;
        last != mTokens.end();
        last++)
    {
        if((*iter) == Token("\n", Token::OTHER))
            break;
    }

    for(; last = mTokens.end(); last++)
    {
        if((*iter) == Token("\n", Token::OTHER))
            break;
    }
    macro.seq = std::vector<Token>(first, last);

    auto result = MACRO_MAP.emplace(mTokens.at(index + 2), macro);
    if(!result.second)
        result.first->second = macro;
    
    mTokens.erase(mTokens.begin() + index,
                  last);
}

bool Preprocessor::expandMacro(std::size_t index)
{
    if(mTokens.at(index).eClass == Token::IDENTIFIER)
    {
        auto iter = MACRO_MAP.find(mTokens.at(index).data);
        if(iter != MACRO_MAP.end())
        {
            mTokens.erase(mTokens.begin() + index);
            mTokens.insert(mTokens.begin() + index,
                           iter->second.seq.begin(),
                           iter->second.seq.end());
            return true;
        }
    }

    return false;
}

bool Preprocessor::isEquality(std::size_t index, const Token& token) const
{
    if(index < mTokens.size())
        return mTokens.at(index) == std::forward<const Token>(token);
    else
        return false;
}

bool Preprocessor::isEquality(std::size_t index, Token&& token) const
{
    if(index < mTokens.size())
        return mTokens.at(index) == std::forward<Token>(token);
    else
        return false;
}