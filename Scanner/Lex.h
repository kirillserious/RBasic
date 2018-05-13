#ifndef LEX_H_
#define LEX_H_

#include <string>
#include "../Type/Type.h"
#include "../Type/Part.h"


enum lex_t
{
    LT_SYMBOL,
    LT_VARIABLE,
    LT_CONSTANT,
    LT_ADDRESS,
    LT_PART,

    LT_NULL       //Значение не задано
};

enum symbol_t
{
    LEX_TRUE, LEX_FALSE, LEX_NULL,          // не используются
    LEX_APPLY,                              // <-
    LEX_TWODOTS,                            // :
    LEX_LT, LEX_GT, LEX_LE, LEX_GE,         // <, >, <=, >=
    LEX_EQ, LEX_NE,                         // == !=
    LEX_SQUREOPEN, LEX_SQURECLOSE,          // [, ]
    LEX_CIRCLEOPEN, LEX_CIRCLECLOSE,        // (, )
    LEX_FIGUREOPEN, LEX_FIGURECLOSE,        // {, }
    LEX_COMMA,                              // ,
    LEX_NOT,                                // !
    LEX_MINUS, LEX_PLUS, LEX_MUL, LEX_DIV,  // -, +, *, /
    LEX_AND, LEX_OR, LEX_ONEEQ,             // &, |, =
    LEX_FUNC,                               // function  
    LEX_FOR, LEX_BREAK, LEX_IN,             // for, break, in

    LEX_ENDL, LEX_ENDOP, LEX_EOF,           // endl, ;, EOF
};

class Lex
{
    void delete_it();
    void create_it(const Lex&);
    
    lex_t ltype;    
    void *value;
public:
    Lex();
    Lex(const Lex&);
    Lex(int i, int fl = 1);
    Lex(const std::string &);
    Lex(const Type &);
    Lex(const Part &);

    Lex& operator=(const Lex&);

    symbol_t             symb () const;
    int                  addr () const;
    const std::string &  name () const;
    const Type &         type () const;
    const Part &         part () const;


    bool is     (lex_t)    const;
    bool sym_is (symbol_t) const;

    ~Lex();
};

#endif