#include "preprocessor.hpp"
#include "token.hpp"
#include "symbol.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    // check command-line arguments
    if(argc >= 2)
    {
        std::string filename = argv[1];
        std::string directory; // this is empty
        Preprocessor pp(filename, directory, Preprocessor::CURRENT_ONLY);
        pp.execute();

        Token::destroy();
        Symbol::destroy();
        return 0;
    }
    else
    {
        std::cerr << "warning: number of command-line arguments is not invalid.\n"
                  << "    number of arguments: "
                  << argc
                  << std::endl;
        return 1;
    }
}