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

#ifndef JSON_ROOM_SYNC_HPP_
#define JSON_ROOM_SYNC_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomSync : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;
private:
    Vector<Room_t> m_vecLstRoom;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomSync() {}
    virtual ~JsonRoomSync() {}

    static String GetStrCmd() { return "room=sync"; }
    virtual void_t Refresh() { m_vecLstRoom.clear(); }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
};

typedef JsonRoomSync  JsonRoomSync_t;
typedef JsonRoomSync* JsonRoomSync_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomSync::ParseJsonCommand(
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
JsonRoomSync::ParseJsonValue(
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

#endif /* JSON_ROOM_SYNC_HPP_ */
