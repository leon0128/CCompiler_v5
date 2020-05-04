#pragma once

#include "token.hpp"
#include "scope.hpp"
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>

class Translator
{
public:
    Translator();

    bool execute(TranslationUnit*, std::string& res);

private:
    bool isConfirmed(AlignmentSpecifier*) const {return true;}
    bool isConfirmed(AtomicTypeSpecifier*) const {return true;}
    bool isConfirmed(Declaration*) const;
    bool isConfirmed(DeclarationSpecifiers*) const;
    bool isConfirmed(Declarator*) const {return true;}
    bool isConfirmed(EnumSpecifier*) const {return true;}
    bool isConfirmed(::ExternalDeclaration*) const;
    bool isConfirmed(FunctionDefinition*) const {return true;}
    bool isConfirmed(FunctionSpecifier*) const {return true;}
    bool isConfirmed(InitDeclarator*) const;
    bool isConfirmed(InitDeclaratorList*) const;
    bool isConfirmed(Initializer*) const {return true;}
    bool isConfirmed(StaticAssertDeclaration*) const {return true;}
    bool isConfirmed(StorageClassSpecifier*) const;
    bool isConfirmed(StructOrUnionSpecifier*) const {return true;}
    bool isConfirmed(TranslationUnit*) const;
    bool isConfirmed(TypedefName*) const {return true;}
    bool isConfirmed(TypeQualifier*) const {return true;}
    bool isConfirmed(TypeSpecifier*) const;

    bool convertTypeSpecifierVec(DeclarationSpecifiers*, std::vector<TypeSpecifier*>&) const;

    template<typename Enum>
    bool enumerationError(const char* cls, Enum tag) const
    {
        std::cerr << "implementation error:\n"
                  << "    what: unexpected enumeration.\n"
                  << "    class: " << cls
                  << "\n    enum: " << tag
                  << std::endl;
        return false;
    }

    std::stringstream mStream;

    bool mIsValid;
};