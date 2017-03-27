/*
 * ZbConvertValueTo.hpp
 *
 *  Created on: Nov 10, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_
#define CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_

#include <Typedefs.h>
#include <debug.hpp>
#include <JsonCommand.hpp>
#include <ZbModelDb.hpp>
#include <ZbZclCmd.hpp>
#include <ZbZclGlobalCmd.hpp>
#include <DeviceInfo.hpp>
#include <LogPlus.hpp>

/*****************************************************************************/
/***                           From Outside                                ***/
/*****************************************************************************/

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToDevice (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("state")) { return; }
    u8_t val = 0x00;
    if(jsonVal["state"].asString() ==  std::string("on"))
    	val = 0x01;
    ZbZclCmd::GetInstance()->SetDevice(device, val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToDimmer (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    u8_t val = atoi(jsonVal["level"].asCString());

    if(jsonVal["state"].asString() == std::string("off")) {
        ZbZclCmd::GetInstance()->SetDevice(device, 0);
        return;
    }
    if (jsonVal["state"].asString() == std::string("on") && (jsonVal["level"].asString() == std::string("-1"))) {
        if(device.Modify()->Action[DI_State].DP_PreValue == 0) {
            ZbZclCmd::GetInstance()->SetDevice(device, 0xFF);
        } else {
            ZbZclCmd::GetInstance()->SetDevice(device, device.Modify()->Action[DI_State].DP_PreValue);
        }
    } else {
        if(val <= 55)
            val *= 3;
        else if(val > 55)
            val = 165 + (val - 55) * 2;
        else
            val = 0;

        if (jsonVal["state"].asString() == std::string("off")) {
            val = 0;
        } else if (jsonVal["state"].asString() == std::string("inc")) {
            val = device->State;
            if(val < 253)  val += 3;
        } else if (jsonVal["state"].asString() == std::string("dec")) {
            val = device->State;
            if(val > 2)  val -= 3;
        }
        ZbZclCmd::GetInstance()->SetDevice(device, val);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToCurtain (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    u8_t val = atoi(jsonVal["level"].asCString());

    if(jsonVal["state"].asString() == std::string("off")) {
        ZbZclCmd::GetInstance()->SetDevice(device, 255);
        return;
    }

    if(jsonVal["state"].asString() == std::string("stop")) {
        device.Modify()->Action[DI_State].DP_ReserveData = 1;
        ZbZclCmd::GetInstance()->SetDevice(device, (device->State));
        return;
    }

    if (jsonVal["state"].asString() == std::string("on") && (jsonVal["level"].asString() == std::string("-1"))) {
        ZbZclCmd::GetInstance()->SetDevice(device, device.Modify()->Action[DI_State].DP_PreValue);
    } else {
        if(val <= 55)
            val *= 3;
        else if(val > 55)
            val = 165 + (val - 55) * 2;
        else
            val = 0;

        if (jsonVal["state"].asString() == std::string("off")) {
            val = 0;
        } else if (jsonVal["state"].asString() == std::string("inc")) {
            val = device->State;
            if(val < 253)  val += 3;
        } else if (jsonVal["state"].asString() == std::string("dec")) {
            val = device->State;
            if(val > 2)  val -= 3;
        }
        ZbZclCmd::GetInstance()->SetDevice(device, val);
    }
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToFan (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    u8_t rcvVal = atoi(jsonVal["level"].asCString());

    if(jsonVal["state"].asString() == std::string("off")) {
        ZbZclCmd::GetInstance()->SetDevice(device, 0);
        return;
    }

    if ((jsonVal["state"].asString() == std::string("on")) && (jsonVal["level"].asString() == std::string("-1"))) {
        if(device.Modify()->Action[DI_State].DP_PreValue == 0) {
            ZbZclCmd::GetInstance()->SetDevice(device, 0xFF);
        } else {
            ZbZclCmd::GetInstance()->SetDevice(device, device.Modify()->Action[DI_State].DP_PreValue);
        }
    } else {
        if (jsonVal["state"].asString() == std::string("off")) {
            rcvVal = 0;
        } else if (jsonVal["state"].asString() == std::string("inc")) {
            rcvVal = device->State;
            if(rcvVal/63 < 4)  rcvVal = (rcvVal/63 + 1) * 63;
        } else if (jsonVal["state"].asString() == std::string("dec")) {
            rcvVal = device->State;
            if(rcvVal/63 > 0)  rcvVal = (rcvVal/63 -1)*63;
        }

        if(rcvVal > 0xFF) rcvVal = 0xFF;
        if(rcvVal < 0) rcvVal = 0x00;

        ZbZclCmd::GetInstance()->SetDevice(device, rcvVal * 63);
    }
}



/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSetValueToIr (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("act")) { return; }
    u8_t byAct = atoi(jsonVal["act"].asCString());
    bool_t isIrcmd = jsonVal.isMember("irid");
    Device_t ircmd;
    if(isIrcmd){
        ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Network=?").
            Bind(atoi(jsonVal["irid"].asCString())).Bind(device->Network.GetValue());
    }
//    ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_State);
//    while()

    device.Modify()->ReserveData(DI_IR_Data) = byAct;
    if(byAct == 0) { //Learn
        ZbZclCmd::GetInstance()->SetIR(device, IrCommand::IRCMD_Learn);
    } else if (byAct == 1) { //Stop
        ZbZclCmd::GetInstance()->SetIR(device, IrCommand::IRCMD_Stop);
    } else if (byAct == 2) { //Del
        if(isIrcmd)
            ZbZclCmd::GetInstance()->SetIR(device, IrCommand::IRCMD_Delete, atoi(jsonVal["irid"].asCString()));
    } else if ((byAct == 3) || (byAct == 4)) { //Enable || Disable
        if(isIrcmd) {
            Json::Value jsonRetVal;
            jsonRetVal["act"] = std::to_string(byAct);
            if(ircmd.get() != NULL) {
                jsonRetVal["state"] = std::to_string(0);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
//                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);

                if (byAct == 3) ircmd.Modify()->Endpoint = 1;
                else if (byAct == 4) ircmd.Modify()->Endpoint = 0;
                ZbDriver::s_pZbModel->Add(ircmd);
                ZbDriver::s_pZbModel->UpdateChanges();
            } else {
                jsonRetVal["state"] = std::to_string(3);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
//                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);
            }
        }
    } else if (byAct == 5) { //Set
        if(isIrcmd) {
            if((ircmd.get() != NULL) && ((ircmd->Endpoint.GetValue() == 1))) { //(ircmd->Endpoint.GetValue() == 1): Enabled command
                ZbZclCmd::GetInstance()->SetIR(device, IrCommand::IRCMD_Active, atoi(jsonVal["irid"].asCString()));
            } else {
                Json::Value jsonRetVal;
                jsonRetVal["act"] = std::to_string(byAct);
                jsonRetVal["state"] = std::to_string(3);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
//                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);
            }
        }
    }
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSetValueToRGB (
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    std::string temp = jsonVal["level"].asString();


    DeviceProperties vDP;
    DeviceProperty dpRed, dpGreen, dpBlue, dpState, dpTime;

    if(jsonVal["state"].asString() == std::string("off")) {
        dpState = device.Modify()->Action[DI_State];
        dpState.DP_AttributeData = 0;
        vDP.push_back(dpState);
        ZbZclGlobalCmd::GetInstance()->WriteAttributeRequest(device, vDP);
        return;
    }

    if ((jsonVal["state"].asString() == std::string("on")) && (jsonVal["level"].asString() == std::string("-1"))) {
        dpRed   = device.Modify()->Action[DI_RGB_Red];
        dpRed.DP_AttributeData      = dpRed.DP_PreValue;
        dpGreen = device.Modify()->Action[DI_RGB_Green];
        dpGreen.DP_AttributeData    = dpGreen.DP_PreValue;
        dpBlue  = device.Modify()->Action[DI_RGB_Blue];
        dpBlue.DP_AttributeData     = dpBlue.DP_PreValue;
        dpState = device.Modify()->Action[DI_State];
        dpState.DP_AttributeData = 1;

    } else {
        size_t pos1stCol = temp.find(":");
        if (pos1stCol == std::string::npos) { return; }
        int_t red = atoi(temp.substr(0, pos1stCol).c_str());

        temp = temp.substr(pos1stCol +1 , temp.size() - pos1stCol);
        size_t pos2ndCol = temp.find(":");
        if (pos2ndCol == std::string::npos) { return; }
        int_t green = atoi(temp.substr(0, pos2ndCol).c_str());

        temp = temp.substr(pos2ndCol +1, temp.size() - pos2ndCol);
        if (temp.size() == 0) { return; }
        int_t blue = atoi(temp.c_str());

        dpRed   = device.Modify()->Action[DI_RGB_Red];
        dpRed.DP_AttributeData      = red;
        dpGreen = device.Modify()->Action[DI_RGB_Green];
        dpGreen.DP_AttributeData    = green;
        dpBlue  = device.Modify()->Action[DI_RGB_Blue];
        dpBlue.DP_AttributeData     = blue;
        dpState = device.Modify()->Action[DI_State];
        if(jsonVal["state"].asString() == std::string("on")) {
            dpState.DP_AttributeData = 1;
        } else if (jsonVal["state"].asString() == std::string("off")){
            dpState.DP_AttributeData = 0;
        }
    }

    vDP.push_back(dpRed);
    vDP.push_back(dpGreen);
    vDP.push_back(dpBlue);
    vDP.push_back(dpState);

    ZbZclGlobalCmd::GetInstance()->WriteAttributeRequest(device, vDP);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSetValueToDaikin (
    Device_t    device,
    Json::Value jsonVal
) {
    DeviceProperties vDP;
    if(jsonVal.isMember("state")) {
        if(jsonVal["state"] == std::string("off")) {
            device.Modify()->IsRequested(DI_State) = TRUE;
            DeviceProperty temp;
            temp = device.Modify()->Action[DI_State];
            temp.DP_AttributeData = 0;
            ZbZclGlobalCmd::GetInstance()->WriteAttributeRequest(device, temp);
            return;
        }
    }
    Json::Value::Members keys = jsonVal.getMemberNames();
    for(Json::Value::Members::const_iterator it = keys.begin(); it != keys.end(); it++) {
        for(Action_t::const_iterator it2 = device.Modify()->Action.begin(); it2 != device.Modify()->Action.end(); it2++) {
            if(it2->second.DP_DIStringName == (std::string) (*it)) {
                DeviceProperty temp;
                temp.DP_ClusterID = it2->second.DP_ClusterID;
                temp.DP_AttributeID = it2->second.DP_ClusterID;
                if(it2->second.DP_DIStringName == std::string("state")) {
                    if(jsonVal[(std::string) (*it)] == std::string("on")) {
                        temp.DP_AttributeData = 1;
                    }
                } else {
                    temp.DP_AttributeData = atoi((jsonVal[(std::string) (*it)]).asCString());
                }
                if(temp.DP_AttributeData >= 0) {
                    device.Modify()->IsRequested(it2->first) = TRUE;
                    temp.DP_DIName = it2->second.DP_DIName;
                    temp.DP_DIStringName = it2->second.DP_DIStringName;
                    vDP.push_back(temp);
                }
                temp = {};
                break;
            }
        }
    }
    ZbZclGlobalCmd::GetInstance()->WriteAttributeRequest(device, vDP);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardGetRequestToDevice(
    Device_t device
) {
    ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, DI_State);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardGetRequestsToDevice(
    Device_t device
) {
    Vector<DeviceInfo> vDI;
    for(Action_t::const_iterator it = device.Modify()->Action.begin(); it != device.Modify()->Action.end(); it++) {
        if((it->first != DI_Model) && (it->first != DI_Model)) {
            vDI.push_back(it->first);
        }
    }
    ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, vDI);
}


#endif /* CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_ */
