#ifndef TYPE_H_
#define TYPE_H_

#include "../Scanner/Lex.h"
#include "SimpleType.h"
#include "Part.h"

#include <vector>
#include <string>



class Type
{
        friend class Part;
public:
        type_t type;
        std::vector<SimpleType> vector;

        void            resize          (int);
        SimpleType&     at              (int);
        int             size            () const;
        Type            cycle_new       (int new_sz) const;

public:
        Type ();
        Type (const SimpleType&);
        Type (const std::vector<SimpleType>&);
        Type (const Part&);
        Type (const Type&) = default;

        Type&  operator= (const Type&) = default;

        friend Type operation  (const Type&, const Type&, int) ;
        friend Type two_dots (const Type&, const Type&);
        Type  operator!  () const;
        Part  operator[] (const Type&);

        std::string to_string() const;
        bool is (type_t) const;
};

#endif