/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueRGB.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 9 Nov 2016 23:59:33
 *
 ******************************************************************************/
#ifndef VALUE_RGB_HPP_
#define VALUE_RGB_HPP_

#include "typedefs.h"
#include "HelperHc.hpp"
#include "ValueDevice.hpp"

class ValueRGB : public ValueDevice {
private:
    u8_t m_byRed;
    u8_t m_byGreen;
    u8_t m_byBlue;
public:
    ValueRGB(u8_t byRed = 0, u8_t byGreen = 0, u8_t byBlue = 0) :
            ValueDevice (Value::Type_t::type_rgb),
            m_byRed   (byRed),
            m_byGreen (byGreen),
            m_byBlue  (byBlue) {}

    virtual ~ValueRGB() {}

    static  Value::Type_t ValueType() { return Value::Type_t::type_dimmer; }
    virtual Json::Value CreateJson();
    virtual bool_t ParseValue(Json::Value jValue) { return FALSE; }
};

typedef ValueRGB  ValueRGB_t;
typedef ValueRGB* ValueRGB_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline Json::Value
ValueRGB::CreateJson() {
    Json::Value jsonValue;

    String strLevel = "(" + GetHex(m_byRed)   + ":" + GetHex(m_byGreen) + ":" + GetHex(m_byBlue)  + ")";
    jsonValue["level"] = strLevel.c_str();
    return jsonValue;
}

#endif /* !VALUE_RGB_HPP_ */
