#ifndef SIMPLETYPE_H_
#define SIMPLETYPE_H_

#include <string>

enum type_t
{
    TYPE_NULL,
    TYPE_NUMERAL,
    TYPE_LOGICAL,
    TYPE_CHARACTER,
    TYPE_FUNCTIONAL
};

class Functional;

class SimpleType
{
public:
    type_t   type;
    void    *value;

    double      *    num_val() const;
    bool        *    log_val() const;
    std::string *    chr_val() const;
    Functional  *    fun_val() const;

    void new_value (const SimpleType &);
    void dlt_value ();

    SimpleType to_num () const;
    SimpleType to_log () const;
    SimpleType to_chr () const;

    double      get_num () const;
    bool        get_log () const;
    std::string get_chr () const;
    Functional  get_fun () const;

public:

     SimpleType ();
     SimpleType (double);
     SimpleType (bool);
     SimpleType (const std::string &);
     SimpleType (const SimpleType &);
     SimpleType (const Functional &);
    ~SimpleType ();

    type_t get_type () const;
    
    bool is_null () const;
    bool is_num  () const;
    bool is_log  () const;
    bool is_chr  () const;
    bool is_fun  () const;

    SimpleType& operator=  (const SimpleType&);

    SimpleType  operator+  (const SimpleType&) const;
    SimpleType  operator-  (const SimpleType&) const;
    SimpleType  operator*  (const SimpleType&) const;
    SimpleType  operator/  (const SimpleType&) const;

    SimpleType  operator&  (const SimpleType&) const;
    SimpleType  operator|  (const SimpleType&) const;
    SimpleType  operator!  () const;

    SimpleType  operator== (const SimpleType&) const;
    SimpleType  operator!= (const SimpleType&) const;
    SimpleType  operator<  (const SimpleType&) const;
    SimpleType  operator<= (const SimpleType&) const;
    SimpleType  operator>  (const SimpleType&) const;
    SimpleType  operator>= (const SimpleType&) const;

    std::string to_string () const;
};

#endif