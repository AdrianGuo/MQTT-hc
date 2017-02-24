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
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef JSON_DEV_LSTADD_HPP_
#define JSON_DEV_LSTADD_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "IO.hpp"
#include "JsonCommand.hpp"

class JsonDevLstAdd : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t  netwk;
        u8_t  type;
        u8_t  order;
        String mac;
    } Device_t, *Device_p;
private:
    Vector<Device_t> m_vecLstDev;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonDevLstAdd() {}
    virtual ~JsonDevLstAdd() {}

    static String GetStrCmd() { return "dev=lstadd"; }
    virtual void_t Refresh() { m_vecLstDev.clear(); }

    JsonCommand_p CreateJsonCommand(Vector<Device_t> vecLstAdd);
    bool_t ParseJsonCommand(JsonCommand_p pJsonComand);
    Vector<Device_t> LstDev() const { return m_vecLstDev; }
};

typedef JsonDevLstAdd  JsonDevLstAdd_t;
typedef JsonDevLstAdd* JsonDevLstAdd_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonDevLstAdd::ParseJsonCommand(
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
JsonDevLstAdd::ParseJsonValue(
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
            !vLstDev.isMember("mac")) { return FALSE; }
        Device_t device;
        device.devid = std::stoi(vLstDev["devid"].asCString());
        device.order = std::stoi(vLstDev["ord"].asCString());
        device.netwk = std::stoi(vLstDev["net"].asCString());
        device.type  = std::stoi(vLstDev["type"].asCString());
        device.mac   = vLstDev["mac"].asCString();
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
JsonDevLstAdd::CreateJsonCommand(
    Vector<Device_t> vecLstAdd
) {
	Notify(DevSig);
    JsonCommand_p pJsonCommand = new JsonCommand("dev=lstadd");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstAdd.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstAdd[i].devid);
        jsonDevice["ord"]   = std::to_string(vecLstAdd[i].order);
        jsonDevice["net"]   = std::to_string(vecLstAdd[i].netwk);
        jsonDevice["type"]  = std::to_string(vecLstAdd[i].type);
        jsonDevice["mac"]   = vecLstAdd[i].mac.c_str();
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_DEV_LSTADD_HPP_ */
