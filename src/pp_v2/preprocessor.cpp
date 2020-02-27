#include "preprocessor.hpp"
#include "initializer.hpp"
#include "../share/tokenizer.hpp"
#include "../share/preprocessing_token.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>
#include <iostream>

Preprocessor::Preprocessor(const std::string& filename,
                           const std::string& directory,
                           ESearch eSearch):
    mInitializer(nullptr),
    mTokenizer(nullptr),
    mFile(filename),
    mDir(directory),
    mSrc(),
    mESearch(eSearch),
    mPpTokens()
{
    mInitializer = new Initializer(this);
    mTokenizer = new Tokenizer(mSrc, mPpTokens);
}

Preprocessor::~Preprocessor()
{
    delete mInitializer;
    delete mTokenizer;
}

void Preprocessor::execute()
{
    mInitializer->execute();
    mTokenizer->execute();

    for(auto&& e : mPpTokens)
        std::cout << e->data << std::endl;
}