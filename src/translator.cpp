#include "translator.hpp"
#include <iostream>

Translator::Translator():
    mStream(),
    mIsValid(true)
{
}

bool Translator::execute(TranslationUnit* translationUnit, std::string& res)
{
    if(translationUnit)
    {
        if(mIsValid = isConfirmed(translationUnit))
            ;
    }

    return mIsValid;
}

bool Translator::isConfirmed(Declaration* declaration) const
{
    bool isValid = true;

    switch(declaration->eDeclaration)
    {
        case(Declaration::DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST):
            isValid = isConfirmed(declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers) &&
                      (declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.initDeclaratorList ? isConfirmed(declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.initDeclaratorList) : true);
            break;
        case(Declaration::STATIC_ASSERT_DECLARATION):
            isValid = isConfirmed(declaration->uDeclaration.sStaticAssertDeclaration.staticAssertDeclaration);
            break;
        
        default:
            isValid = false;
            enumerationError("Declaration", declaration->eDeclaration);
            break;
    }

    /*
        A declaration other than a static_assert declaration shall declare at least a declarator
        (other than the parameters of a function or the members of a structure or union), a tag, or
        the members of an enumeration.
    */
    if(isValid)
    {
        if(declaration->eDeclaration == Declaration::DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST)
        {
            if(declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.initDeclaratorList)
                ;
            else
            {
                TypeSpecifier* typeSpecifier = nullptr;
                for(DeclarationSpecifiers* ds = declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers; ds != nullptr;)
                {
                    switch(ds->eDeclarationSpecifiers)
                    {
                        case(DeclarationSpecifiers::TYPE_SPECIFIER):
                            typeSpecifier = ds->uDeclarationSpecifiers.sTypeSpecifier.typeSpecifier;
                            ds = nullptr;
                            break;
                        case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
                            ds = ds->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier;
                            break;
                        case(DeclarationSpecifiers::TYPE_QUALIFIER):
                            ds = ds->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers;
                            break;
                        case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
                            ds = ds->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers;
                            break;
                        case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
                            ds = ds->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers;
                            break;
                        
                        default:
                            isValid = false;
                            enumerationError("DeclarationSpecifiers", ds->eDeclarationSpecifiers);
                            ds = nullptr;
                            break;
                    }
                }

                if(isValid)
                {
                    switch(typeSpecifier->eTypeSpecifier)
                    {
                        case(TypeSpecifier::STRUCT_OR_UNION_SPECIFIER):
                        {
                            switch(typeSpecifier->uTypeSpecifier.sStructOrUnionSpecifier.structOrUnionSpecifier->eStructOrUnionSpecifier)
                            {
                                case(StructOrUnionSpecifier::STRUCT_OR_UNION_IDENTIFIER_STRUCT_DECLARATION_LIST):
                                    if(!typeSpecifier->uTypeSpecifier.sStructOrUnionSpecifier.structOrUnionSpecifier->uStructOrUnionSpecifier.sStructOrUnionIdentifierStructDeclarationList.identifier)
                                        isValid = false;
                                    break;

                                default:
                                    break;
                            }

                            break;
                        }
                        case(TypeSpecifier::ENUM_SPECIFIER):
                        {
                            switch(typeSpecifier->uTypeSpecifier.sEnumSpecifier.enumSpecifier->eEnumSpecifier)
                            {
                                case(EnumSpecifier::IDENTIFIER_ENUMERATOR_LIST):
                                    if(!typeSpecifier->uTypeSpecifier.sEnumSpecifier.enumSpecifier->uEnumSpecifier.sIdentifierEnumeratorList.identifier)
                                        isValid = false;
                                    break;
                                
                                default:
                                    break;
                            }

                            break;
                        }

                        default:
                            isValid = false;
                            break;
                    }

                    if(!isValid)
                    {
                        std::cerr << "Translator error:\n"
                                  << "    what: A declaration other than a static_assert declaration shall declare at least a declarator (other than the parameters of a function or the members of a structure or union), a tag, or the members of an enumeration."
                                  << std::endl;
                    }
                }
            }
        }
    }

    /*
        If an identifier has no linkage, there shall be no more than one declaration of the identifier
        (in a declarator or type specifier) with the same scope and in the same name space, except
        that:
        — a typedef name may be redefined to denote the same type as it currently does,
        provided that type is not a variably modified type;
        — tags may be redeclared as specified in 6.7.2.3.
    */
    if(isValid)
        std::cout << "=== not implementation ===" << std::endl;
    
    /*
        All declarations in the same scope that refer to the same object or function shall specify
        compatible types.
    */
    if(isValid)
        std::cout << "=== not implementation ===" << std::endl;

    /*
        At most, one storage-class specifier may be given in the declaration specifiers in a
        declaration, except that _Thread_local may appear with static or extern.
    */
    if(isValid)
    {
        if(declaration->eDeclaration == Declaration::DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST)
        {
            bool isThreadLocal = false;
            StorageClassSpecifier* storageClassSpecifier = nullptr;
            for(DeclarationSpecifiers* ds = declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers; ds != nullptr;)
            {
                switch(ds->eDeclarationSpecifiers)
                {
                    case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
                    {
                        if(ds->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier->element == "_Thread_local")
                            isThreadLocal = true;
                        else if(storageClassSpecifier)
                            isValid = storageClassSpecifier->element == ds->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier->element;
                        else
                            storageClassSpecifier = ds->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier;

                        if(isValid)
                            ds = ds->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier;
                        else
                            ds = nullptr;

                        break;
                    }
                    case(DeclarationSpecifiers::TYPE_SPECIFIER):
                        ds = ds->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers;
                        break;
                    case(DeclarationSpecifiers::TYPE_QUALIFIER):
                        ds = ds->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers;
                        break;
                    case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
                        ds = ds->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers;
                        break;
                    case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
                        ds = ds->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers;
                        break;
                    
                    default:
                        ds = nullptr;
                        isValid = false;
                        enumerationError("DeclarationSpecifiers", ds->eDeclarationSpecifiers);
                        break;
                }
            }

            if(isValid)
            {
                if(isThreadLocal)
                {
                    if(storageClassSpecifier)
                    {
                        if(storageClassSpecifier->element != "extern" &&
                           storageClassSpecifier->element != "static")
                            isValid = false;
                    }
                }
            }

            if(!isValid)
            {
                std::cerr << "Translator error:\n"
                          << "    what: At most, one storage-class specifier may be given in the declaration specifiers in a declaration, except that _Thread_local may appear with static or extern."
                          << std::endl;
            }
        }
    }

    /*
        In the declaration of an object with block scope, if the declaration specifiers include
        _Thread_local, they shall also include either static or extern. If
        _Thread_local appears in any declaration of an object, it shall be present in every
        declaration of that object.
    */
    if(isValid)
        std::cout << "=== not implementation ===" << std::endl;

    /*
        _Thread_local shall not appear in the declaration specifiers of a function declaration.
    */
    if(isValid)
        std::cout << "=== not implementation ===" << std::endl;

    return isValid;
}

