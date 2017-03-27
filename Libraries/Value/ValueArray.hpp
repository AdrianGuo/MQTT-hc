#ifndef VALUE_ARRAY_HPP_
#define VALUE_ARRAY_HPP_

#include "Typedefs.h"
#include "Value.hpp"

class ValueArray : public Value {
private:
    u8_p m_pbyValue;
    u8_t m_byLength;
    u8_t m_byCount;
public:
    ValueArray(u8_t byLength = 0);
    virtual ~ValueArray();

    u8_t GetLength();

    u8_p GetValue();
    bool_t PushValue(u8_t byValue);
    bool_t PushArray(u8_p pBuffer, u8_t byLength);
};

typedef ValueArray  ValueArray_t;
typedef ValueArray* ValueArray_p;

#endif /* !VALUE_BYTEARRAY_HPP_ */
