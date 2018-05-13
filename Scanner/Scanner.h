#ifndef SCANNER_H_
#define SCANNER_H_

#include "Lex.h"
#include "../Exeption.h"

#include <string>
#include <vector>
#include <map>
#include <utility>

class Scanner
{
        enum state_type
        {
                ST_H,
                ST_VAR,
                ST_NUM,
                ST_NUMAFTERDOT,
                ST_COM,
                ST_LT,
                ST_GT,
                ST_EQ,
                ST_NOT,
                ST_STR,
                ST_STRSLESH
        };

        static std::map<char, symbol_t>         one_simbol;
        static std::map<std::string, symbol_t>  keywords;

        int c;
        std::string buffer;
        state_type state;

        FILE *file;

        void get();
        void add();
        void clear();
        void to_next_line();

public:
        Scanner();
        Scanner(const std::string &);
        Lex get_lex();
};

#endif
