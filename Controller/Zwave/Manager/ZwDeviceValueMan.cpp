/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDeviceValueMan.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 08:57:39
 *
 ******************************************************************************/
#include "HcDevice.hpp"
#include "ValueDimmer.hpp"
#include "ValueSwitch.hpp"
#include "ValuePirSensor.hpp"
#include "ZwDeviceValueMan.hpp"

ZwDeviceValueMan_p ZwDeviceValueMan::m_pInstance = NULL;

/**
 * @func   ZwDeviceValueMan
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDeviceValueMan::ZwDeviceValueMan() { }

/**
 * @func   ~ZwDeviceValueMan
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDeviceValueMan::~ZwDeviceValueMan() { }

/**
 * @func   Register
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDeviceValueMan::Register(
    u8_t byTypeDev,
    funcCreateValue pFunctionCreator
) {
    m_mapTypeDeviceValue[byTypeDev] = pFunctionCreator;
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDeviceValueMan_p
ZwDeviceValueMan::GetInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new ZwDeviceValueMan();
    }
    return m_pInstance;
}

/**
 * @func   Register
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDeviceValueMan::Register() {
    Register(DEVICE_TYPE_CONTACT, ValueSwitch::CreateValue);
    Register(DEVICE_TYPE_DIMMER, ValueDimmer::CreateValue);
    Register(DEVICE_TYPE_PIR_SENSOR, ValuePirSensor::CreateValue);
}

/**
 * @func   CreateValueDevice
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
ZwDeviceValueMan::CreateValueDevice(
    u8_t byTypeDev,
    Json::Value jsonValue
) {
    Map<u8_t, funcCreateValue>::iterator_t it =
    m_mapTypeDeviceValue.find(byTypeDev);

    if (it != m_mapTypeDeviceValue.end()) {
        return it->second(jsonValue);
    }
    return NULL;
}
