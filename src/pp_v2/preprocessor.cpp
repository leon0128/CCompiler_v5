#include "preprocessor.hpp"
#include "initializer.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>
#include <iostream>

Preprocessor::Preprocessor(const std::string& filename,
                           const std::string& directory,
                           ESearch eSearch):
    mInitializer(nullptr),
    mFile(filename),
    mDir(directory),
    mSrc(),
    mESearch(eSearch)
{
    mInitializer = new Initializer(this);
}

void Preprocessor::execute()
{
    mInitializer->execute();

    std::cout << mSrc << std::endl;
}