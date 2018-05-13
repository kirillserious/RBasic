#include "Interpreter.h"
#include "../Type/Part.h"
#include "../Type/Type.h"
#include "../Type/Functional.h"
#include <iostream>

std::map <std::string, Type> Interpreter::v_table;

std :: vector <std :: pair <Lex, Lex>>
Interpreter :: get_params ()
{
        int count = stack.top().addr();
        stack.pop();
        std :: vector <std :: pair <Lex, Lex>> result(count);
        for (int i = 0; i < count; ++i) {
                result[count - i - 1] = get_param();
        }
        return result;
}

std :: pair <Lex, Lex>
Interpreter :: get_param ()
{
        std :: pair <Lex, Lex> result;
        if(stack.top().is(LT_NULL)) {
                result.first = stack.top();
                stack.pop();
                result.second = stack.top();
                stack.pop();
                return result;
        } else {
                result.second = stack.top();
                stack.pop();
                result.first = stack.top();
                stack.pop();
                return result;
        }
}

void 
Interpreter :: c ()
{
        Type result;
        auto params = get_params();
        type_t type = TYPE_NULL;
        for (int i = 0; i < params.size(); ++i) {
                if (!params[i].first.is(LT_NULL)) {
                        throw InterExeption("Нет в c() переменной с таким именем");
                }
                if (type == TYPE_NULL) {
                        type = get_type(params[i].second).type;
                }
                if (get_type(params[i].second).is(TYPE_CHARACTER) && type != TYPE_FUNCTIONAL) {
                        type = TYPE_CHARACTER;
                        for (int i = 0; i < result.size(); ++i) {
                                result.at(i) = result.at(i).to_chr();
                        }   
                }
                switch (type) {
                case TYPE_NUMERAL:
                        result.at(i) = get_type(params[i].second).vector[0].to_num();
                        break;
                case TYPE_LOGICAL:
                        result.at(i) = get_type(params[i].second).vector[0].to_log();
                        break;
                case TYPE_CHARACTER:
                        result.at(i) = get_type(params[i].second).vector[0].to_chr();
                        break;
                case TYPE_FUNCTIONAL:
                        if(!get_type(params[i].second).is(TYPE_FUNCTIONAL)) {
                                throw InterExeption("Нельзя ничего привести к функции");
                        }
                default:
                        result.at(i) = get_type(params[i].second).vector[0];
                        break;
                }
        }
        result.type = type;
        stack.push(result);
}

void
Interpreter :: length ()
{
        auto params = get_params();
        if (params.size() != 1)           throw InterExeption("To many arguments in length()");
        if (!params[0].first.is(LT_NULL)) throw InterExeption("Функция length не содержит поля");
        
        Type result((double)get_type(params[0].second).size());
        stack.push(result);

}

void
Interpreter :: mode ()
{
        auto params = get_params();
        if (params.size() != 1)           throw InterExeption("To many arguments in mode()");
        if (!params[0].first.is(LT_NULL)) throw InterExeption("Функция mode() не содержит поля");
        
        switch(get_type(params[0].second).type) {
        case TYPE_NULL:         stack.push(Type(std :: string("NULL")));        break;
        case TYPE_LOGICAL:      stack.push(Type(std :: string("logical")));     break;
        case TYPE_CHARACTER:    stack.push(Type(std :: string("character")));   break;
        case TYPE_NUMERAL:      stack.push(Type(std :: string("numeral")));     break;
        case TYPE_FUNCTIONAL:   stack.push(Type(std :: string("functional")));  break;
        default :               break;
        }       

}

Type&
Interpreter :: get_str_type_ref (const std::string &s)
{
        if (is_func) {
                auto i = local_table.find(s);
                if (i != local_table.end()) {
                        return i->second;
                }
        }
        auto i = global_table.find(s);
        if (i != global_table.end()) {

                return i->second;
        }
        throw InterExeption("Переменная не определена");
}

Type &
Interpreter :: get_var_type_ref (const Lex &lex)
{
        get_str_type_ref(lex.name());
}

bool
Interpreter :: is_var_part (const Lex &lex){
        if (part_table.find(lex.name()) != part_table.end()) return true;
        else return false;
}

Part &
Interpreter :: get_var_part_ref (const Lex &lex)
{
        auto i = part_table.find(lex.name());
        if (i != part_table.end()) return i->second;
        throw InterExeption("Нет такой части");
}

Type
Interpreter :: get_type(const Lex &lex)
{
        if (lex.is(LT_VARIABLE)) {
                if (is_var_part(lex)) {
                        return get_var_part_ref(lex);
                } else {
                        return get_var_type_ref(lex); 
                }   
        } else if (lex.is(LT_CONSTANT)) {
                return lex.type();
        } else if (lex.is(LT_PART)) {
                return Type(lex.part());
        }
        
}

void
Interpreter :: set_type (const std::string &variable, const Type &type)
{
        if (part_table.find(variable) != part_table.end()) {
                part_table[variable] = type;
                return;
        }
        if (is_func) {
                local_table[variable] = type;
        } else {
                global_table[variable] = type;
        }
}

