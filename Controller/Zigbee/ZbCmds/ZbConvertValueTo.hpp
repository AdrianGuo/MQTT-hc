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
ForwardSetValueToDimmer (
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

    if((val == 0 ) && (jsonVal["state"].asString() == std::string("on"))) {
        val = 100;
    } else if (jsonVal["state"].asString() == std::string("off")) {
        val = 0;
    } else if (jsonVal["state"].asString() == std::string("inc")) {
        val = device->State;

        if (val <= 165)
            val /= 3;
        else if (val > 165)
            val = 55 + (val - 165) / 2;
        else
            val = 0;

        if(val < 100)  val++;
    } else if (jsonVal["state"].asString() == std::string("dec")) {
        val = device->State;

        if (val <= 165)
            val /= 3;
        else if (val > 165)
            val = 55 + (val - 165) / 2;
        else
            val = 0;

        if(val > 0)  val--;
    }
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
    u8_t val = atoi(jsonVal["level"].asCString());

    if(val > 0 && val < 101) val = (val -1)/25 + 1;
    else val = 0;

    if((val == 0 ) && (jsonVal["state"].asString() == std::string("on"))) {
        val = 100;
    } else if (jsonVal["state"].asString() == std::string("off")) {
        val = 0;
    } else if (jsonVal["state"].asString() == std::string("inc")) {
        val = device->State;
        val /= 63;
        if(val < 4)  val--;
    } else if (jsonVal["state"].asString() == std::string("dec")) {
        val = device->State;
        val /= 63;
        if(val > 0)  val--;
    }
    ZbZclCmd::GetInstance()->SetDevice(pZbMessage, device, val);
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ForwardSetValueToRGB (
    ZbMessage_p pZbMessage,
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
        vDI.push_back(it->first);
    }
    ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, vDI);
}


#endif /* CONTROLLER_ZIGBEE_ZBCMDS_ZBCONVERTVALUETO_HPP_ */
