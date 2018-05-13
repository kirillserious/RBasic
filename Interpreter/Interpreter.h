#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "../Parser/Parser.h"

#include <string>
#include <stack>

class Interpreter
{
    typedef std::vector <Lex>   Poliz;
    typedef std::stack  <Lex>   Stack;

    static std::map <std::string, Type> v_table;

    std :: pair <Lex, Lex> get_param ();
    std :: vector <std :: pair <Lex, Lex>> get_params ();
    void mode ();
    void length ();
    void c ();

    std::map <std::string, Type> &global_table;
    std::map <std::string, Type> &local_table;
    std::map <std::string, Part> part_table;

    bool is_func;
    bool is_for;

    Poliz poliz;
    Stack stack;

    Interpreter() = default;

    Type   get_type (const Lex &);
    bool   is_var_part(const Lex &);
    Type & get_str_type_ref (const std::string &);
    Type & get_var_type_ref (const Lex &lex);
    Part & get_var_part_ref (const Lex &lex);
    void   set_type (const std::string&, const Type&);

    void function_call();
    void my_for();
    Lex  start();

    Interpreter (std::map <std::string, Type> &gt, std::map <std::string, Type> &lt)
        : global_table(gt), local_table(lt) {}

public:
    static void make_line(const Poliz &);
};

#endif