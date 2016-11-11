/*
 * ZbDeviceControl.hpp
 *
 *  Created on: Nov 5, 2016
 *      Author: taho
 */

#ifndef ZBDEVICECONTROL_HPP_
#define ZBDEVICECONTROL_HPP_

#include <typedefs.h>
#include <debug.hpp>
#include <ZbSocketCmd.hpp>
#include <ZbModelDb.hpp>
#include <json.h>


/*****************************************************************************/
/***                            To Outside                                 ***/
/*****************************************************************************/

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardStateToOutside(
    const ZbDeviceDb_p device
) {
    Json::Value val;
    val["level"] = std::to_string(device->State);
    if(device->State == 0) {
        val["state"] = std::string("off");
    } else {
        val["state"] = std::string("on");
    }
    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardDimmerStateToOutside(
    const ZbDeviceDb_p device
) {
    int_t iLevel = device->State;
    if (iLevel <= 165)
        iLevel /= 3;
    else if (iLevel > 165)
        iLevel = 55 + (iLevel - 165) / 2;
    else
        iLevel = 0;
    Json::Value val;
    int_t state = device->Action[DI_OnOff].DP_AttributeData;
    val["level"] = std::to_string(iLevel);
    if(state == 1) {
        val["state"] = std::string("on");
    } else {
        val["state"] = std::string("off");
    }

    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardFanStateToOutside(
    const ZbDeviceDb_p device
) {
    int_t iLevel = (device->State)/63;
    Json::Value val;
    val["level"] = std::to_string(iLevel);
    val["state"] = std::to_string(device->Action[DI_OnOff].DP_AttributeData);
    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSensorStateToOutside(
    const ZbDeviceDb_p device
) {
    Json::Value val;
    val["level"] = std::to_string(device->State);
    val["power"] = std::to_string(device->Action[DI_Power].DP_AttributeData);
    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**ower
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardRGBStateToOutside(
    const ZbDeviceDb_p device
) {
    Json::Value val;
    std::string level;
    level = std::to_string(device->Action[DI_RGB_Red].DP_AttributeData) + ":"
            + std::to_string(device->Action[DI_RGB_Green].DP_AttributeData) + ":"
            + std::to_string(device->Action[DI_RGB_Blue].DP_AttributeData);
    val["level"] = level;
    if(device->Action[DI_State].DP_AttributeData == 1) {
        val["state"] = std::string("on");
    } else {
        val["state"] = std::string("off");
    }
    val["time"] = std::to_string(device->Action[DI_RGB_RemainingTime].DP_AttributeData);
    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardDaikinStateToOutside(
    const ZbDeviceDb_p device
) {
    Json::Value val;
    for(Action_t::const_iterator_t it = device->Action.begin(); it != device->Action.end(); it++) {
        if(it->second.DP_IsChanged == TRUE) {
            val[it->second.DP_DIStringName] = std::to_string(it->second.DP_AttributeData);
            device->Action[it->first].DP_IsChanged = FALSE;
        }
    }
    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardIrState(
    ZbDeviceDb_p device
){
    switch (device->State) {
        case 0x00:
        case 0x01:
        case 0x02:
            break;

        case 0x03: {
            ZbDeviceDb_p tmpIr = new ZbDeviceDb();
            tmpIr->DeviceID = device->Action[DeviceInfo::DI_State].DP_AttributeData;
            tmpIr->Type = 1001;
            tmpIr->Device = DbPtr<ZbDeviceDb>(device);
            ZbSocketCmd::GetInstance()->SendIrRes(DbPtr<ZbDeviceDb>(device), 0, device->Action[DeviceInfo::DI_State].DP_AttributeData);
        }
            break;

        case 0x04:

            break;

        case 0x05:

            break;

        case 0x06:

            break;

        case 0x07:

            break;


        case 0x08:

            break;

        default:
            break;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardForErrorProcessing(
    const ZbDeviceDb_p device
) {
    ZbZdoCmd::GetInstance()->LeaveRequest((u16_t) device->Network.GetValue());
}

#endif /* ZBDEVICECONTROL_HPP_ */

