#ifndef VALUE_INTEGER_HPP
#define VALUE_INTEGER_HPP

#include "Value.hpp"
#include "typedefs.h"

class ValueInt : public Value {
private:
    int_t m_iValue;
public:
    ValueInt(int_t iValue = 0);
    virtual ~ValueInt();

    int_t GetValue() const;
    void_t SetValue(int_t iValue);

    ValueInt& operator= (ValueInt& rhs);
    ValueInt& operator= (int_t iValue);

    bool_t operator== (const ValueInt& rhs) const;
    bool_t operator!= (const ValueInt& rhs) const;
    bool_t operator>  (const ValueInt& rhs) const;
    bool_t operator>= (const ValueInt& rhs) const;
    bool_t operator<  (const ValueInt& rhs) const;
    bool_t operator<= (const ValueInt& rhs) const;

    bool_t operator== (const int_t iValue) const;
    bool_t operator!= (const int_t iValue) const;
    bool_t operator>  (const int_t iValue) const;
    bool_t operator>= (const int_t iValue) const;
    bool_t operator<  (const int_t iValue) const;
    bool_t operator<= (const int_t iValue) const;

    friend bool_t operator!= (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator== (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator>  (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator>= (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator<  (const int_t lhs, const ValueInt& rhs);
    friend bool_t operator<= (const int_t lhs, const ValueInt& rhs);
};

typedef ValueInt  ValueInt_t;
typedef ValueInt* ValueInt_p;

#endif  /* !VALUE_INTEGER_HPP */

