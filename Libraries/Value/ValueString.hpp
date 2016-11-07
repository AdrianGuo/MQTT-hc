#ifndef VALUE_STRING_HPP
#define	VALUE_STRING_HPP

#include "Value.hpp"
#include "String.hpp"

class ValueString : public Value {
private:
    u8_t    m_byValueID;
    String  m_strValue;
public:
    ValueString(String strValue = "", u8_t byValueID = 0);
    virtual ~ValueString();

    u8_t GetValueID();
    void_t SetValueID(u8_t byValueID);

    String& GetValue();
    void_t SetValue(String strValue);
    bool_t IsEqual(String strValue);
};

typedef ValueString ValueString_t;
typedef ValueString* ValueString_p;

#endif	/* VALUE_STRING_HPP */

