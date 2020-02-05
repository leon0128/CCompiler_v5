#include "preprocessor.hpp"

int main(int argc, char** argv)
{
    Preprocessor pp;

    if(pp(argc, argv))
        return 0;
    else
        return 1;
}