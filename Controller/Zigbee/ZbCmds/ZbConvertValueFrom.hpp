/*
 * ZbDeviceControl.hpp
 *
 *  Created on: Nov 5, 2016
 *      Author: taho
 */

#ifndef ZBDEVICECONTROL_HPP_
#define ZBDEVICECONTROL_HPP_

#include <Typedefs.h>
#include <debug.hpp>
#include <LogPlus.hpp>
#include <ZbSocketCmd.hpp>
#include <ZbModelDb.hpp>
#include <ZbDriver.hpp>
#include <json.h>

#include "SMQTT.hpp"


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
//    Json::Value val;
//    val["level"] = std::to_string(device->State);
//    if(device->State == 0) {
//        val["state"] = std::string("off");
//    } else {
//        val["state"] = std::string("on");
//    }
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
    SMQTT::s_pInstance->Publish(device->Name, device->Action[DI_State].DP_AttributeData);
    device->idwNumTimesNotReply = 0;
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
    if((state == 0) || (iLevel == 0)) {
        val["state"] = std::string("off");
    } else {
        val["state"] = std::string("on");
    }

//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardCurtainStateToOutside(
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
//    int_t state = device->Action[DI_OnOff].DP_AttributeData;
    val["level"] = std::to_string(iLevel);
    if(device->ReserveData(DI_State) == 1) {
        val["state"] = std::string("stop");
        device->ReserveData(DI_State) = 0;
    } else {
        if(iLevel == 100) {
            val["state"] = std::string("off");
        } else {
            val["state"] = std::string("on");
        }
    }
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
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
    if(device->State > 0) {
        val["state"] = std::string("on");
    } else {
        val["state"] = std::string("off");
    }
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
//void_t
//ForwardSensorStateToOutside(
//    const ZbDeviceDb_p device
//) {
//    Json::Value val;
//    val["level"] = std::to_string(device->State);
//    if(device->Endpoint.GetValue() != 1) {
//        Device_t masterDevice = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint =?").Bind(device->Network.GetValue()).Bind(1);
//        val["power"] = std::to_string(masterDevice.Modify()->Action[DI_Power].DP_AttributeData);
//    } else {
//        val["power"] = std::to_string(device->Action[DI_Power].DP_AttributeData);
//    }
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
//}

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
//    val["time"] = std::to_string(device->Action[DI_RGB_RemainingTime].DP_AttributeData);
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
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
    for(Action_t::const_iterator it = device->Action.begin(); it != device->Action.end(); it++) {
        if(it->second.DP_IsResponsed == TRUE) {
           if(it->first == DI_State) {
               if(it->second.DP_AttributeData == 1) val[it->second.DP_DIStringName] = std::string("on");
               else val[it->second.DP_DIStringName] = std::string("off");
           } else {
               val[it->second.DP_DIStringName] = std::to_string(it->second.DP_AttributeData);
           }
            device->Action[it->first].DP_IsResponsed = FALSE;
        }
    }
//    if (val.size() != 0)
//        ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), val);
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
    Json::Value jsonRetVal;
    jsonRetVal["act"] = std::to_string(device->Action[DI_IR_Data].DP_ReserveData);
    DEBUG2("Received IR State: %d", device->State);
    switch (device->State) {
        case 0x00:
            jsonRetVal["state"] = std::to_string(6);
            break;

        case 0x01:
            jsonRetVal["state"] = std::to_string(1);
            break;

        case 0x02:
            jsonRetVal["state"] = std::to_string(2);
            break;

        case 0x03: {
            DEBUG2("Add new IrCmd %04X,", device->Action[DI_State].DP_ReserveData);
            Device_t pdevice = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").
                    Bind(device->Network.GetValue()).Bind(device->Endpoint.GetValue());
            if(pdevice.get() == NULL) { return; }

            Device_t ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Network=?").
                    Bind(device->Action[DI_State].DP_ReserveData).Bind(device->Network.GetValue());
            if(ircmd.get() == NULL) {
                    ZbDeviceDb_p pZbDevice = new ZbDeviceDb();
                    pZbDevice->DeviceID  = device->Action[DI_State].DP_ReserveData;
                    pZbDevice->Network = device->Network.GetValue();
                    pZbDevice->Model = String("IR-CMD");
                    pZbDevice->Endpoint = 1;
                    pZbDevice->ParentDevice = pdevice;
                    ZbDriver::s_pZbModel->Add(pZbDevice);
                    ZbDriver::s_pZbModel->UpdateChanges();
            } else {
                ircmd.Modify()->Endpoint = 1;
                ircmd.Modify()->Model = String("IR-CMD");
                ZbDriver::s_pZbModel->Add(ircmd);
                ZbDriver::s_pZbModel->UpdateChanges();
            }
            jsonRetVal["state"] = std::to_string(0);
            jsonRetVal["irid"] = std::to_string(device->Action[DI_State].DP_ReserveData);
        }
            break;

        case 0x04:
            jsonRetVal["state"] = std::to_string(4);
            break;

        case 0x05:
        case 0x06: {
            jsonRetVal["state"] = std::to_string(0);
            jsonRetVal["irid"] = std::to_string(device->Action[DI_State].DP_ReserveData);
            if(device->State == 0x06) {
                Device_t ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Network=?").
                        Bind(device->Action[DI_State].DP_ReserveData).Bind(device->Network.GetValue());
                if(ircmd.get() != NULL) {
                    ircmd.Remove();
                    ZbDriver::s_pZbModel->UpdateChanges();
                }
            }
        }
            break;

        case 0x07:
            jsonRetVal["state"] = std::to_string(3);
            break;


        case 0x08:
            jsonRetVal["state"] = std::to_string(5);
            break;

        default:
            break;
    }
//    ZbSocketCmd::GetInstance()->SendZbStt(DbPtr<ZbDeviceDb>(device), jsonRetVal);
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