bool Translator::isConfirmed(DeclarationSpecifiers* declarationSpecifier) const
{
    bool isValid = true;

    switch(declarationSpecifier->eDeclarationSpecifiers)
    {
        case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
            isValid = isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier) &&
                      (declarationSpecifier->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier ? isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier) : true);
            break;
        case(DeclarationSpecifiers::TYPE_SPECIFIER):
            isValid = isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sTypeSpecifier.typeSpecifier) &&
                      (declarationSpecifier->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers ? isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers) : true);
            break;
        case(DeclarationSpecifiers::TYPE_QUALIFIER):
            isValid = isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sTypeQualifier.typeQualifier) &&
                      (declarationSpecifier->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers ? isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers) : true);
            break;
        case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
            isValid = isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sFunctionSpecifier.functionSpecifier) &&
                      (declarationSpecifier->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers ? isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers) : true);
            break;
        case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
            isValid = isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sAlignmentSpecifier.alignmentSpecifier) &&
                      (declarationSpecifier->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers ? isConfirmed(declarationSpecifier->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers) : true);
            break;
        
        default:
            isValid = false;
            enumerationError("DeclarationSpecifiers", declarationSpecifier->eDeclarationSpecifiers);
            break;
    }

    return isValid;
}

