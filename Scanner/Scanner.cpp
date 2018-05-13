#include "Lex.h"

#include <string>
#include <cstdio>
#include <cctype>
#include <iostream>
#include <vector>
#include <utility>

#include "Scanner.h"

std::map<char, symbol_t>
Scanner :: one_simbol =
{
        { ':',   LEX_TWODOTS     },
        { '[',   LEX_SQUREOPEN   },
        { ']',   LEX_SQURECLOSE  },
        { '(',   LEX_CIRCLEOPEN  },
        { ')',   LEX_CIRCLECLOSE },
        { '{',   LEX_FIGUREOPEN  },
        { '}',   LEX_FIGURECLOSE },
        { ',',   LEX_COMMA       },
        { '-',   LEX_MINUS       },
        { '+',   LEX_PLUS        },
        { '*',   LEX_MUL         },
        { '/',   LEX_DIV         },
        { ';',   LEX_ENDOP       },
        { '&',   LEX_AND         },
        { '|',   LEX_OR          }
};

std::map<std::string, symbol_t>
Scanner :: keywords =
{
        { "TRUE",        LEX_TRUE   },
        { "FALSE",       LEX_FALSE  },
        { "NULL",        LEX_NULL   },
        { "for",         LEX_FOR    },
        { "function",    LEX_FUNC   },
        { "break",       LEX_BREAK  },
        { "in",          LEX_IN     }
};

Scanner :: Scanner()
        : file(nullptr)
{
        state = ST_H;
        get();
}

Scanner :: Scanner(const std::string &fileName)
{
        file = fopen(fileName.c_str(), "r");
        state = ST_H;
        get();
}

void
Scanner :: get() 
{ 
        c = file ? fgetc(file) : getc(stdin); 
}

void
Scanner :: clear() 
{ 
        buffer.clear(); 
}

void
Scanner :: add() {
        buffer += c; 
}

void
Scanner :: to_next_line()
{
        while (true)
        {
                if (c == EOF || c == '\n') {
                        return;
                }
                get();
        }
}

Lex
Scanner :: get_lex ()
{
        state = ST_H;
        while (true) {
                switch (state) {
                /* Начальное состояние */
                case ST_H:
                        /* EOF */
                        if (c == EOF) {
                                return Lex(LEX_EOF);
                        }
                        /* Перевод строки */
                        else if (c == '\n') {
                                c = ' ';
                                return Lex(LEX_ENDL);
                        }
                        /* Буквенные */
                        else if (isalpha(c) || c == '.') {
                                clear();
                                add();
                                get();
                                state = ST_VAR;
                                break;
                        }
                        /* Цифровые */
                        else if (isdigit(c)) {
                                clear();
                                add();
                                get();
                                state = ST_NUM;
                                break;
                        }
                        /* Комментарий */
                        else if (c == '#') {
                                get();
                                state = ST_COM;
                                break;
                        }
                        /* Односимвольные */
                        else if (one_simbol.find(c) != one_simbol.end()) {
                                symbol_t t = one_simbol.at(c);
                                get();
                                return Lex(t);
                        }
                        /* Строка */
                        else if (c == '"') {
                                clear();
                                get();
                                state = ST_STR;
                                break;
                        }
                        /* Остальное */
                        else if (c == '<') {
                                get();
                                state = ST_LT;
                                break;
                        }
                        else if (c == '>') {
                                get();
                                state = ST_GT;
                                break;
                        }
                        else if (c == '!') {
                                get();
                                state = ST_NOT;
                                break;
                        }
                        else if (c == '=') {
                                get();
                                state = ST_EQ;
                                break;
                        }
                        /* Что-то пробельное */
                        else if (isspace(c)) {
                                get();
                                break;
                        }
                        /* Что-то нехорошее */
                        else {
                                to_next_line();
                                throw ScannerExeption("Scanner: invalid character");
                        }    
                /* Буквенные */
                case ST_VAR:
                        if (isalpha(c) || isdigit(c) || c == '.') {
                                add();
                                get();
                                break;
                        }
                        /* Ключевое слово */
                        else if (keywords.find(buffer) != keywords.end()) {
                                symbol_t key = keywords.at(buffer);
                                if (key == LEX_TRUE) {
                                        return Lex(Type(true));
                                }
                                else if (key == LEX_FALSE) {
                                        return Lex(Type(false));
                                } 
                                else if (key == LEX_NULL) {
                                        return Lex(Type(SimpleType()));
                                }
                                else {
                                        return Lex(key);
                                }
                        }
                        /* Переменная */
                        else {
                                return Lex(buffer);
                        }
                /* Числовые */
                case ST_NUM:
                        if (isdigit(c)) {
                                add();
                                get();
                                break;
                        }
                        else if (c == '.') {
                                add();
                                get();
                                state = ST_NUMAFTERDOT;
                                break;
                        }
                        else {
                                return Lex(Type(std::stod(buffer)));
                        }
                /* Числовые после точки */
                case ST_NUMAFTERDOT:
                        if (isdigit(c)) {
                                add();
                                get();
                                break;
                        }
                        else {
                                return Lex(Type(std::stod(buffer)));
                        }
                /* Комментарий */
                case ST_COM:
                        if (c == '\n' || c == EOF) {
                                state = ST_H;
                                break;
                        }
                        else {
                                get();
                                break;
                        }
                /* Строка */
                case ST_STR:
                        if (c == '\\') {
                                get();
                                state = ST_STRSLESH;
                                break;
                        }
                        else if (c == '"') {
                                get();
                                return Lex(Type(buffer));
                        }
                        else if (c == '\n' || c == EOF) {
                                to_next_line();
                                throw ScannerExeption("Строка не закончилась");
                        }
                        else {
                                add();
                                get();
                                break;
                        }
                /* Строка после \ */
                case ST_STRSLESH:
                        if (c == 'n') {
                                buffer += '\n';
                                get();
                                state = ST_STR;
                                break;
                        }
                        else if (c == 't') {
                                buffer += '\t';
                                get();
                                state = ST_STR;
                                break;
                        }
                        else if (c == '"') {
                                buffer += '"';
                                get();
                                state = ST_STR;
                                break;
                        }
                        else {
                                buffer += '/';
                                state = ST_STR;
                                break;
                        }
                /* Остальное */
                case ST_LT:
                        if (c == '=') {
                                get();
                                return Lex(LEX_LE);
                        }
                        else if (c == '-') {
                                get();
                                return Lex(LEX_APPLY);
                        }
                        else {
                                return Lex(LEX_LT);
                        }
                case ST_GT:
                        if (c == '=') {
                                get();
                                return Lex(LEX_GE);
                        }
                        else {
                                return Lex(LEX_GT);
                        }
                case ST_NOT:
                        if (c == '=') {
                                get();
                                return Lex(LEX_NE);
                        }
                        else {
                                return Lex(LEX_NOT);
                        }
                case ST_EQ:
                        if (c == '=') {
                                get();
                                return Lex(LEX_EQ);
                        }
                        else {
                                return Lex(LEX_ONEEQ);
                        }
                default: 
                        break;
                }
        }
}