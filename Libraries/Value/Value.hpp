#ifndef VALUE_HPP_
#define VALUE_HPP_

#include "Typedefs.h"
#include "ValueCore.hpp"

class Value : public ValueCore {
public:
    typedef enum {
        type_none = 0,     // Standard Type
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
        type_light_sensor,
        type_event,
        type_maximum
    } Type_t;
protected:
    Type_t m_valueType;
    void_t Swap(Value& other);
private:
    bool_t m_boChanged;
public:
    Value(Type_t valueType = type_none, const_char_p pName = "Value");
    Value(const Value& copied);
    virtual ~Value();

//    virtual void_t SetType(Type_t valueType) { m_valueType = valueType; }
//    virtual Type_t GetType() const { return m_valueType; }

    virtual void_t SetChange() { m_boChanged = TRUE; }
    virtual void_t ResetChange() { m_boChanged = FALSE; }
    virtual bool_t IsChanged() const { return m_boChanged; }

    Value& operator= (const Value& copied);

    virtual Type_t GetType() const;
    virtual void_t SetType(Type_t valueType);

    virtual bool_t operator== (const Value& rhs) const;
    virtual bool_t operator!= (const Value& rhs) const;
    virtual bool_t operator>= (const Value& rhs) const;
    virtual bool_t operator<= (const Value& rhs) const;
    virtual bool_t operator>  (const Value& rhs) const;
    virtual bool_t operator<  (const Value& rhs) const;
};

typedef Value  Value_t;
typedef Value* Value_p;

#endif  /* VALUE_HPP_ */
