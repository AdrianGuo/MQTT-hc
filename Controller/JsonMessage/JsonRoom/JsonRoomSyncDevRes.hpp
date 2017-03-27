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

#ifndef JSON_ROOM_SYNCDEVRES_HPP_
#define JSON_ROOM_SYNCDEVRES_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomSyncDevRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t  order;
        u8_t  netwk;
    } Device_t, *Device_p;

    JsonRoomSyncDevRes() {}
    virtual ~JsonRoomSyncDevRes() {}

    static String GetStrCmd() { return "room=syncdevres"; }
    virtual void_t Refresh() {}
    virtual JsonCommand_p CreateJsonCommand(i32_t idwRoomId,
            String strRoomName, Vector<Device_t> vecLstDevice);
};

typedef JsonRoomSyncDevRes  JsonRoomSyncDevRes_t;
typedef JsonRoomSyncDevRes* JsonRoomSyncDevRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomSyncDevRes::CreateJsonCommand(
    i32_t idwRoomId,
    String strRoomName,
    Vector<Device_t> vecLstDevice
) {
    JsonCommand_p pJsonCommand = new JsonCommand("room", "syncdevres");
    Json::Value jsonValue;

    jsonValue["roomid"] = std::to_string(idwRoomId);
    jsonValue["name"] = strRoomName.c_str();

    for (u32_t i = 0; i < vecLstDevice.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstDevice[i].devid);
        jsonDevice["ord"]   = std::to_string(vecLstDevice[i].order);
        jsonDevice["netwk"]   = std::to_string(vecLstDevice[i].netwk);
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_ROOM_SYNCDEVRES_HPP_ */
