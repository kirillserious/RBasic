#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <map>
#include <vector>

#include "Type.h"
#include "../Scanner/Lex.h"

class Functional
{
public:
    std::map <std::string, Type> v_table;
    std::vector <Lex> poliz;
};

#endif