void
Interpreter :: function_call ()
{
        Lex function = stack.top();
        stack.pop(); 
        if (function.name() == "c")      { c();      return; }
        if (function.name() == "length") { length(); return; }
        if (function.name() == "mode")   { mode();   return; }

        
        Type f = get_type(function);
        if (f.type != TYPE_FUNCTIONAL) throw InterExeption("Вызов не функции");

        auto params = get_params();
        auto i = f.vector.begin();
                if (i->type == TYPE_NULL) throw InterExeption("Нельзя выполнить NULL");

                std::map<std::string, Type> tmp_map;
                std::vector<std::pair<std::string, Type>> &v_table = i->fun_val()->v_table;
                /* Копируем строки */
                for (auto j = v_table.begin(); j != v_table.end(); ++j) {
                        tmp_map[j->first] = Type(SimpleType());
                }

                /* Копируем значения с именами, удаляем их из tmp_par */

                auto tmp_par = params;
                for (auto j = tmp_par.begin(); j != tmp_par.end(); ) {
                        if (j->first.is(LT_VARIABLE)) {
                                auto k = tmp_map.find(j->first.name());
                                if (k != tmp_map.end()) {
                                        k->second = get_type(j->second);
                                        j = tmp_par.erase(j);
                                } else {
                                        throw InterExeption("Функция не содержит этой переменной");
                                }
                        } else {
                                ++j;
                        }
                }

                /* Записываем значения по порядку */
                for (auto j = v_table.begin(); j != v_table.end(); ++j) {
                        if (tmp_map[j->first].type == TYPE_NULL) {
                                if (!tmp_par.empty()) {
                                        tmp_map[j->first] = get_type(tmp_par[0].second);
                                        tmp_par.erase(tmp_par.begin());
                                } else {
                                        tmp_map[j->first] = j->second;
                                }
                        }
                }

                if (!tmp_par.empty()) throw InterExeption("To many arguments in function");

                Interpreter inter(global_table, tmp_map);
                inter.is_func      = true;
                inter.is_for       = false;
                inter.poliz        = i->fun_val()->poliz;

                stack.push(inter.start());
                global_table = inter.global_table;
}

void
Interpreter :: my_for ()
{
        Interpreter inter(global_table, local_table);
        inter.is_func        = is_func;
        inter.is_for         = true;
        inter.poliz          = get_type(stack.top()).vector[0].fun_val()->poliz;

        stack.pop();
        std :: string name_m = stack.top().name();
        int cnt = get_type(stack.top()).size();
        stack.pop();


        inter.part_table = part_table;

        std::string name = stack.top().name();
        stack.pop();
        
        if (part_table.find(name) != part_table.end()) throw InterExeption("Уже есть такое имя");

        for (int i = 1; i <= cnt; ++i) {
                inter.part_table[name] = inter.get_str_type_ref(name_m)[Type((double) i)];
                inter.start();
        }

        local_table = inter.local_table;
        global_table = inter.global_table;
}

Lex
Interpreter :: start ()
{
        int i = 0;
        int size = poliz.size();
        while (i < size) {
                if (!poliz[i].is(LT_SYMBOL)) {
                        stack.push(poliz[i]);
                        ++i;
                } else if (poliz[i].sym_is(LEX_SQUREOPEN)) {
                        Type t = get_type(stack.top());
                        stack.pop();
                        Part p = get_var_type_ref(stack.top())[t];
                        stack.pop();
                        stack.push(p);
                        ++i;
                } else if (poliz[i].sym_is(LEX_APPLY)){
                        Type t = get_type(stack.top());
                        stack.pop();

                        if (stack.top().is(LT_VARIABLE)) {
                                set_type(stack.top().name(), t);
                        } else if (stack.top().is(LT_PART)) {
                                auto p = stack.top().part();
                                stack.pop();
                                stack.push(p = t);
                        } else {
                                throw InterExeption("Неверное присваивание");
                        }
                        
                        if (poliz[i + 1].is(LT_SYMBOL) && poliz[i + 1].sym_is(LEX_ENDOP)) {
                                i += 2;
                                stack.pop();
                        } else {
                                ++i;
                        }
                        
                } else if (poliz[i].sym_is(LEX_ENDOP)) {
                        if (!stack.empty()) {
                                std :: cout << get_type(stack.top()).to_string() << std::endl;
                                stack.pop();
                        }
                        ++i;
                } else if (poliz[i].sym_is(LEX_FUNC)) {
                        function_call();
                        ++i;
                } else if(poliz[i].sym_is(LEX_NOT)) {
                        Type t = get_type(stack.top());
                        stack.pop();
                        stack.push(!t);
                        ++i;
                } else if(poliz[i].sym_is(LEX_FOR)) {
                        my_for();
                        i += 2;
                } else if(poliz[i].sym_is(LEX_BREAK)) {
                        return Lex();
                } else {
                        Type t1 = get_type(stack.top());
                        stack.pop();
                        Type t2 = get_type(stack.top());
                        stack.pop();
                        stack.push(operation(t2, t1, poliz[i].symb()));
                        ++i;
                }
        }
        
        if (!is_for && is_func && stack.empty())    throw InterExeption("Нет возващаемого значения");
        if ((is_for || !is_func) && !stack.empty()) throw InterExeption("For Эээээээээ Вернул что-то");
        if (!stack.empty()) return get_type(stack.top());
        return Lex();
}

void
Interpreter :: make_line (const Poliz &poliz)
{

        auto tmp = Interpreter::v_table;
        std::map <std::string, Type> tmp2;
        Interpreter inter(tmp, tmp2);
        inter.poliz   = poliz;
        inter.is_func = false;
        inter.is_for  = false;
        inter.start();
        Interpreter::v_table = inter.global_table;
}
