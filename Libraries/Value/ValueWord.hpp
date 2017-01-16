#ifndef VALUE_WORD_HPP_
#define VALUE_WORD_HPP_

#include "typedefs.h"
#include "Value.hpp"

class ValueWord : public Value {
private:
    u16_t m_wValue;
public:
    ValueWord(u16_t wValue = 0);
    virtual ~ValueWord();

    u16_t  GetValue() const;
    void_t SetValue(u16_t wValue);

    ValueWord& operator= (ValueWord& rhs);
    ValueWord& operator= (u16_t wValue);

    bool_t operator== (const ValueWord& rhs) const;
    bool_t operator!= (const ValueWord& rhs) const;
    bool_t operator>  (const ValueWord& rhs) const;
    bool_t operator>= (const ValueWord& rhs) const;
    bool_t operator<  (const ValueWord& rhs) const;
    bool_t operator<= (const ValueWord& rhs) const;

    bool_t operator== (const u16_t wValue) const;
    bool_t operator!= (const u16_t wValue) const;
    bool_t operator>  (const u16_t wValue) const;
    bool_t operator>= (const u16_t wValue) const;
    bool_t operator<  (const u16_t wValue) const;
    bool_t operator<= (const u16_t wValue) const;

    friend bool_t operator!= (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator== (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator>  (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator>= (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator<  (const u16_t lhs, const ValueWord& rhs);
    friend bool_t operator<= (const u16_t lhs, const ValueWord& rhs);
};

typedef ValueWord  ValueWord_t;
typedef ValueWord* ValueWord_p;

#endif /* !VALUE_WORD_HPP_ */
