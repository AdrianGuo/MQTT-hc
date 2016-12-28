/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-11-04 09:00:00 (Fri, 04 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_DEV_STT_HPP_
#define JSON_DEV_STT_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevStt : public JsonMessageBase {
public:
    typedef struct {
        i32_t       devid;
        u8_t        order;
        u8_t        type;
        u8_t        netwk;
        Json::Value value;
    } Device_t, *Device_p;

private:
    Vector<Device_t> m_vecLstDev;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonDevStt() {}
    virtual ~JsonDevStt() {}

    static String GetStrCmd() { return "dev=stt"; }
    virtual void_t Refresh() { m_vecLstDev.clear(); }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(Vector<Device_t> vecLstDevice);
    Vector<Device_t> LstDev() const { return m_vecLstDev; }
};

typedef JsonDevStt  JsonDevStt_t;
typedef JsonDevStt* JsonDevStt_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevStt::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevStt::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("dev")) { return FALSE; }

    Json::Value lstdev = jsonValue["dev"];

    for (u32_t i = 0; i < lstdev.size(); i++) {
        Json::Value vLstDev = lstdev[i];
        if (!vLstDev.isMember("devid") ||
            !vLstDev.isMember("ord")   ||
            !vLstDev.isMember("net")   ||
            !vLstDev.isMember("type")  ||
            !vLstDev.isMember("val")) { return FALSE; }

        Device_t device;

        device.devid = std::stoi(vLstDev["devid"].asCString());
        device.order = std::stoi(vLstDev["ord"].asCString());
        device.netwk = std::stoi(vLstDev["net"].asCString());
        device.type  = std::stoi(vLstDev["type"].asCString());
        device.value = vLstDev["val"];
        m_vecLstDev.push_back(device);
    }

    return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevStt::CreateJsonCommand(
    Vector<Device_t> vecLstDevice
) {
    Json::Value jsonValue;
    JsonCommand_p pJsonCommand = new JsonCommand("dev=stt");

    for (u32_t i = 0; i < vecLstDevice.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstDevice[i].devid);
        jsonDevice["ord"]   = std::to_string(vecLstDevice[i].order);
        jsonDevice["net"]   = std::to_string(vecLstDevice[i].netwk);
        jsonDevice["type"]  = std::to_string(vecLstDevice[i].type);
        jsonDevice["val"]   = vecLstDevice[i].value;
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_DEV_STT_HPP_ */
