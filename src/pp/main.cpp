#include "preprocessor.hpp"

int main(int argc, char** argv)
{
    Preprocessor pp;

    if(pp(argc, argv, "", Preprocessor::CURRENT_ONLY))
        return 0;
    else
        return 1;
}