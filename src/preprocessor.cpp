#include "preprocessor.hpp"
#include "initializer.hpp"
#include <iostream>
#include <utility>

Preprocessor::Preprocessor():
    mInitializer(nullptr),
    mFile(),
    mDir(),
    mESearch(CURRENT_ONLY),
    mIsValid(true)
{
    mInitializer = new Initializer();
}

Preprocessor::~Preprocessor()
{
    delete mInitializer;
}

bool Preprocessor::execute(const std::string& file,
                           const std::string& dir,
                           ESearch eSearch)
{
    mFile = file,
    mDir = dir;
    mESearch = eSearch;

    initialize();

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
    mIsValid = mInitializer->execute(mFile, mDir, mESearch);

    if(!mIsValid)
        error("failed to initialize source.");
}

void Preprocessor::error(const char* message) const
{
    std::cerr << "preprocessor-class-error:\n    "
              << message
              << std::endl;
}