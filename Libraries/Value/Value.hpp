#ifndef VALUE_HPP_
#define VALUE_HPP_

#include "typedefs.h"

typedef enum ValueType {
    ValueType_None = 0,     // Stanadadr Type
    ValueType_Bool,
    ValueType_Byte,
    ValueType_Decimal,
    ValueType_Integer,
    ValueType_Word,
    ValueType_Dword,
    ValueType_String,
    ValueType_Array,
    ValueType_ZwType,       // Zwave Type
    ValueType_ZwDriver,
    ValueType_ZwNode,
    ValueType_ZwAppFunc,
    ValueType_ZwBasic,
    ValueType_ZwCtrller,
    ValueType_ZwFirmware,
    ValueType_ZwNvm,
    ValueType_ZwPower,
    ValueType_ZwTransport,
    ValueType_Count
} ValueType_t;

class Value {
private:
    ValueType_t m_valueType;
    bool_t m_boChanged;
public:
    Value(ValueType_t valueType = ValueType_None) : m_boChanged (FALSE) {
        m_valueType = valueType;
    }
    virtual ~Value() {}

    virtual void_t SetType(ValueType_t valueType) { m_valueType = valueType; }
    virtual ValueType_t GetType() const { return m_valueType; }

    virtual void_t SetChange() { m_boChanged = TRUE; }
    virtual void_t ResetChange() { m_boChanged = FALSE; }
    virtual bool_t IsChanged() const { return m_boChanged; }
    virtual void_t Process() { }

private:

};

typedef Value Value_t;
typedef Value* Value_p;

#endif  /* VALUE_HPP_ */
