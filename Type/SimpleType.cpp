#include "Type.h"
#include "Functional.h"
#include <string>
#include <iostream>
#include <sstream>

double      * SimpleType:: num_val() const { return (double*)value;      }
bool        * SimpleType:: log_val() const { return (bool*)value;        }
std::string * SimpleType:: chr_val() const { return (std::string*)value; }
Functional  * SimpleType:: fun_val() const { return (Functional*)value;  }

void
SimpleType:: new_value(const SimpleType &v)
{
        switch (type) {
        case TYPE_NUMERAL:
                value = new double(*v.num_val());
                break;
        case TYPE_LOGICAL:
                value = new bool(*v.log_val());
                break;
        case TYPE_CHARACTER:
                value = new std::string(*v.chr_val());
                break;
        case TYPE_FUNCTIONAL:
                value = new Functional(*v.fun_val());
                break;
        default:
                value = nullptr;
        }
}

void
SimpleType:: dlt_value()
{
        switch (type) {
        case TYPE_NUMERAL:
                delete num_val();
                break;
        case TYPE_LOGICAL:
                delete log_val();
                break;
        case TYPE_CHARACTER:
                delete chr_val();
                break;
        case TYPE_FUNCTIONAL:
                delete fun_val();
                break;
        default:
                break;
        }
}


SimpleType::SimpleType()
    :type (TYPE_NULL)
{}

SimpleType::SimpleType(double v)
    :type (TYPE_NUMERAL)
{
    value = new double(v);
}

SimpleType::SimpleType(bool v)
    :type (TYPE_LOGICAL)
{
    value = new bool(v);
}

SimpleType::SimpleType(const std::string & v)
    :type (TYPE_CHARACTER)
{
    value = new std::string(v);
}

SimpleType :: SimpleType (const Functional &v)
    :type(TYPE_FUNCTIONAL)
{
    value = new Functional(v);
}

SimpleType::SimpleType(const SimpleType &v)
    :type(v.type)
{
    new_value(v);
}

SimpleType::~SimpleType()
{
    dlt_value();
}

type_t SimpleType::get_type() const
{
    return type;
}

bool SimpleType::is_null () const { return type == TYPE_NULL;       }
bool SimpleType::is_num  () const { return type == TYPE_NUMERAL;    }
bool SimpleType::is_log  () const { return type == TYPE_LOGICAL;    }
bool SimpleType::is_chr  () const { return type == TYPE_CHARACTER;  }
bool SimpleType::is_fun  () const { return type == TYPE_FUNCTIONAL; }

SimpleType
SimpleType::to_num()const
{
    switch (type) {
    case TYPE_NUMERAL:
        return *this;
    case TYPE_LOGICAL:
        return get_log() ? 1. : 0.;
    case TYPE_NULL:
        return SimpleType();
    default:
        throw "Wrong types of operation";
    }
}

SimpleType
SimpleType::to_log()const
{
    switch (type) {
    case TYPE_NUMERAL:
        return (get_num() != 0);
    case TYPE_LOGICAL:
        return get_log();
    case TYPE_NULL:
        return SimpleType();
    default:
        throw "Wrong types of operation";
    }
}

SimpleType
SimpleType::to_chr()const
{
    try {
        return to_string();
    }
    catch (...) {
        throw "Wrong types of operation";
    }
}

double      SimpleType::get_num() const { return *num_val(); }
bool        SimpleType::get_log() const { return *log_val(); }
std::string SimpleType::get_chr() const { return *chr_val(); }
Functional  SimpleType::get_fun() const { return *fun_val(); }



SimpleType & 
SimpleType::operator=(const SimpleType &v)
{
    dlt_value();
    type = v.type;
    new_value(v);
    return *this;
}

SimpleType
SimpleType::operator+(const SimpleType & a)const
{
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    /* Num + Num */
    else if (is_num() && a.is_num()) {
        return get_num() + a.get_num();
    }
    /* Chr + Chr */
    else if (is_chr() && a.is_chr()) {
        return get_chr() + a.get_chr();
    }
    /* Chr + Num */
    else if ((is_chr() && a.is_num()) || (is_num() && a.is_chr())) {
        return to_chr() + a.to_chr();
    }
    /* Num + Log */
    else if ((is_num() && a.is_log()) || (is_log() && a.is_num())) {
        return to_num() + a.to_num();
    } 
    /* Num + Null */
    else if ((is_num() && a.is_null()) || (is_null() && a.is_num())) {
        return SimpleType();
    }
    else {
        throw "Wrong types operation +";
    }
}

SimpleType SimpleType::operator-(const SimpleType &a) const
{
    /* Null - Null */
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    /* Num - Num */
    else if (is_num() && a.is_num()) {
        return get_num() - a.get_num();
    }
    /* Num - Log*/
    else if ((is_num() && a.is_log()) || (is_log() && a.is_num())) {
        return to_num() - a.to_num();
    }
    /* Num - Null */
    else if ((is_num() && a.is_null()) || (is_null() && a.is_num())) {
        return SimpleType();
    }
    else {
        throw "Wrong types operation -";
    }
}

