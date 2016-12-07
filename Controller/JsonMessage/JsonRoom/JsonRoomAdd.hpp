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

#ifndef JSON_ROOM_ADD_HPP_
#define JSON_ROOM_ADD_HPP_

#include <iostream>
#include <string>
#include "typedefs.h"
#include "String.hpp"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomAdd : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;
private:
    Vector<Room_t> m_vecLstRoom;
	bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomAdd() {}
    virtual ~JsonRoomAdd() {}

    static String GetStrCmd() { return "room=add"; }
    virtual void_t Refresh() { m_vecLstRoom.clear(); }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    Vector<Room_t> ListRoom() const { return m_vecLstRoom; }
};

typedef JsonRoomAdd  JsonRoomAdd_t;
typedef JsonRoomAdd* JsonRoomAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomAdd::ParseJsonCommand(
JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomAdd::ParseJsonValue(
	Json::Value& jsonValue
) {
    if (!jsonValue.isMember("lst")) { return FALSE; }
    Json::Value lstroom = jsonValue["lst"];
    for (u32_t i = 0; i < lstroom.size(); i++) {
        Json::Value vRoom = lstroom[i];
        if (!vRoom.isMember("roomid") || !vRoom.isMember("name")) { return FALSE; }
        Room_t room;
        room.roomid = std::stoi(vRoom["roomid"].asCString());
        room.name   = vRoom["name"].asCString();
        m_vecLstRoom.push_back(room);
    }
    return TRUE;
}

#endif /* !JSON_ROOM_ADD_HPP_ */
