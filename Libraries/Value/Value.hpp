#ifndef VALUE_HPP_
#define VALUE_HPP_

#include "typedefs.h"

class Value {
public:
    typedef enum {
        type_none = 0,     // Stanadadr Type
        type_bool,
        type_byte,
        type_decimal,
        type_interger,
        type_word,
        type_dword,
        type_string,
        type_array,
        type_switch,
        type_dimmer,
        type_rgb,
        type_pir_ac,
        type_pir_sensor,
        type_door_sensor,
        type_temp_sensor,
        type_humi_sensor,
        type_light_sensor
    } Type_t;
private:
    Type_t m_valueType;
    bool_t m_boChanged;
public:
    Value(Type_t valueType = type_none) : m_boChanged (FALSE) {
        m_valueType = valueType;
    }
    virtual ~Value() {}

    virtual void_t SetType(Type_t valueType) { m_valueType = valueType; }
    virtual Type_t GetType() const { return m_valueType; }

    virtual void_t SetChange() { m_boChanged = TRUE; }
    virtual void_t ResetChange() { m_boChanged = FALSE; }
    virtual bool_t IsChanged() const { return m_boChanged; }
};

typedef Value  Value_t;
typedef Value* Value_p;

#endif  /* VALUE_HPP_ */