bool Translator::isConfirmed(::ExternalDeclaration* externalDeclaration) const
{
    bool isValid = true;

    switch(externalDeclaration->eExternalDeclaration)
    {
        case(ExternalDeclaration::FUNCTION_DEFINITION):
            isValid = isConfirmed(externalDeclaration->uExternalDeclaration.sFunctionDefinition.functionDefinition);
            break;
        case(ExternalDeclaration::DECLARATION):
            isValid = isConfirmed(externalDeclaration->uExternalDeclaration.sDeclaration.declaration);
            break;
        
        default:
            isValid = false;
            enumerationError("ExternalDeclaration", externalDeclaration->eExternalDeclaration);
            break;
    }

    /*
        The storage-class specifiers auto and register shall not appear in the declaration
        specifiers in an external declaration.
    */
    if(isValid)
    {
        DeclarationSpecifiers* declarationSpecifiers = nullptr;
        switch(externalDeclaration->eExternalDeclaration)
        {
            case(::ExternalDeclaration::FUNCTION_DEFINITION):
                declarationSpecifiers = externalDeclaration->uExternalDeclaration.sFunctionDefinition.functionDefinition->declarationSpecifiers;
                break;
            case(::ExternalDeclaration::DECLARATION):
            {
                switch(externalDeclaration->uExternalDeclaration.sDeclaration.declaration->eDeclaration)
                {
                    case(Declaration::DECLARATION_SPECIFIERES_INIT_DECLARATOR_LIST):
                        declarationSpecifiers = externalDeclaration->uExternalDeclaration.sDeclaration.declaration->uDeclaration.sDeclarationSpecifiersInitDeclaratorList.declarationSpecifiers;
                        break;
                    
                    default:
                        break;
                }
                break;
            }

            default:
                break;
        }

        while(declarationSpecifiers != nullptr)
        {
            switch(declarationSpecifiers->eDeclarationSpecifiers)
            {
                case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
                {
                    if(declarationSpecifiers->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier->element == "auto" ||
                       declarationSpecifiers->uDeclarationSpecifiers.sStorageClassSpecifier.storageClassSpecifier->element == "register")
                    {
                        isValid = false;
                        declarationSpecifiers = nullptr;
                        std::cerr << "Translator error\n"
                                  << "    what: The storage-class specifiers auto and register shall not appear in the declaration specifiers in an external declaration."
                                  << std::endl;
                    }
                    else
                        declarationSpecifiers = declarationSpecifiers->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier;
                
                    break;
                }
                case(DeclarationSpecifiers::TYPE_SPECIFIER):
                    declarationSpecifiers = declarationSpecifiers->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers;
                    break;
                case(DeclarationSpecifiers::TYPE_QUALIFIER):
                    declarationSpecifiers = declarationSpecifiers->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers;
                    break;
                case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
                    declarationSpecifiers = declarationSpecifiers->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers;
                    break;
                case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
                    declarationSpecifiers = declarationSpecifiers->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers;
                    break;
                
                default:
                    declarationSpecifiers = nullptr;
                    enumerationError("DeclarationSpecifiers", declarationSpecifiers->eDeclarationSpecifiers);
                    break;
            }
        }
    }
    
    /*
        There shall be no more than one external definition for each identifier declared with
        internal linkage in a translation unit. Moreover, if an identifier declared with internal
        linkage is used in an expression (other than as a part of the operand of a sizeof or
        _Alignof operator whose result is an integer constant), there shall be exactly one
        external definition for the identifier in the translation unit.
    */
    if(isValid)
        std::cout << "=== not implement ===" << std::endl;

    return isValid;
}

