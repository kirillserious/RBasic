#include "Parser.h"
#include "../Scanner/Lex.h"
#include "../Type/Type.h"
#include "../Type/Functional.h"


#include <map>
#include <vector>
#include <stack>
#include <iostream>
#include <string>

std::map <symbol_t, int>
Parser :: priority =
{
        {LEX_NOT,         2},
        {LEX_MUL,         3},
        {LEX_DIV,         3},
        {LEX_PLUS,        4},
        {LEX_MINUS,       4},
        {LEX_LT,          5},
        {LEX_LE,          5},
        {LEX_GT,          5},
        {LEX_GE,          5},
        {LEX_EQ,          6},
        {LEX_NE,          6},
        {LEX_AND,         7},
        {LEX_OR,          8},
        {LEX_TWODOTS,     9},
        {LEX_APPLY,      10},
        {LEX_ENDOP,      11},
        {LEX_FIGUREOPEN, 20},
        {LEX_SQUREOPEN,  20},
        {LEX_CIRCLEOPEN, 20}
};

void
Parser :: pop_by_priority (symbol_t s, Stack *stack, Poliz *poliz)
{
        while (!stack->empty() 
                        && priority.at(stack->top()) < priority.at(s)) {
                poliz->push_back(stack->top());
                stack->pop();
        }
}

void
Parser :: pop_to_empty (Poliz* poliz, Stack* stack)
{
        while (!stack->empty()) {
                poliz->push_back(stack->top());
                stack->pop();
        }
}

void
Parser :: pop_to_symbol (symbol_t s, Poliz *poliz, Stack *stack)
{
        while (!stack->empty() && stack->top() != s) {
                poliz->push_back(stack->top());
                stack->pop();
        }
        if (!stack->empty()) {
                stack->pop();
        }
}

void
Parser :: next_not_endl ()
{
        while (is(LT_SYMBOL) && sym_is(LEX_ENDL)) {
                print_plus();
                get();
        }
}

void
Parser :: print_plus()
{
        if (!is_file) std :: cout << "+\t";
}

enum LocalReturns
{
        RETURNS_VAR,
        RETURNS_PART,
        RETURNS_FUNC
};

void
Parser :: Line (Poliz *poliz, Stack *stack)
{
        if (is(LT_SYMBOL) && sym_is(LEX_EOF)) {
                return;
        }
        while (is(LT_SYMBOL) && sym_is(LEX_ENDL)) get();

        Expression(poliz, stack);
        if (is(LT_SYMBOL) && (sym_is(LEX_ENDOP) || sym_is(LEX_ENDL) || sym_is(LEX_EOF))){
                pop_to_empty(poliz, stack);
                poliz->push_back(LEX_ENDOP);
                return;
        } else {
                throw ParserExeption("Семантическая ошибка");
        }
}

void
Parser :: Expression (Poliz* poliz, Stack* stack, bool break_place)
{
        //std :: cout << "Expression\n";
        if (is(LT_SYMBOL) && sym_is(LEX_FOR)) {
                get();
                For(poliz, stack);
                return;
        }

        if (break_place && is(LT_SYMBOL) && sym_is(LEX_BREAK)) {
                poliz->push_back(LEX_BREAK);
                get();
                return;

        }

        if (!is(LT_VARIABLE)) {
                Exp1(poliz, stack, 0, break_place);
                return;
        }
        
        int res = Variable(poliz, stack);
        if (res == RETURNS_FUNC) {
                Exp1(poliz, stack, 1, break_place);
                return;
        }

        if (is(LT_SYMBOL) && sym_is(LEX_APPLY)) {
                stack->push(symb());
                get();
                Expression(poliz, stack, break_place);
                return;
        } 
        Exp1(poliz, stack, 1, break_place);
        return;
}

