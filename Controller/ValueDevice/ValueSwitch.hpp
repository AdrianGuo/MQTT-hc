/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueSwitch.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 9 Nov 2016 23:59:07
 *
 ******************************************************************************/
#ifndef VALUE_SWITCH_HPP_
#define VALUE_SWITCH_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "ValueDevice.hpp"

static const String StateON  = String("on");
static const String StateOFF = String("off");

class ValueSwitch : public ValueDevice {
private:
    u8_t    m_byLevel;
    String  m_strState;
public:
    static const u8_t LevelMAX = 100;

    ValueSwitch(Json::Value jsonValue);
    ValueSwitch(u8_t byLevel = 0, String strState = String());

    static ValueDevice_p CreateValue(Json::Value jsonValue);

    virtual ~ValueSwitch() {}

    static  Value::Type_t ValueType() { return Value::Type_t::type_switch; }
    virtual Json::Value CreateJson();

    virtual bool_t CheckParam() const;
    virtual bool_t CheckJsVal(Json::Value jsonValue) const;
    virtual bool_t ParseValue(Json::Value jsonValue);

    u8_t   Level() const { return m_byLevel;  }
    String State() const { return m_strState; }

    void_t SetLevel(u8_t byLevel)    { m_byLevel  = byLevel;  }
    void_t SetState(String strState) { m_strState = strState; }

    bool_t operator== (Json::Value jsonValue) const;
    bool_t operator!= (Json::Value jsonValue) const;
    bool_t operator>  (Json::Value jsonValue) const;
    bool_t operator<  (Json::Value jsonValue) const;
    bool_t operator>= (Json::Value jsonValue) const;
    bool_t operator<= (Json::Value jsonValue) const;
};

typedef ValueSwitch  ValueSwitch_t;
typedef ValueSwitch* ValueSwitch_p;

/**
 * @func   ValueSwitch
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueSwitch::ValueSwitch(
    Json::Value jsonValue
) : ValueDevice (Value::Type_t::type_switch) {
    ParseValue(jsonValue);
}

/**
 * @func   ValueSwitch
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueSwitch::ValueSwitch(
    u8_t byLevel,
    String strState
) : ValueDevice (Value::Type_t::type_switch),
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
ValueSwitch::CreateValue(
    Json::Value jsonValue
) {
    ValueSwitch_p pValueSwitch = new ValueSwitch(jsonValue);

    if (pValueSwitch->CheckParam()) {
        return pValueSwitch;
    }

    delete pValueSwitch;
    return NULL;
}

/**
 * @func   CreateJson
 * @brief  None
 * @param  None
 * @retval None
 */
inline Json::Value
ValueSwitch::CreateJson() {
    Json::Value jsonValue;

    jsonValue["level"] = std::to_string(m_byLevel);
    jsonValue["state"] = (m_byLevel > 0) ? "on" : "off";

    return jsonValue;
}

/**
 * @func   CheckJsVal
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::CheckParam() const {
    if (((m_byLevel >  0) && (m_strState == StateOFF)) ||
        ((m_byLevel == 0) && (m_strState == StateON )) ||
        (m_byLevel > LevelMAX)) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @func   CheckJsVal
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::CheckJsVal(
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    u8_t byLevel = std::stoi(jsonValue["level"].asCString());
    String strState = jsonValue["state"].asCString();

    if (((byLevel >  0) && (strState == StateOFF)) ||
        ((byLevel == 0) && (strState == StateON )) ||
        (byLevel > LevelMAX)) {
        return FALSE;
    }

    return TRUE;
}

/**
 * @func   ParseValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::ParseValue(
    Json::Value jsonValue
) {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (CheckJsVal(jsonValue)) {
        return FALSE;
    }

    m_strState = jsonValue["state"].asCString();
    m_byLevel  = std::stoi(jsonValue["level"].asCString());

    return TRUE;
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator== (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE; }

    u8_t byLevel = std::stoi(jsonValue["level"].asCString());
    String strState = jsonValue["state"].asCString();

    if ((m_byLevel == byLevel) && (m_strState == strState)) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator!= (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE; }

    u8_t byLevel = std::stoi(jsonValue["level"].asCString());
    String strState = jsonValue["state"].asCString();

    if ((m_byLevel != byLevel) || (m_strState != strState)) {
        return TRUE;
    }

    return FALSE;
}

/**
 * @func   operator>
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator> (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE;}

    return FALSE;
}

/**
 * @func   operator<
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator< (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE; }

    return FALSE;
}

/**
 * @func   operator>=
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator>= (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE; }

    return FALSE;
}

/**
 * @func   operator<=
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValueSwitch::operator<= (
    Json::Value jsonValue
) const {
    if (!jsonValue.isMember("level") || !jsonValue["state"]) { return FALSE; }

    if (!CheckJsVal(jsonValue)) { return FALSE; }

    return FALSE;
}

#endif /* !VALUE_SWITCH_HPP_ */
