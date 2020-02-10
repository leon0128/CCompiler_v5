#include "preprocessor.hpp"
#include "token.hpp"
#include "data.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <iostream>
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

Preprocessor::Preprocessor():
    mIsValid(true)
{
}

Preprocessor::~Preprocessor()
{
}

bool Preprocessor::operator()(int argc, char** argv)
{
    confirmArguments(argc, argv);
    if(mIsValid)
        replaceTrigraphs();
    if(mIsValid)
        joinBackslash();
    if(mIsValid)
        deleteComment();
    if(mIsValid)
        writeResult();
    if(mIsValid)
        tokenization();

    return mIsValid;
}

void Preprocessor::confirmArguments(int argc, char** argv)
{
    // confirm argc
    if(argc < 2)
    {
        std::cerr << "error: the number of command-line arguments must be greater than 2."
                  << std::endl;
        mIsValid = false;
        return;
    }

    // confirm argv
    DATA::filename() = argv[1];
    if(!FileManager::read(DATA::filename().c_str(), DATA::source()))
    {
        std::cerr << "error: cannot open file; \""
                  << DATA::filename()
                  << "\""
                  << std::endl;
        mIsValid = false;
    }
}

void Preprocessor::replaceTrigraphs()
{
    if(!config().pp_is_replaced_trigraph)
        return;

    for(auto pos = DATA::source().find("??");
        pos != std::string::npos;
        pos = DATA::source().find("??", pos + 1))
    {
        if(pos + 2 >= DATA::source().size())
            break;

        auto iter = TRIGRAPH_MAP.find(DATA::source().substr(pos, 3));
        if(iter != TRIGRAPH_MAP.end())
            DATA::source().replace(pos, 3, 1, iter->second);
    }
}

void Preprocessor::joinBackslash()
{
    for(auto pos = DATA::source().find('\\');
        pos != std::string::npos;
        pos = DATA::source().find('\\', pos))
    {
        auto endPos = pos + 1;
        bool isValid = false;

        if(config().pp_is_ignored_space)
        {
            for(; endPos < DATA::source().size(); endPos++)
            {
                if(DATA::source().at(endPos) == ' ')
                    continue;
                else if(DATA::source().at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }
        else
        {
            for(; endPos < DATA::source().size(); endPos++)
            {
                if(DATA::source().at(endPos) == '\n')
                    isValid = true;
                
                break;
            }
        }

        if(isValid)
            DATA::source().replace(pos, endPos - pos + 1, "");
        else
            pos++;
    }
}

void Preprocessor::deleteComment()
{
    for(auto pos = DATA::source().find('/');
        pos != std::string::npos;
        pos = DATA::source().find('/', pos + 1))
    {
        if(pos + 1 >= DATA::source().size())
            break;

        if(DATA::source().at(pos + 1) == '/')
        {
            auto p = DATA::source().find('\n', pos + 2);
            if(p != std::string::npos)
                DATA::source().replace(pos, p - pos + 1, " ");
            else
                DATA::source().replace(pos, DATA::source().size() - pos, " ");
        }
        else if(DATA::source().at(pos + 1) == '*')
        {
            auto p = DATA::source().find("*/", pos + 2);
            if(p != std::string::npos)
                DATA::source().replace(pos, p - pos + 2, " ");
        }
    }
}

void Preprocessor::writeResult()
{
    std::string outputFilename(config().gen_result_pathname +
                               "/" +
                               config().pp_result_filename);

    if(!FileManager::write(outputFilename.c_str(), DATA::source()))
    {
        std::cerr << "error: cannot open file; \""
                  << outputFilename
                  << "\""
                  << std::endl;
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
        i < DATA::source().size() && mIsValid;
        i++)
    {
        auto iterator = PUNCTUATOR_VEC.begin(); // for punctuator

        // whitespace
        if(DATA::source().at(i) == ' ' ||
           DATA::source().at(i) == '\0')
            continue;
        // identifier
        else if(isAlphabet(DATA::source().at(i)))
        {
            auto end = i + 1;
            for(; end < DATA::source().size(); end++)
            {
                if(!isAlphabet(DATA::source().at(end)) &&
                   !isNumber(DATA::source().at(end)))
                    break;
            }
            
            DATA::tokens().
                emplace_back(DATA::source().substr(i, end - i),
                             Token::IDENTIFIER);
            i = end - 1;
        }
        // preprocessing number
        else if(isNumber(DATA::source().at(i)))
        {
            auto end = i + 1;
            for(; end < DATA::source().size(); end++)
            {
                if(!isNumber(DATA::source().at(end)))
                    break;
            }

            DATA::tokens().
                emplace_back(DATA::source().substr(i, end - i),
                             Token::PREPROCESSING_NUMBER);
            i = end - 1;
        }
        // string literal ('')
        else if(DATA::source().at(i) == '\'')
        {
            auto end = i + 1;
            for(; end < DATA::source().size(); end++)
            {
                if(DATA::source().at(end) == '\'' &&
                   !(DATA::source().at(end - 1) == '\\'))
                    break;
                else if(DATA::source().at(end) == '\n')
                {
                    mIsValid = false;
                    std::cerr << "error: End token of string literal corresponding to start token does not exist (single-quotation)."
                              << std::endl;
                    break;
                }
            }

            DATA::tokens().
                emplace_back(DATA::source().substr(i, end - i + 1),
                             Token::STRING_LITERAL);
            i = end + 1;
        }
        // string literal ("")
        else if(DATA::source().at(i) == '"')
        {
            auto end = i + 1;
            for(; end < DATA::source().size(); end++)
            {
                if(DATA::source().at(end) == '"' &&
                   !(DATA::source().at(end - 1) == '\\'))
                    break;
                else if(DATA::source().at(end) == '\n')
                {
                    mIsValid = false;
                    std::cerr << "error: End token of string literal corresponding to start token does not exist (double-quotation)."
                              << std::endl;
                    break;
                }
            }

            DATA::tokens().
                emplace_back(DATA::source().substr(i, end - i + 1),
                             Token::STRING_LITERAL);
            i = end + 1;
        }
        // punctuator
        else if((iterator = isPunctuator(DATA::source(), i)) != PUNCTUATOR_VEC.end())
        {
            auto iter = DIGRAPH_MAP.find((*iterator));
            if(iter != DIGRAPH_MAP.end())
            {
                DATA::tokens().
                    emplace_back(iter->second,
                                 Token::PUNCTUATOR);
            }
            else
            {
                DATA::tokens().
                    emplace_back(*iterator,
                                 Token::PUNCTUATOR);
            }

            i += iterator->size() - 1;
        }
        // other
        else
        {
            DATA::tokens().
                emplace_back(std::string(1, DATA::source().at(i)),
                             Token::OTHER);
        }
    }
}