#ifndef VALUE_ID_HPP
#define VALUE_ID_HPP

#include "typedefs.h"

class ValueID {
private:
public:
    enum ValueType {
        ValueType_Bool = 0,
        ValueType_Byte,
        ValueType_Decimal,
        ValueType_Int,
        ValueType_Word,
        ValueType_Dword,
        ValueType_String,
        ValueType_List,
        ValueType_Count
    } ValueType_t;
};


#endif /* VALUE_ID_HPP */
