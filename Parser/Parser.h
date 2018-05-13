#ifndef PARSER_H_
#define PARSER_H_

#include "../Scanner/Scanner.h"
#include "../Exeption.h"

#include <map>
#include <vector>
#include <stack>
#include <iostream>

class Functional;
class LexSymbol;

class Parser : Scanner
{
public:
    typedef std::vector <Lex>       Poliz;
    typedef std::stack  <symbol_t>  Stack;
private:

    static std::map <symbol_t, int> priority;

    Lex lex;
    bool is_file;

    void get () { lex = get_lex();}

    symbol_t                symb () { return lex.symb(); }
    const std::string &     name () { return lex.name(); }
    const Type &            type () { return lex.type(); }
    int                     addr () { return lex.addr(); }
    const Part &            part () { return lex.part(); }
    
    bool is     (lex_t t)    { return lex.is(t);     }
    bool sym_is (symbol_t t) { return lex.sym_is(t); }



    void pop_by_priority (symbol_t, Stack*, Poliz*);
    void pop_to_empty    (Poliz *a, Stack *b);
    void pop_to_symbol   (symbol_t type, Poliz *poliz, Stack *stack);
    void next_not_endl();


    void Line         (Poliz *a, Stack *b);
    void Expression   (Poliz *a, Stack *b, bool break_place = false);
    void Exp1         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp2         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp3         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp4         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp5         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp6         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    void Exp7         (Poliz *a, Stack *b, int flag = 0, bool break_place = false);
    int  Variable     (Poliz *a, Stack *b);
    void Function     (Poliz *a, Stack *b);
    void FunctionArgs (Functional *);
    void InFigure     (Poliz *a, Stack *b, bool break_place);
    void For          (Poliz *a, Stack *b);
    int  FunctionCall (Poliz *poliz, Stack *stack);

    
    void print_plus();

public:


    Parser () : is_file(false) {}
    Parser (const std::string& filename) : Scanner(filename), is_file(true) {}

    Poliz get_line ();
};

#endif