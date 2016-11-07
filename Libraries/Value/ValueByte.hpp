#ifndef VALUE_BYTE_HPP
#define VALUE_BYTE_HPP

#include "typedefs.h"
#include "Value.hpp"

class ValueByte : public Value {
private:
    u8_t m_byValue;
public:
    ValueByte(u8_t byValue = 0);
    virtual ~ValueByte();

    u8_t GetValue() const;
    void_t SetValue(u8_t byValue);

    ValueByte& operator= (ValueByte& rhs);
    ValueByte& operator= (u8_t byValue);

    bool_t operator== (ValueByte& rhs) const;
    bool_t operator== (const u8_t byValue) const;

    bool_t operator!= (ValueByte& rhs) const;
    bool_t operator!= (const u8_t byValue) const;

    bool_t operator>  (ValueByte& rhs) const;
    bool_t operator>  (const u8_t byValue) const;

    bool_t operator>= (ValueByte& rhs) const;
    bool_t operator>= (const u8_t byValue) const;

    bool_t operator<  (ValueByte& rhs) const;
    bool_t operator<  (const u8_t byValue) const;

    bool_t operator<= (ValueByte& rhs) const;
    bool_t operator<= (const u8_t byValue) const;

    operator u8_t() const { return m_byValue; }
};

typedef ValueByte ValueByte_t;
typedef ValueByte* ValueByte_p;

#endif  /* !VALUE_BYTE_HPP */

