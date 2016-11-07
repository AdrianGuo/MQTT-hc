#ifndef VALUE_BASE_HPP_
#define VALUE_BASE_HPP_

#include "Value.hpp"
#include "Vector.hpp"
#include "String.hpp"

class ValueBase : public Value {
private:
    typedef struct ValueBaseElement {
        Value_p pAttrib;
        String strName;
    } ValueBaseElement_t, *ValueBaseElement_p;

    Vector<ValueBaseElement_p> m_vecElements;
public:
    ValueBase();
    virtual ~ValueBase();
    void_t Add(Value_p pAttrib, String strName);
    virtual void_t SetChange();
    virtual bool_t IsChanged();
    virtual void_t Process();
};

typedef ValueBase ValueBase_t;
typedef ValueBase* ValueBase_p;

#endif  /* !VALUE_BASE_HPP_ */