int
Parser :: Variable (Poliz *poliz, Stack *stack)
{
        //std :: cout << "Variable" << std :: endl;
        std::string variable = name();

        get();
        if (is(LT_SYMBOL) && sym_is(LEX_SQUREOPEN)) {
                poliz->push_back(variable);
                stack->push(symb());
                get();
                Expression(poliz, stack);
                next_not_endl();
                if (is(LT_SYMBOL) && sym_is(LEX_SQURECLOSE)) {
                        pop_to_symbol(LEX_SQUREOPEN, poliz, stack);
                        poliz->push_back(LEX_SQUREOPEN);
                        get();
                        return RETURNS_PART;
                }
                   
                throw ParserExeption("Нет закрывающейся ]");
        } 
        if (is(LT_SYMBOL) && sym_is(LEX_CIRCLEOPEN)) {
                get();
                if (is(LT_SYMBOL) && sym_is(LEX_CIRCLECLOSE)) {
                        poliz->push_back(Lex(0, 0));
                        get();
                } else {
                        int arg_count = FunctionCall(poliz, stack);
                        poliz->push_back(Lex(arg_count, 0));
                }  
                poliz->push_back(variable);
                poliz->push_back(LEX_FUNC);
                return RETURNS_FUNC;
        }
        poliz->push_back(variable);
        return RETURNS_VAR;
}

int
Parser :: FunctionCall (Poliz *poliz, Stack *stack)
{
        next_not_endl();
        Stack new_stack;

        if (is(LT_VARIABLE)) {
                int ret = Variable(poliz, stack);
                switch (ret) {
                case RETURNS_VAR:
                        if (is(LT_SYMBOL) && sym_is(LEX_ONEEQ)) {
                                get();
                                next_not_endl();
                                Expression(poliz, &new_stack);
                                pop_to_empty(poliz, &new_stack);
                                break;
                        }
                case RETURNS_PART:
                        if (is(LT_SYMBOL) && sym_is(LEX_APPLY)) {
                                new_stack.push(symb());
                                get();
                                Expression(poliz, &new_stack);
                        } else {
                                Exp1(poliz, &new_stack, 1);
                        }
                        pop_to_empty(poliz, &new_stack);
                        poliz->push_back(Lex());
                        break;
                case RETURNS_FUNC:
                        Exp1(poliz, &new_stack, 1);
                        pop_to_empty(poliz, &new_stack);
                        poliz->push_back(Lex());
                        break;
                default:
                        break;
                }
        } else {
                Expression(poliz, &new_stack);
                pop_to_empty(poliz, &new_stack);
                poliz->push_back(Lex());
        }

        next_not_endl();

        if (is(LT_SYMBOL) && sym_is(LEX_COMMA)) {
                get();
                return FunctionCall(poliz, stack) + 1;
        } else if (is(LT_SYMBOL) && sym_is(LEX_CIRCLECLOSE)) {
                get();
                return 1;
        } else {
                throw ParserExeption("Нет закрывающейся )");
        }
}

void
Parser :: Exp1 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp1\n";
        Exp2(poliz, stack, flag, break_place);
        if (is(LT_SYMBOL) && (sym_is(LEX_AND) || sym_is(LEX_OR))) {
                pop_by_priority(symb(), stack, poliz);
                stack->push(symb());
                get();
                Exp1(poliz, stack, 0, break_place);
        }
}

void
Parser :: Exp2 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp2\n";
        if (flag) { 
                Exp3(poliz, stack, flag, break_place);
        } else {
                if (is(LT_SYMBOL) && sym_is(LEX_NOT)) {
                        pop_by_priority(symb(), stack, poliz);
                        stack->push(symb());
                        get();
                }
                Exp3(poliz, stack, 0, break_place);
        }

}

