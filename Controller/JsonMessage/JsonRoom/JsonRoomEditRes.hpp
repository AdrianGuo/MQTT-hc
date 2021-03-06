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

#ifndef JSON_ROOM_EDITRES_HPP_
#define JSON_ROOM_EDITRES_HPP_

#include "Typedefs.h"
#include "String.hpp"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomEditRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        i8_t   ret;
        String name;
    } Room_t, *Room_p;

    JsonRoomEditRes() {}
    virtual ~JsonRoomEditRes() {}

    static String GetStrCmd() { return "room=editres"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(Vector<Room_t> vecLstRoom);
};

typedef JsonRoomEditRes  JsonRoomEditRes_t;
typedef JsonRoomEditRes* JsonRoomEditRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomEditRes::CreateJsonCommand(
    Vector<Room_t> vecLstRoom
) {
    JsonCommand_p pJsonCommand = new JsonCommand("room", "editres");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstRoom.size(); i++) {
        Json::Value jsonRoom;
        jsonRoom["roomid"] = std::to_string(vecLstRoom[i].roomid);
        jsonRoom["name"]   = vecLstRoom[i].name.c_str();
        jsonRoom["ret"]    = std::to_string(vecLstRoom[i].ret);
        jsonValue["lst"].append(jsonRoom);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_ROOM_EDITRES_HPP_ */
