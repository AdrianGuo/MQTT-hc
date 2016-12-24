#ifndef VALUE_BOOL_HPP_
#define VALUE_BOOL_HPP_

#include "typedefs.h"
#include "Value.hpp"

class ValueBool : public Value {
private:
    bool_t m_boValue;
public:
    ValueBool(bool_t boValue = FALSE);
    virtual ~ValueBool();

    bool_t GetValue() const;
    void_t SetValue(bool_t boValue);

    ValueBool& operator= (bool_t boValue);

    bool_t operator== (const ValueBool& rhs) const;
    bool_t operator!= (const ValueBool& rhs) const;

    bool_t operator== (const bool_t boValue) const;
    bool_t operator!= (const bool_t boValue) const;

    friend bool_t operator!= (const bool_t lhs, const ValueBool& rhs);
    friend bool_t operator== (const bool_t lhs, const ValueBool& rhs);
};

typedef ValueBool  ValueBool_t;
typedef ValueBool* ValueBool_p;

#endif /* !VALUE_BOOL_HPP_ */