SimpleType SimpleType::operator*(const SimpleType &a) const
{
    /* Null * Null */
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    /* Num * Num */
    else if (is_num() && a.is_num()) {
        return get_num() * a.get_num();
    }
    /* Num * Log */
    else if ((is_num() && a.is_log()) || (is_log() && a.is_num())) {
        
        return to_num() * a.to_num();
    }
    /* Num * Null */
    else if ((is_num() && a.is_null()) || (is_null() && a.is_num())) {
        return SimpleType();
    }
    else {
        throw "Wrong types operation *";
    }
}

SimpleType SimpleType::operator/(const SimpleType &a) const
{
    /* Null / Null */
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    /* Num / Num */
    else if (is_num() && a.is_num()) {
        return get_num() / a.get_num();
    }
    /* Num / Log */
    else if ((is_num() && a.is_log()) || (is_log() && a.is_num())) {
        return to_num() / a.to_num();
    }
    /* Num / Null */
    else if ((is_num() && a.is_null()) || (is_null() && a.is_num())) {
        return SimpleType();
    }
    else {
        throw "Wrong types operation /";
    }
}

SimpleType
SimpleType::operator&(const SimpleType &a) const
{
    /* Log & Log*/
    if (is_log() && a.is_log()) {
        return get_log() && a.get_log();
    }
    /* Log & Null */
    else if ((is_log() && a.is_null()) || (is_null() && a.is_log())) {
        return SimpleType();
    }
    /* Log & Num */
    else if ((is_log() && a.is_num()) || (is_num() && a.is_log())) {
        return to_log() & a.to_log();
    }
    else {
        throw "Wrong types operation &";
    }
}

SimpleType
SimpleType::operator|(const SimpleType &a) const
{
    /* Log | Log*/
    if (is_log() && a.is_log()) {
        return get_log() || a.get_log();
    }
    /* Log & Null */
    else if ((is_log() && a.is_null()) || (is_null() && a.is_log())) {
        return true;
    }
    /* Log & Num */
    else if ((is_log() && a.is_num()) || (is_num() && a.is_log())) {
        return to_log() | a.to_log();
    }
    else {
        throw "Wrong types operation &";
    }
}

SimpleType
SimpleType::operator!() const
{
    if (is_log()) {
        return !get_log();
    }
    else if (is_null()) {
        return SimpleType();
    }
    else if (is_num()) {
        return !to_log();
    }
    else {
        throw "Wrong type of operation !";
    }
}

SimpleType
SimpleType::operator==(const SimpleType &a) const
{
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    else if (is_num() && a.is_num()) {
        return get_num() == a.get_num();
    }
    else if (is_chr() && a.is_chr()) {
        return get_chr() == a.get_chr();
    }
    else if ((is_log() && a.is_num()) || (is_num() && a.is_log())) {
        return to_log() == a.to_log();
    }
    else if (is_null() || a.is_null()) {
        return false;
    }
    else if ((is_chr() || a.is_chr())) {
        try {
            return to_chr() == a.to_chr();
        }
        catch (...) {
            throw "Wrong types operation ==";
        }
    }
    else {
        throw "Wrong types operation ==";
    }
}

SimpleType
SimpleType::operator!=(const SimpleType & a) const
{
    return !operator==(a);
}

SimpleType 
SimpleType::operator< (const SimpleType& a) const
{
    if (is_null() && a.is_null()) {
        return SimpleType();
    }
    else if (is_num() && a.is_num()) {
        return get_num() < a.get_num();
    }
    else if (is_chr() && a.is_chr()) {
        return get_chr() < a.get_chr();
    }
    else if (is_null() || a.is_null()) {
        return SimpleType();
    }
    else if ((is_chr() || a.is_chr())) {
        try {
            return to_chr() < a.to_chr();
        }
        catch (...) {
            throw "Wrong types operation <";
        }
    }
    else {
        throw "Wrong types operation <";
    }
}

SimpleType
SimpleType::operator<=(const SimpleType &a) const
{
        if (is_null() && a.is_null()) {
            return SimpleType();
        }
        else if (is_num() && a.is_num()) {
            return get_num() <= a.get_num();
        }
        else if (is_chr() && a.is_chr()) {
            return get_chr() <= a.get_chr();
        }
        else if ((is_chr() || a.is_chr())) {
            try {
                return to_chr() <= a.to_chr();
            }
            catch (...) {
                throw "Wrong types operation <=";
            }
        }
        else {
            throw "Wrong types operation <=";
        }
    }

SimpleType
SimpleType::operator> (const SimpleType &a) const
{
    try {
        return !operator<=(a);
    }
    catch (...) {
        throw "Wrong types operation <=";
    }
}

SimpleType
SimpleType::operator>=(const SimpleType &a) const
{
    try {
        return ! operator< (a);
    }
    catch (...) {
        throw "Wrong types operation <=";
    }
}


std::string
SimpleType::to_string()const
{
    std::ostringstream iss;
    switch (type) {
    case TYPE_NUMERAL:
        iss << get_num();
        return iss.str();
    case TYPE_LOGICAL:
        return (get_log()) ? "TRUE" : "FALSE";
    case TYPE_CHARACTER:
        return get_chr();
    case TYPE_FUNCTIONAL:
        throw "Cannot print function";
    default:
        return "NULL";
    }
}
