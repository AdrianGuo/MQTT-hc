/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValuePir.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 14 Nov 2016 11:33:37
 *
 ******************************************************************************/
#ifndef CONTROLLER_VALUEDEVICE_VALUEPIRSENSOR_HPP_
#define CONTROLLER_VALUEDEVICE_VALUEPIRSENSOR_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "ValueDevice.hpp"

class ValuePirSensor : public ValueDevice {
private:
    u8_t m_byLevel;
    u8_t m_byPower;
public:
    ValuePirSensor(Json::Value jsonValue);
    ValuePirSensor(u8_t byLevel = 0, u8_t byPower = 0);

    static ValueDevice_p CreateValue(Json::Value jsonValue);

    virtual ~ValuePirSensor() {}

    static  Value::Type_t ValueType() { return Value::Type_t::type_pir_sensor; }
    virtual Json::Value CreateJson();

    virtual bool_t CheckParam() const { return TRUE; }
    virtual bool_t CheckJsVal(Json::Value jValue) const { return TRUE; }
    virtual bool_t ParseValue(Json::Value jsonValue);

    u8_t   Level() const { return m_byLevel; }
    u8_t   Power() const { return m_byPower; }

    void_t SetLevel(u8_t byLevel) { m_byLevel = byLevel; }
    void_t SetPower(u8_t byPower) { m_byPower = byPower; }
};

typedef ValuePirSensor  ValuePirSensor_t;
typedef ValuePirSensor* ValuePirSensor_p;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValuePirSensor::ValuePirSensor(
    Json::Value jsonValue
) : ValueDevice (Value::Type_t::type_pir_sensor) {
    ParseValue(jsonValue);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValuePirSensor::ValuePirSensor(
    u8_t byLevel,
    u8_t byPower
) : ValueDevice (Value::Type_t::type_pir_sensor),
    m_byLevel (byLevel),
    m_byPower (byPower) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline ValueDevice_p
ValuePirSensor::CreateValue(Json::Value jsonValue) {
    return new ValuePirSensor(jsonValue);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline Json::Value
ValuePirSensor::CreateJson() {
    Json::Value jsonValue;

    jsonValue["level"] = std::to_string(m_byLevel);
    jsonValue["power"] = std::to_string(m_byPower);

    return jsonValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
ValuePirSensor::ParseValue(
    Json::Value jsonValue
) {
    if (!jsonValue.isMember("level") ||
        !jsonValue["power"]) { return FALSE; }

    m_byLevel = std::stoi(jsonValue["level"].asCString());
    m_byPower = std::stoi(jsonValue["power"].asCString());

    return TRUE;
}

#endif /* CONTROLLER_VALUEDEVICE_VALUEPIRSENSOR_HPP_ */
