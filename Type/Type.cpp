#include "Type.h"
#include "Part.h"
#include "../Scanner/Lex.h"

#include <string>
#include <iostream>

Type :: Type ()
    : type (TYPE_NULL) {}

Type :: Type (const std::vector<SimpleType> &v)
{
    type_t t = TYPE_NULL;
    for (auto i : v) {
        if (t == TYPE_NULL) {
            t = i.type;
        } else if (t != i.type) {
            throw "Разные типы аргументов";
        }
    }
    type = t;
    vector = v;
}

Type :: Type (const SimpleType &v)
    : type (v.get_type())
{
    vector.push_back(v);
}

Type :: Type (const Part &p)
{
    type = p.value->type;
    for (auto i = p.pos.begin(); i != p.pos.end(); ++i) {
        vector.push_back(p.value->at(*i));
    }
}

std::string
Type :: to_string () const
{
    std::string result;
    if (vector.empty()) {
        result = "NULL(0)";
        return result;
    }
    for (auto i = vector.begin(); i != vector.end(); ++i) {
        result += i->to_string() + ' ';
    }

    return result;
}

void
Type :: resize (int new_sz)
{
    while (new_sz > vector.size()) {
        vector.push_back(SimpleType());
    }
}

SimpleType &
Type :: at (int index)
{
    resize(index + 1);
    return vector[index];
}

Type
Type :: cycle_new (int new_sz) const
{
    Type result = *this;
    int sz = size();
    
    for (int i = size(); i < new_sz; ++i) {
        result.vector.push_back(result.at(i % sz));
    }
    return result;
}

int
Type :: size() const
{
    return vector.size();
}

Type
operation  (const Type& a, const Type& b, int op)
{
    Type result;
    int sz = std::max(a.size(), b.size());
    auto a1 = a.cycle_new(sz);
    auto b1 = b.cycle_new(sz);
            
    for (int i = 0; i < sz; ++i) {
        switch (op) {
        case LEX_PLUS:
            result.at(i) = a1.at(i) + b1.at(i);
            break;
        case LEX_MINUS:
            result.at(i) = a1.at(i) - b1.at(i);
            break;
        case LEX_MUL:
            result.at(i) = a1.at(i) * b1.at(i);
            break;
        case LEX_DIV:
            result.at(i) = a1.at(i) / b1.at(i);
            break;
        case LEX_AND:
            result.at(i) = a1.at(i) & b1.at(i);
            break;
        case LEX_OR:
            result.at(i) = a1.at(i) | b1.at(i);
            break;
        case LEX_EQ:
            result.at(i) = a1.at(i) == b1.at(i);
            break;
        case LEX_NE:
            result.at(i) = a1.at(i) != b1.at(i);
            break;
        case LEX_LT:
            result.at(i) = a1.at(i) < b1.at(i);
            break;
        case LEX_LE:
            result.at(i) = a1.at(i) <= b1.at(i);
            break;
        case LEX_GT:
            result.at(i) = a1.at(i) > b1.at(i);
            break;
        case LEX_GE:
            result.at(i) = a1.at(i) >= b1.at(i);
            break;
        case LEX_TWODOTS:
            return two_dots(a, b);
        default:
            break;
        }
    }
    for (auto &i : result.vector) {
        if(!i.is_null()) {
            result.type = i.type;
            return result;
        }
    }
    result.type = TYPE_NULL;
    return result;
}

Type
Type :: operator! () const
{
    Type result = *this;
    for (int i = 0; i < this->size(); ++i) {
        result.at(i) = !result.at(i);
    }
    return result;
}

Type
two_dots (const Type &a, const Type &b)
{
        if (a.size() == 0
                        || b.size() == 0
                        || a.vector[0].is_null()
                        || b.vector[0].is_null()
                        || a.is(TYPE_CHARACTER)
                        || b.is(TYPE_CHARACTER)
                        || a.is(TYPE_FUNCTIONAL)
                        || b.is(TYPE_FUNCTIONAL))
                throw "Неверные аргументы :";

        Type result;
        result.type = TYPE_NUMERAL;
        auto a1 = a.vector[0];
        auto b1 = b.vector[0];
        if ((a1 < b1).get_log()) {
                for (SimpleType i = a1; (i <= b1).get_log(); i = i + 1.) {
                        result.vector.push_back(i);
                }
        } else {
                for (SimpleType i = a1; (i >= b1).get_log(); i = i - 1.) {
                        result.vector.push_back(i);
                }
        }
        return result;
}

Part
Type :: operator[] (const Type &t)
{
        if (t.is(TYPE_NUMERAL)) {
                Part result;
                result.value = this;
                for (int i = 0; i < t.size(); ++i) {
                        if (!t.vector[i].is_null()) {
                                resize(t.vector[i].get_num());
                                result.pos.push_back(t.vector[i].get_num() - 1);
                        }
                }
                return result;
        }
    
        if (t.is(TYPE_LOGICAL)) {
                Part result;
                result.value = this;
                Type new_t = t.cycle_new(size());
                for (int i = 0; i < t.size(); ++i) {
                        if (new_t.vector[i].get_log()) {
                                resize(i+1);
                                result.pos.push_back(i);
                        }  
                }
                return result;
        }
        
        throw "Неверный индекс";
}

bool
Type :: is (type_t t) const
{
        return t == type;
}