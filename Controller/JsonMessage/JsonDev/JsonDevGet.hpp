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
 * Last Changed:     Date: 2016-10-25 14:00:00 (Tue, 25 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_DEV_GET_HPP_
#define JSON_DEV_GET_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "LogPlus.hpp"

class JsonDevGet : public JsonMessageBase {
public:
    typedef struct {
        i32_t       devid;
        u8_t        order;
        u8_t        netwk;
        u8_t        type;
    } Device_t, *Device_p;

private:
    Vector<Device_t> m_vecLstDev;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonDevGet() {}
    virtual ~JsonDevGet() {}

    static const String GetStrCmd() { return "dev=get"; }
    virtual void_t Refresh() { m_vecLstDev.clear(); }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(Vector<Device_t> vecLstDev);
    Vector<Device_t> LstDev() const { return m_vecLstDev; }
};

typedef JsonDevGet  JsonDevGet_t;
typedef JsonDevGet* JsonDevGet_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevGet::ParseJsonCommand(
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
JsonDevGet::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("dev")) { return FALSE; }
    Json::Value lstdev = jsonValue["dev"];

    for (u32_t i = 0; i < lstdev.size(); i++) {
        Json::Value vLstDev = lstdev[i];
        if (!vLstDev.isMember("devid") ||
            !vLstDev.isMember("ord")   ||
            !vLstDev.isMember("net")   ||
            !vLstDev.isMember("type")) { return FALSE; }
        Device_t device;
        device.devid = std::stoi(vLstDev["devid"].asCString());
        device.order = std::stoi(vLstDev["ord"].asCString());
        device.netwk = std::stoi(vLstDev["net"].asCString());
        device.type  = std::stoi(vLstDev["type"].asCString());
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
JsonDevGet::CreateJsonCommand(
    Vector<Device_t> vecLstDev
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev=get");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstDev.size(); i++) {
        Json::Value jsonZwDevice;
        jsonZwDevice["devid"] = std::to_string(vecLstDev[i].devid);
        jsonZwDevice["ord"] = std::to_string(vecLstDev[i].order);
        jsonZwDevice["net"] = std::to_string(vecLstDev[i].netwk);
        jsonZwDevice["type"] = std::to_string(vecLstDev[i].type);
        jsonValue["dev"].append(jsonZwDevice);
    }

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_DEV_GET_HPP_ */
