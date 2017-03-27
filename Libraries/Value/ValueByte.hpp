#ifndef VALUE_BYTE_HPP
#define VALUE_BYTE_HPP

#include "Typedefs.h"
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

    bool_t operator!= (const ValueByte& rhs) const;
    bool_t operator== (const ValueByte& rhs) const;
    bool_t operator>  (const ValueByte& rhs) const;
    bool_t operator>= (const ValueByte& rhs) const;
    bool_t operator<  (const ValueByte& rhs) const;
    bool_t operator<= (const ValueByte& rhs) const;

    bool_t operator!= (const u8_t byValue) const;
    bool_t operator== (const u8_t byValue) const;
    bool_t operator>  (const u8_t byValue) const;
    bool_t operator>= (const u8_t byValue) const;
    bool_t operator<  (const u8_t byValue) const;
    bool_t operator<= (const u8_t byValue) const;

    friend bool_t operator!= (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator== (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator>  (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator>= (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator<  (const u8_t lhs, const ValueByte& rhs);
    friend bool_t operator<= (const u8_t lhs, const ValueByte& rhs);
};

typedef ValueByte  ValueByte_t;
typedef ValueByte* ValueByte_p;

#endif  /* !VALUE_BYTE_HPP */

