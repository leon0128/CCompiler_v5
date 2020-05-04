#pragma once

#include <unordered_map>
#include <string>

class Scope;

class Scope
{
public:
    class Node;

    enum EScope
    {
        FUNCTION,
        FILE,
        BLOCK,
        FUNCTION_PROTOTYPE
    };

    enum ENameSpace
    {
        LABEL,
        TAG,
        MEMBER,
        OTHER
    };

    Scope* outer() const {return mOuter;}
    EScope scope() const {return mScope;}

    std::unordered_map<std::string, Node*>& getMap(ENameSpace tag)
    {
        switch(tag)
        {
            case(LABEL):
                return mLabelMap;
            case(TAG):
                return mTagMap;
            case(MEMBER):
                return mMemberMap;
            case(OTHER):
                return mOtherMap;
        }
    }

private:
    Scope* mOuter;
    EScope mScope;

    std::unordered_map<std::string, Node*> mLabelMap;
    std::unordered_map<std::string, Node*> mTagMap;
    std::unordered_map<std::string, Node*> mMemberMap;
    std::unordered_map<std::string, Node*> mOtherMap;
};

class Scope::Node
{
public:
    class Object;

    enum ETag
    {
        OBJECT
    } tag;

    union
    {
        Object* object;
    } uNode;

    Node(ETag inTag):
        tag(inTag),
        uNode{nullptr}{}
};

class Scope::Node::Object
{
public:
    enum StorageClass
    {
        NONE,
        EXTERN,
        STATIC,
        _THREAD_LOCAL,
        AUTO,
        REGISTER,
        _THREAD_LOCAL_EXTERN,
        _THREAD_LOCAL_STATIC
    } storageClass;

    enum Type
    {
        NONE,
        VOID,
        CHAR,
        SIGNED_CHAR,
        UNSIGNED_CHAR,
        SIGNED_SHORT,
        UNSIGNED_CHAR,
        SINGED_INT,
        UNSIGNED_INT,
        SINGED_LONG,
        UNSIGNED_LONG,
        SINGED_LONG_LONG,
        UNSIGEND_LONG_LONG,
        FLOAT,
        DOUBLE,
        LONG_DOUBLE,
        _BOOL,
        _COMPLEX_FLOAT,
        _COMPLEX_DOUBLE,
        _COMPLEX_LONG_DOUBLE,
        ATOMIC,
        STRUCT_OR_UNION,
        ENUM,
        TYPEDEF
    } type;

    enum Qualification
    {
        NONE,
        CONST,
        RESTRICT,
        VOLATILE,
        _ATOMIC
    } qualification;

    
};