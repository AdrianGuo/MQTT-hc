#ifndef VALUE_STRING_HPP
#define	VALUE_STRING_HPP

#include "Value.hpp"
#include "String.hpp"

class ValueString : public Value {
private:
    u8_t    m_byValueId;
    String  m_strValue;
public:
    ValueString(String strValue = "", u8_t byValueId = 0);
    virtual ~ValueString();

    u8_t GetValueId();
    void_t SetValueId(u8_t byValueId);

    String& GetValue();
    void_t PushBack(char c);
    void_t SetValue(String strValue);
    bool_t IsEqual(String strValue);
};

typedef ValueString  ValueString_t;
typedef ValueString* ValueString_p;

#endif	/* VALUE_STRING_HPP */

