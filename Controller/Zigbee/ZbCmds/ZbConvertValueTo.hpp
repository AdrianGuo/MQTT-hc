/*
 * ZbConvertValueTo.hpp
 *
 *  Created on: Nov 10, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_
#define CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_

#include <typedefs.h>
#include <debug.hpp>
#include <json.h>
#include <ZbModelDb.hpp>
#include <ZbMessage.hpp>
#include <ZbZclCmd.hpp>
#include <ZbZclGlobalCmd.hpp>
#include <DeviceInfo.hpp>

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
    ZbMessage_p pZbMessage,
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level")) { return; }
    u8_t val = atoi(jsonVal["level"].asCString());
    ZbZclCmd::GetInstance()->SetDevice(pZbMessage, device, val);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToDimmerCurtain (
    ZbMessage_p pZbMessage,
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    u8_t val = atoi(jsonVal["level"].asCString());

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
    device.Modify()->Action[DeviceInfo::DI_State].DP_AttributeData = val;
    ZbZclCmd::GetInstance()->SetDevice(pZbMessage, device, val);
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */

void_t
ForwardSetValueToFan (
    ZbMessage_p pZbMessage,
    Device_t    device,
    Json::Value jsonVal
) {
    if(!jsonVal.isMember("level") || !jsonVal.isMember("state")) { return; }
    u8_t rcvVal = atoi(jsonVal["level"].asCString());

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

    ZbZclCmd::GetInstance()->SetDevice(pZbMessage, device, rcvVal/25 * 63);
}



/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSetValueToIr (
    ZbMessage_p pZbMessage,
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

    if(byAct == 0) { //Learn
        ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Learn);
    } else if (byAct == 1) { //Stop
        ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Stop);
    } else if (byAct == 2) { //Del
        if(isIrcmd)
            ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Delete, atoi(jsonVal["irid"].asCString()));
    } else if ((byAct == 3) || (byAct == 4)) { //Enable || Disable
        if(isIrcmd) {
            Json::Value jsonRetVal;
            if(ircmd.Modify() != NULL) {
                jsonRetVal["state"] = std::to_string(0);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);

                if (byAct == 3) ircmd.Modify()->Endpoint = 1;
                else if (byAct == 4) ircmd.Modify()->Endpoint = 0;
                ZbDriver::s_pZbModel->Add(ircmd);
                ZbDriver::s_pZbModel->UpdateChanges();
            } else {
                jsonRetVal["state"] = std::to_string(3);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);
            }
        }
    } else if (byAct == 5) { //Set
        if(isIrcmd) {
            if((ircmd.Modify() != NULL) && ((ircmd->Endpoint.GetValue() == 1))) { //(ircmd->Endpoint.GetValue() == 1): Enabled command
                ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Active, atoi(jsonVal["irid"].asCString()));
            } else {
                Json::Value jsonRetVal;
                jsonRetVal["state"] = std::to_string(3);
                jsonRetVal["irid"] = jsonVal["irid"].asString();
                ZbSocketCmd::GetInstance()->SendZbStt(device, jsonRetVal);
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

    int_t state;
    if(jsonVal["state"].asString() == std::string("on")) {
        state = 1;
    } else if (jsonVal["state"].asString() == std::string("off")){
        state = 0;
    }

    DeviceProperties vDP;
    DeviceProperty dpRed, dpGreen, dpBlue, dpState, dpTime;
    dpRed.DP_AttributeData = red;
    dpRed.DP_ClusterID = ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL;
    dpRed.DP_DIName = DeviceInfo::DI_RGB_Red;

    dpGreen.DP_AttributeData = green;
    dpGreen.DP_ClusterID = ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL;
    dpGreen.DP_DIName = DeviceInfo::DI_RGB_Green;

    dpBlue.DP_AttributeData = blue;
    dpBlue.DP_ClusterID = ZCL_CLUSTER_ID_LIGHTING_COLOR_CONTROL;
    dpBlue.DP_DIName = DeviceInfo::DI_RGB_Blue;

    dpState.DP_AttributeData = state;
    dpState.DP_ClusterID = ZCL_CLUSTER_ID_GEN_ON_OFF;
    dpState.DP_DIName = DeviceInfo::DI_State;

    vDP.push_back(dpRed);
    vDP.push_back(dpGreen);
    vDP.push_back(dpBlue);
    vDP.push_back(dpState);

//    DEBUG2("Red: %d, Green: %d, Blue: %d, State: %d, Time: %d", red, green, blue, state);
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
    Json::Value::Members keys = jsonVal.getMemberNames();
    for(Json::Value::Members::const_iterator it = keys.begin(); it != keys.end(); it++) {
        for(Action_t::const_iterator_t it2 = device.Modify()->Action.begin(); it2 != device.Modify()->Action.end(); it2++) {
            if(it2->second.DP_DIStringName == (std::string) (*it)) {
                DeviceProperty temp;
                temp.DP_ClusterID = it2->second.DP_ClusterID;
                temp.DP_AttributeID = it2->second.DP_ClusterID;
                if(it2->second.DP_DIStringName == std::string("state")) {
                    if(jsonVal[(std::string) (*it)] == std::string("on")) {
                        temp.DP_AttributeData = 1;
                    } else {
                        temp.DP_AttributeData = 0;
                        vDP.clear();
                        temp.DP_DIName = it2->second.DP_DIName;
                        temp.DP_DIStringName = it2->second.DP_DIStringName;
                        vDP.push_back(temp);
                        temp = {};
                        goto END;
                    }
                } else {
                    temp.DP_AttributeData = atoi((jsonVal[(std::string) (*it)]).asCString());
                }
                if(temp.DP_AttributeData >= 0) {
                    temp.DP_DIName = it2->second.DP_DIName;
                    temp.DP_DIStringName = it2->second.DP_DIStringName;
                    vDP.push_back(temp);
                }
                temp = {};
                break;
            }
        }
    }
END:
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
    Device_t    device
) {
    ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, DeviceInfo::DI_State);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardGetRequestsToDevice(
    Device_t    device
) {
    Vector<DeviceInfo> vDI;
    for(Action_t::const_iterator_t it = device.Modify()->Action.begin(); it != device.Modify()->Action.end(); it++) {
        if((it->first != DeviceInfo::DI_Model) && (it->first != DeviceInfo::DI_Model)) {
            vDI.push_back(it->first);
        }
    }
    ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, vDI);
}


#endif /* CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_ */