void
Parser :: Exp3 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp3\n";
        Exp4(poliz, stack, flag, break_place);
        if (is(LT_SYMBOL) && (sym_is(LEX_LT) 
                        || sym_is(LEX_GT) 
                        || sym_is(LEX_LE) 
                        || sym_is(LEX_GE) 
                        || sym_is(LEX_EQ) 
                        || sym_is(LEX_NE))) {
                pop_by_priority(symb(), stack, poliz);
                stack->push(symb());
                get();
                Exp4(poliz, stack, 0, break_place);
        }
}

void
Parser :: Exp4 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp4\n";
        Exp5(poliz, stack, flag, break_place);
        if (is(LT_SYMBOL) && (sym_is(LEX_PLUS) || sym_is(LEX_MINUS))) {
                pop_by_priority(symb(), stack, poliz);
                stack->push(symb());
                get();
                Exp4(poliz, stack, 0, break_place);
        }
}

void
Parser :: Exp5 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp5\n";
        Exp6(poliz, stack, flag, break_place);
        if (is(LT_SYMBOL) && (sym_is(LEX_MUL) || sym_is(LEX_DIV))) {
                pop_by_priority(symb(), stack, poliz);
                stack->push(symb());
                get();
                Exp5(poliz, stack, 0, break_place);
        }
}

void
Parser :: Exp6 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp6\n";
        Exp7(poliz, stack, flag, break_place);
        if (is(LT_SYMBOL) && sym_is(LEX_TWODOTS)) {
                pop_by_priority(symb(), stack, poliz);
                stack->push(symb());
                get();
                Exp6(poliz, stack, 0, break_place);
        }
}

void
Parser :: Exp7 (Poliz* poliz, Stack* stack, int flag, bool break_place)
{
        //std::cout << "Exp7\n";
        if (flag) return;
        next_not_endl();
        if (is(LT_SYMBOL) && sym_is(LEX_CIRCLEOPEN)) {
                stack->push(symb());
                get();
                Expression(poliz, stack);

                next_not_endl();
                if (is(LT_SYMBOL) && sym_is(LEX_CIRCLECLOSE)) {
                        pop_to_symbol(LEX_CIRCLEOPEN, poliz, stack);
                        get();
                        return;
                }
                throw ParserExeption("Нет закрывающейся )");
        }

        if (is(LT_SYMBOL) && sym_is(LEX_FUNC)) {
                get();
                Function(poliz, stack);
                return;
        }

        if (is(LT_SYMBOL) && sym_is(LEX_FIGUREOPEN)) {
                stack->push(symb());
                get();
                InFigure(poliz, stack, break_place);
                return;
        }

        if (is(LT_CONSTANT)) {
                poliz->push_back(type());
                get();
                return;
        }

        if (is(LT_VARIABLE)) {
                Variable(poliz, stack);
                return;
        }

        throw ParserExeption("Нет константы или переменной");
}

void
Parser :: InFigure (Poliz *poliz, Stack *stack, bool break_place)
{
        //std :: cout << "InFigure\n";

        next_not_endl();
        Expression(poliz, stack, break_place);
        if (is(LT_SYMBOL) && (sym_is(LEX_ENDOP) || sym_is(LEX_ENDL))){
                if (sym_is(LEX_ENDL)) print_plus();
                get();
                next_not_endl();
                pop_to_symbol(LEX_FIGUREOPEN, poliz, stack);
                if (is(LT_SYMBOL) && sym_is(LEX_FIGURECLOSE)) {       
                        get();
                        return;
                }
                stack->push(LEX_FIGUREOPEN);
                poliz->push_back(LEX_ENDOP);
                InFigure(poliz, stack, break_place);
                return;
        }
        if (is(LT_SYMBOL) && sym_is(LEX_FIGURECLOSE)) {
                pop_to_symbol(LEX_FIGUREOPEN, poliz, stack);
                get();
                return;
        }
        throw ParserExeption("InFigure: семантическая ошибка");
}


