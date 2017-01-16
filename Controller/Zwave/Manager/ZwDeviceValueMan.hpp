/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDeviceValueMan.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 08:56:14
 *
 ******************************************************************************/
#ifndef ZW_DEVICEVALUE_MAN_HPP_
#define ZW_DEVICEVALUE_MAN_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "json.h"
#include "ValueDevice.hpp"
#include "ZwCmdClass.hpp"

typedef ValueDevice_p (*funcCreateValue)(Json::Value);

class ZwDeviceValueMan {
private:
    static ZwDeviceValueMan* m_pInstance;
    ZwDeviceValueMan();
    Map<u8_t, funcCreateValue> m_mapTypeDeviceValue;
    void_t Register(u8_t byTypeDev, funcCreateValue pFunctionCreator);
public:
    static ZwDeviceValueMan* GetInstance();
    virtual ~ZwDeviceValueMan();
    void_t Register();
    ValueDevice_p CreateValueDevice(u8_t byTypeDev, Json::Value jsonValue);
};

typedef ZwDeviceValueMan  ZwDeviceValueMan_t;
typedef ZwDeviceValueMan* ZwDeviceValueMan_p;

#endif /* !ZW_DEVICEVALUE_MAN_HPP_ */
