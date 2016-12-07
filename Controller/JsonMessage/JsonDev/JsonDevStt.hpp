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
        i32_t devid;
        u8_t order;
        u8_t type;
        u8_t netwk;
        Json::Value value;
    } Device_t, *Device_p;

    JsonDevStt() {}
    virtual ~JsonDevStt() {}

    static String GetStrCmd() { return "dev=stt"; }
    virtual void_t Refresh() { }
    JsonCommand_p CreateJsonCommand(Vector<Device_t> vecLstDevice);
};

typedef JsonDevStt  JsonDevStt_t;
typedef JsonDevStt* JsonDevStt_p;

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
    JsonCommand_p pJsonCommand = new JsonCommand("dev=stt");
    Json::Value jsonValue;

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
