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

#ifndef JSON_DEV_OFF_HPP_
#define JSON_DEV_OFF_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevOff : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t order;
        u8_t net;
    } Device_t, *Device_p;

    JsonDevOff() {}
    virtual ~JsonDevOff() {}

    static String GetStrCmd() { return "dev=off"; }
    virtual void_t Refresh() { }
    JsonCommand_p CreateJsonCommand(Vector<Device_t> vecLstDevice);
};

typedef JsonDevOff  JsonDevOff_t;
typedef JsonDevOff* JsonDevOff_p;

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevOff::CreateJsonCommand(
    Vector<Device_t> vecLstDevice
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev=off");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstDevice.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstDevice[i].devid);
        jsonDevice["ord"] = std::to_string(vecLstDevice[i].order);
        jsonDevice["net"] = std::to_string(vecLstDevice[i].net);
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_DEV_OFF_HPP_ */
