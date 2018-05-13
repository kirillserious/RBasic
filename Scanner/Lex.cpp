#include "Lex.h"
#include "../Type/Type.h"
#include "../Type/Part.h"

void
Lex :: delete_it () 
{
        switch (ltype) {
        case LT_SYMBOL:
        case LT_ADDRESS:
                delete (int*)value;
                break;
        case LT_VARIABLE:
                delete (std::string*)value;
                break;
        case LT_CONSTANT:
                delete (Type*)value;
                break;
        case LT_PART:
                delete (Part*)value;
                break;
        default:
                break;
    }
}

void
Lex :: create_it (const Lex &v)
{
        ltype = v.ltype;
        switch (ltype) {
        case LT_SYMBOL:
        case LT_ADDRESS:
                value = new int(*(int*)v.value);
                break;
        case LT_VARIABLE:
                value = new std::string(*(std::string*)v.value);
                break;
        case LT_CONSTANT:
                value = new Type(*(Type*)v.value);
                break;
        case LT_PART:
                value = new Part(*(Part*)v.value);
                break;
        default:
                break;
    }
}

Lex :: Lex ()
        : ltype (LT_NULL) 
{}

Lex :: Lex (const Lex &v)
{
        create_it(v);
}

Lex :: Lex (int v, int flag)
{
        ltype = flag ? LT_SYMBOL : LT_ADDRESS;
        value = new int(v);
}

Lex :: Lex(const std::string &v)
        : ltype (LT_VARIABLE)
{
        value = new std::string(v);
}

Lex :: Lex(const Type &v)
        : ltype (LT_CONSTANT)
{
        value = new Type(v);
}

Lex :: Lex(const Part &v)
        : ltype (LT_PART)
{
        value = new Part(v);
}



Lex& 
Lex :: operator= (const Lex& v)
{
        delete_it();
        create_it(v);
}


symbol_t
Lex :: symb () const
{
        return *(symbol_t*)value;
}

int
Lex :: addr () const
{
        return *(int*)value;
}

const std::string & 
Lex :: name () const
{
        return *(std::string*)value;
}

const Type &
Lex :: type () const
{
        return *(Type*)value;
}

const Part &
Lex :: part () const
{
        return *(Part*)value;
}

bool
Lex :: is (lex_t t) const
{
        return t == ltype;
}

bool
Lex :: sym_is (symbol_t t) const
{
        return t == *(int*)value;
}


Lex :: ~Lex()
{
        delete_it();
}