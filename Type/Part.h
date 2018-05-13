#ifndef PART_H_
#define PART_H_ 

#include <vector>

class Type;

struct Part
{
    friend class Type;

    Type *value;
    std::vector <int> pos;
public:
    Part& operator=(const Type &);
    Part& operator=(const Part &p) {
        value = p.value;
        pos = p.pos;
        return *this;
    }
};

#endif