#include "preprocessor.hpp"
#include "../share/file_manager.hpp"
#include "../share/config.hpp"
#include <utility>

Preprocessor::Preprocessor(const std::string& filename,
                           const std::string& directory,
                           ESearch eSearch):
    mFile(filename),
    mDir(directory),
    mSrc(),
    mESearch(eSearch)
{
}

void Preprocessor::execute()
{

}