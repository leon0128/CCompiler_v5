#pragma once

class Scope;

class Scope
{
public:
    enum EKind
    {
        FUNCTION,
        FILE,
        BLOCK,
        FUNCTION_PROTOTYPE
    };

    Scope* outer;
    EKind* kind;

    Scope(Scope* o, EKind k):
        outer(o),
        kind(k){}
};