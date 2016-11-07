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

#ifndef CONTROLLER_JSONMESSAGE_JSONZbBLSTADD_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZbBLSTADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonZbLstAdd {
public:
    JsonZbLstAdd() {}
    virtual ~JsonZbLstAdd() {}
    JsonCommand_p CreateJsonCommand(Devices_t devices);
    static String GetStrCmd() { return "zb=lstAdd"; }
};

typedef JsonZbLstAdd  JsonZbLstAdd_t;
typedef JsonZbLstAdd* JsonZbLstAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbLstAdd::CreateJsonCommand(
    Devices_t devices
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zb", "lstAdd");
    Json::Value jsonValue;
    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Json::Value dev;
        dev["devid"] = std::to_string((*it)->DeviceID.GetValue());
        dev["type"] = std::to_string((*it)->RealType);
        dev["ord"] = std::to_string((*it)->Endpoint.GetValue());
        jsonValue["dev"].append(dev);
    }

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONZbBLSTADD_HPP_ */
