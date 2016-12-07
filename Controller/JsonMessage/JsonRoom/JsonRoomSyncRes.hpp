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

#ifndef JSON_ROOM_SYNCRES_HPP_
#define JSON_ROOM_SYNCRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomSyncRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;

    JsonRoomSyncRes() {}
    virtual ~JsonRoomSyncRes() {}

    static String GetStrCmd() { return "room=syncres"; }
    virtual void_t Refresh() {}
    virtual JsonCommand_p CreateJsonCommand(Vector<Room_t> vecLstRoom);
};

typedef JsonRoomSyncRes  JsonRoomSyncRes_t;
typedef JsonRoomSyncRes* JsonRoomSyncRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomSyncRes::CreateJsonCommand(
    Vector<Room_t> vecLstRoom
) {
    JsonCommand_p pJsonCommand = new JsonCommand("room", "syncres");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstRoom.size(); i++) {
        Json::Value jsonRoom;
        jsonRoom["roomid"] = std::to_string(vecLstRoom[i].roomid);
        jsonRoom["name"]   = vecLstRoom[i].name.c_str();
        jsonValue["lst"].append(jsonRoom);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_ROOM_SYNCRES_HPP_ */
