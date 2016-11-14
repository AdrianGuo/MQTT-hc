#ifndef VALUE_DWORD_HPP_
#define VALUE_DWORD_HPP_

#include "typedefs.h"
#include "Value.hpp"

class ValueDword : public Value {
private:
    u32_t m_dwValue;
public:
    ValueDword(u32_t dwValue = 0);
    virtual ~ValueDword();

    u32_t GetValue() const;
    void_t SetValue(u32_t dwValue);

    ValueDword& operator= (ValueDword& rhs);
    ValueDword& operator= (u32_t dwValue);

    bool_t operator== (ValueDword& rhs) const;
    bool_t operator== (const u32_t dwValue) const;

    bool_t operator!= (ValueDword& rhs) const;
    bool_t operator!= (const u32_t dwValue) const;

    bool_t operator>  (ValueDword& rhs) const;
    bool_t operator>  (const u32_t dwValue) const;

    bool_t operator>= (ValueDword& rhs) const;
    bool_t operator>= (const u32_t dwValue) const;

    bool_t operator<  (ValueDword& rhs) const;
    bool_t operator<  (const u32_t dwValue) const;

    bool_t operator<= (ValueDword& rhs) const;
    bool_t operator<= (const u32_t dwValue) const;

    operator u32_t() const { return m_dwValue; }
};

typedef ValueDword  ValueDword_t;
typedef ValueDword* ValueDword_p;

#endif /* !VALUE_WORD_HPP_ */