void
Parser :: Function (Poliz *poliz, Stack *stack)
{
        //std :: cout << "Function" << std :: endl;
        next_not_endl();
        if (!is(LT_SYMBOL) || !sym_is(LEX_CIRCLEOPEN)) {
                throw ParserExeption("Нет открывающейся скобки");
        }
        get();
        next_not_endl();

        Functional func;
        if (!is(LT_SYMBOL) || !sym_is(LEX_CIRCLECLOSE)) {
                FunctionArgs(&func);       
        } else {
                get();
        }

        Stack new_stack;
        Expression(&func.poliz, &new_stack);
        pop_to_empty(&func.poliz, &new_stack);
        poliz->push_back(Type(func));
}

void
Parser :: FunctionArgs (Functional *func)
{
        //std :: cerr << "FunctionArgs" << std :: endl;

        next_not_endl();
        if (!is(LT_VARIABLE)) throw ParserExeption("Нет переменной");
        std::string variable = name();
        get();
        next_not_endl();
        if (is(LT_SYMBOL) && sym_is(LEX_CIRCLECLOSE)) {
                if (func->is_here(variable)) {
                        throw ParserExeption("Повторное описание локальной переменной");
                }

                func->v_table.push_back({variable, SimpleType()});
                get();
                return;
        }
        if (is(LT_SYMBOL) && sym_is(LEX_COMMA)) {

                if (func->is_here(variable)) {
                        throw ParserExeption("Повторное описание локальной переменной");
                }
                func->v_table.push_back({variable, SimpleType()});
                get();
                FunctionArgs(func);
                return;
        }
        if (is(LT_SYMBOL) && sym_is(LEX_ONEEQ)) {
                get();
                next_not_endl();
                if (is(LT_CONSTANT)) {
                        if (func->is_here(variable)) {
                                throw ParserExeption("Повторное описание локальной переменной");
                        }
                        func->v_table.push_back({variable, type()});                      
                        get();
                        next_not_endl();
                        if (is(LT_SYMBOL) && sym_is(LEX_CIRCLECLOSE)) {
                                get();
                                return;
                        }
                        if (is(LT_SYMBOL) && sym_is(LEX_COMMA)) {
                                get();
                                FunctionArgs(func);
                                return;
                        }
                        throw ParserExeption("Неизвестный символ FunctionArgs");
                }
                throw ParserExeption("Нет константы");
        }
        throw ParserExeption("Неверный символ");
}

void
Parser :: For (Poliz *poliz, Stack *stack)
{
        next_not_endl();
        if (!is(LT_SYMBOL) || !sym_is(LEX_CIRCLEOPEN)) {
                throw ParserExeption("Нет открывающейся (");
        }
        get();
        next_not_endl();
        if (Variable(poliz, stack) != RETURNS_VAR) {
            throw ParserExeption("Не может быть вектор элементом вектора");
        }
        next_not_endl();
        if (!is(LT_SYMBOL) || !sym_is(LEX_IN)) {
            throw ParserExeption("Нет in");
        }
        get();
        next_not_endl();
        Expression(poliz, stack);
        next_not_endl();
        if (!is(LT_SYMBOL) || !sym_is(LEX_CIRCLECLOSE)) {
            throw ParserExeption("Нет закрывающейся )");
        }
        get();
        Functional func;
        Stack new_stack;
        Expression(&func.poliz, &new_stack, true);
        pop_to_empty(&func.poliz, &new_stack);
        func.poliz.push_back(LEX_ENDOP);
        poliz->push_back(Type(func));
        poliz->push_back(LEX_FOR);
        
}

Parser :: Poliz
Parser :: get_line ()
{
        try {
                get();
                Poliz poliz;                
                Stack stack;                
                Line(&poliz, &stack);        
                return poliz;
        } catch (ScannerExeption& se){
                throw se;
        } catch (ParserExeption& pe) {
                while(!is(LT_SYMBOL) || !sym_is(LEX_ENDL)) {
                        get();
                }
                throw pe;
        }

        
}