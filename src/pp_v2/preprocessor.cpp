#include "preprocessor.hpp"

Preprocessor::Preprocessor(const std::string& filename,
                           const std::string& directory,
                           ESearch eSearch):
    mFilename(filename),
    mDirectory(directory),
    mSource(),
    mESearch(eSearch)
{
}

void Preprocessor::execute()
{
    
}