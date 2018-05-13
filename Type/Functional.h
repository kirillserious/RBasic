#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

#include <map>
#include <vector>

#include "Type.h"
#include "../Scanner/Lex.h"

#include <iostream>
class Functional
{
public:
    std::vector <std::pair<std::string, Type>> v_table;
    std::vector <Lex> poliz;

    bool is_here (const std::string &s) {
        
        for (auto i = v_table.begin(); i != v_table.end(); ++i) {
            if (i->first == s) return true;
        }
        return false;
    }
};

#endif