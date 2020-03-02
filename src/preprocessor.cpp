#include "preprocessor.hpp"
#include "initializer.hpp"
#include "pp_tokenizer.hpp"
#include <iostream>
#include <utility>

Preprocessor::Preprocessor():
    mInitializer(nullptr),
    mPPTokenizer(nullptr),
    mFile(),
    mDir(),
    mSrc(),
    mESearch(CURRENT_ONLY),
    mPPTokens(),
    mIsValid(true)
{
    mInitializer = new Initializer(this);
    mPPTokenizer = new PPTokenizer(this);
}

Preprocessor::~Preprocessor()
{
    delete mInitializer;
    delete mPPTokenizer;
}

bool Preprocessor::execute(const std::string& file,
                           const std::string& dir,
                           ESearch eSearch)
{
    mFile = file,
    mDir = dir;
    mESearch = eSearch;

    initialize();

    if(mIsValid)
        ppTokenize();

    return mIsValid;
}

bool Preprocessor::execute(std::string&& file,
                           std::string&& dir,
                           ESearch eSearch)
{
    std::string f(std::forward<std::string>(file));
    std::string d(std::forward<std::string>(dir));

    return execute(f, d, eSearch);
}

void Preprocessor::initialize()
{
    mIsValid = mInitializer->execute();

    if(!mIsValid)
        error("failed to initialize source code.");
}

void Preprocessor::ppTokenize()
{
    mIsValid = mPPTokenizer->execute();

    if(!mIsValid)
        error("failed to tokenize source code.");
}

void Preprocessor::error(const char* message) const
{
    std::cerr << "preprocessor-class-error:\n    "
              << message
              << std::endl;
}