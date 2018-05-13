#include "Part.h"
#include "Type.h"

Part &
Part :: operator = (const Type& t)
{
    if (t.size() > pos.size()) throw "Неверное присваивание part";

    Type new_t = t.cycle_new(pos.size());
    
    if (t.type == TYPE_CHARACTER && value->type != TYPE_FUNCTIONAL) {
        value->type = TYPE_CHARACTER;
        for (int i = 0; i < value->size(); ++i) {
            value->at(i) = value->at(i).to_chr();
        }        
    }

    for (int i = 0; i < pos.size(); ++i) {

        switch (value->type) {
            case TYPE_FUNCTIONAL:
                if (t.type != TYPE_FUNCTIONAL && t.type != TYPE_NULL) {
                    throw "В вектор-функцию непонятно что";
                }
                value->vector[pos[i]] = t.vector[i];
            case TYPE_NUMERAL:
            value->vector[pos[i]] = t.vector[i].to_num();
            break;
            case TYPE_LOGICAL:
            value->vector[pos[i]] = t.vector[i].to_log();
            break;
            case TYPE_CHARACTER:
            value->vector[pos[i]] = t.vector[i].to_chr();
            break;
            default:
            value->type = t.type;
            value->vector[pos[i]] = t.vector[i];
        }
        
    }
    return *this;
}