#ifndef CONTROLLER_JSONMESSAGE_JSONZBLSTDEL_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBLSTDEL_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "ZbModelDb.hpp"

class JsonZbLstDel {
public:
    JsonZbLstDel() {}
    virtual ~JsonZbLstDel() {}
    JsonCommand_p CreateJsonCommand(Devices_t devices);
    static String GetStrCmd() { return "dev=lstdel"; }
};

typedef JsonZbLstDel  JsonZbLstDel_t;
typedef JsonZbLstDel* JsonZbLstDel_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZbLstDel::CreateJsonCommand(
    Devices_t devices
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev", "lstdel");
    Json::Value jsonValue;
    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        if((*it)->RealType > 0) {
            Json::Value dev;
            dev["devid"] = std::to_string((*it)->DeviceID.GetValue());
            dev["ord"] = std::to_string((*it)->Endpoint.GetValue());

            jsonValue["dev"].append(dev);
        }
    }

    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONZBLSTDEL_HPP_ */