bool Translator::isConfirmed(InitDeclarator* initDeclarator) const
{
    bool isValid = true;

    switch(initDeclarator->eInitDeclarator)
    {
        case(InitDeclarator::DECLARATOR):
            isValid = isConfirmed(initDeclarator->uInitDeclarator.sDeclarator.declarator);
            break;
        case(InitDeclarator::DECLARATOR_INITIALIZER):
            isValid = isConfirmed(initDeclarator->uInitDeclarator.sDeclaratorInitializer.declarator) &&
                      isConfirmed(initDeclarator->uInitDeclarator.sDeclaratorInitializer.initializer);
            break;
        
        default:
            isValid = false;
            enumerationError("InitDeclarator", initDeclarator->eInitDeclarator);
            break;
    }

    return isValid;
}

bool Translator::isConfirmed(InitDeclaratorList* initDeclaratorList) const
{
    bool isValid = true;

    for(auto&& id : initDeclaratorList->initDeclarators)
    {
        if(!(isValid = isConfirmed(id)))
            break;
    }

    return isValid;
}

bool Translator::isConfirmed(StorageClassSpecifier* storageClassSpecifier) const
{
    bool isValid = true;

    return isValid;
}

bool Translator::isConfirmed(TranslationUnit* translationUnit) const
{
    bool isValid = true;

    for(auto&& ed : translationUnit->externalDeclarations)
    {
        if(!(isValid = isConfirmed(ed)))
            break;
    }

    return isValid;
}

bool Translator::isConfirmed(TypeSpecifier* typeSpecifier) const
{
    bool isValid = true;

    switch(typeSpecifier->eTypeSpecifier)
    {
        case(TypeSpecifier::BASE):
            break;
        case(TypeSpecifier::ATOMIC_TYPE_SPECIFIER):
            isValid = isConfirmed(typeSpecifier->uTypeSpecifier.sAtomicTypeSpecifier.atomicTypeSpecifier);
            break;
        case(TypeSpecifier::STRUCT_OR_UNION_SPECIFIER):
            isValid = isConfirmed(typeSpecifier->uTypeSpecifier.sStructOrUnionSpecifier.structOrUnionSpecifier);
            break;
        case(TypeSpecifier::ENUM_SPECIFIER):
            isValid = isConfirmed(typeSpecifier->uTypeSpecifier.sEnumSpecifier.enumSpecifier);
            break;
        case(TypeSpecifier::TYPEDEF_NAME):
            isValid = isConfirmed(typeSpecifier->uTypeSpecifier.sTypedefName.typedefName);
            break;
        
        default:
            isValid = false;
            enumerationError("TypeSpecifier", typeSpecifier->eTypeSpecifier);
            break;
    }

    /*
        At least one type specifier shall be given in the declaration specifiers in each declaration,
        and in the specifier-qualifier list in each struct declaration and type name. Each list of
        type specifiers shall be one of the following multisets (delimited by commas, when there
        is more than one multiset per item); the type specifiers may occur in any order, possibly
        intermixed with the other declaration specifiers.
    */
   if(isValid)
   {

   }

    return isValid;
}

bool Translator::convertTypeSpecifierVec(DeclarationSpecifiers* declarationSpecifiers, std::vector<TypeSpecifier*>& typeSpecifierVec) const
{
    bool isValid = true;

    for(DeclarationSpecifiers* ds = declarationSpecifiers; ds != nullptr;)
    {
        switch(ds->eDeclarationSpecifiers)
        {
            case(DeclarationSpecifiers::STORAGE_CLASS_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sStorageClassSpecifier.declarationSpecifier;
                break;
            case(DeclarationSpecifiers::TYPE_SPECIFIER):
                typeSpecifierVec.push_back(ds->uDeclarationSpecifiers.sTypeSpecifier.typeSpecifier);
                ds = ds->uDeclarationSpecifiers.sTypeSpecifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::TYPE_QUALIFIER):
                ds = ds->uDeclarationSpecifiers.sTypeQualifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::FUNCTION_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sFunctionSpecifier.declarationSpecifiers;
                break;
            case(DeclarationSpecifiers::ALIGNMENT_SPECIFIER):
                ds = ds->uDeclarationSpecifiers.sAlignmentSpecifier.declarationSpecifiers;
                break;
            
            default:
                ds = nullptr;
                isValid = false;
                enumerationError("DeclarationSpecifiers", ds->eDeclarationSpecifiers);
                break;
        }
    }

    return isValid;
}