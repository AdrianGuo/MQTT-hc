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
#include <ZbDriver.hpp>
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
ForwardDimmerOrCurtainStateToOutside(
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
    if(device->Endpoint.GetValue() != 1) {
        Device_t masterDevice = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint =?").Bind(device->Network.GetValue()).Bind(1);
        val["power"] = std::to_string(masterDevice.Modify()->Action[DI_Power].DP_AttributeData);
    } else {
        val["power"] = std::to_string(device->Action[DI_Power].DP_AttributeData);
    }
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
            DEBUG1("________IR: Idle________");
            break;

        case 0x01:
            DEBUG1("________IR: Learning________");
            break;

        case 0x02:
            DEBUG1("________IR: Timeouted________");
            break;

        case 0x04:
            DEBUG1("________IR: Full Mem________");
            break;

        case 0x03: {
            ZbDeviceDb_p tmpIr = new ZbDeviceDb();
            tmpIr->DeviceID = device->Action[DeviceInfo::DI_State].DP_AttributeData;
            tmpIr->Type = 1; //Enable
            tmpIr->Model = String("IR-CMD");
            tmpIr->Device = DbPtr<ZbDeviceDb>(device);
            ZbDriver::s_pZbModel->Add(tmpIr);
            ZbDriver::s_pZbModel->UpdateChanges();
            ZbSocketCmd::GetInstance()->SendIrRes(DbPtr<ZbDeviceDb>(device), 0, device->Action[DeviceInfo::DI_State].DP_AttributeData);
        }
            break;

        case 0x05:
        case 0x06: {
            ZbSocketCmd::GetInstance()->SendIrRes(DbPtr<ZbDeviceDb>(device), 0, device->Action[DeviceInfo::DI_State].DP_AttributeData);
            Device_t ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Model=?").Bind(device->Action[DeviceInfo::DI_State].DP_AttributeData).Bind(String("IR-CMD"));
            if(ircmd.Modify() != NULL) {
                ircmd.Remove();
                ZbDriver::s_pZbModel->UpdateChanges();
            }
        }
            break;

        case 0x07:
            ZbSocketCmd::GetInstance()->SendIrRes(DbPtr<ZbDeviceDb>(device), 6, device->Action[DeviceInfo::DI_State].DP_AttributeData);
            break;


        case 0x08:
            DEBUG1("________IR: Overloaded________");
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

