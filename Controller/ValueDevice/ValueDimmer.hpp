/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDimmer.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 9 Nov 2016 23:59:18
 *
 ******************************************************************************/
#ifndef VALUE_DIMMER_HPP_
#define VALUE_DIMMER_HPP_

#include "Typedefs.h"
#include "String.hpp"
#include "ValueDevice.hpp"

class ValueDimmer : public ValueDevice {
private:
    u8_t    m_byLevel;
    String  m_strState;
public:
    ValueDimmer(Json::Value jsonValue);
    ValueDimmer(u8_t byLevel = 0, String strState = String());
    static ValueDevice_p CreateValue(Json::Value jsonValue);
    virtual ~ValueDimmer() {}

    static  Value::Type_t ValueType() { return Value::Type_t::type_dimmer; }
    virtual Json::Value CreateJson();

    virtual bool_t CheckParam() const;
    virtual bool_t CheckJsVal(Json::Value jsonValue) const;
    virtual bool_t ParseValue(Json::Value jsonValue);

    u8_t   Level() const { return m_byLevel;  }
    String State() const { return m_strState; }

    void_t SetLevel(u8_t byLevel) { m_byLevel = byLevel; }
    void_t SetState(String strState) { m_strState = strState; }
};

typedef ValueDimmer  ValueDimmer_t;
typedef ValueDimmer* ValueDimmer_p;

/**
 * @func   ValueDimmer
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueDimmer::ValueDimmer(
    Json::Value jsonValue
) : ValueDevice (Value::Type_t::type_dimmer) {
    ParseValue(jsonValue);
}

/**
 * @func   ValueDimmer
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueDimmer::ValueDimmer(
    u8_t byLevel,
    String strState
) : ValueDevice (Value::Type_t::type_dimmer),
    m_byLevel  (byLevel),
    m_strState (strState) {

}

/**
 * @func   CreateValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline ValueDevice_p
ValueDimmer::CreateValue(
    Json::Value jsonValue
) {
    return new ValueDimmer(jsonValue);
}

/**
 * @func   CreateJson
 * @brief  None
 * @param  None
 * @retval None
 */
inline Json::Value
ValueDimmer::CreateJson() {
    Json::Value jsonValue;

    jsonValue["level"] = std::to_string(m_byLevel);
    jsonValue["state"] = m_strState.c_str();

    return jsonValue;
}

/**
 * @func   CheckParam
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueDimmer::CheckParam() const {
    return TRUE;
}

/**
 * @func   CheckJsVal
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueDimmer::CheckJsVal(
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    return TRUE;
}

/**
 * @func   ParseValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueDimmer::ParseValue(
    Json::Value jsonValue
) {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    m_strState = jsonValue["state"].asCString();
    m_byLevel  = std::stoi(jsonValue["level"].asCString());

    return TRUE;
}

#endif /* VALUE_DIMMER_HPP_ */
