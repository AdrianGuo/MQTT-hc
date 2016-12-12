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

#ifndef JSON_ROOM_LINKDEVRES_HPP_
#define JSON_ROOM_LINKDEVRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLinkDevRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t order;
        u8_t netwk;
        i8_t ret;
    } Device_t, *Device_p;
    JsonRoomLinkDevRes() {}
    virtual ~JsonRoomLinkDevRes() {}

    static String GetStrCmd() { return "room=linkdevres"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(i32_t idwRoomId, Vector<Device_t> vecLstDevice);

};

typedef JsonRoomLinkDevRes  JsonRoomLinkDevRes_t;
typedef JsonRoomLinkDevRes* JsonRoomLinkDevRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomLinkDevRes::CreateJsonCommand(
    i32_t idwRoomId,
    Vector<Device_t> vecLstDevice
) {
    JsonCommand_p pJsonCommand = new JsonCommand("room", "linkdevres");
    Json::Value jsonValue;

    jsonValue["roomid"] = std::to_string(idwRoomId);

    for (u32_t i = 0; i < vecLstDevice.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstDevice[i].devid);
        jsonDevice["ord"]   = std::to_string(vecLstDevice[i].order);
        jsonDevice["net"]   = std::to_string(vecLstDevice[i].netwk);
        jsonDevice["ret"]   = std::to_string(vecLstDevice[i].ret);
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_ROOMLINKDEVRES_HPP_ */