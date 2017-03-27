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

#ifndef JSON_FLOOR_LSTROOMRES_HPP_
#define JSON_FLOOR_LSTROOMRES_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorLstRoomRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;
private:
    Vector<Room_t> m_vecLstRoom;
    i32_t m_idwFloorId;
    String m_strFloorName;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFloorLstRoomRes() : m_idwFloorId (-1) {}
    virtual ~JsonFloorLstRoomRes() {}

    static String GetStrCmd() { return "floor=lstroomres"; }
    virtual void_t Refresh() { m_vecLstRoom.clear(); m_idwFloorId = -1; m_strFloorName = ""; }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t FloorId() const { return m_idwFloorId; }
    String FloorName() const { return m_strFloorName; }
    Vector<Room_t> LstRoom() const { return m_vecLstRoom; }
};

typedef JsonFloorLstRoomRes  JsonFloorLstRoomRes_t;
typedef JsonFloorLstRoomRes* JsonFloorLstRoomRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFloorLstRoomRes::ParseJsonCommand(
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
JsonFloorLstRoomRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("floorid") ||
        !jsonValue.isMember("name")    ||
        !jsonValue.isMember("room")) { return FALSE; }

    m_idwFloorId = std::stoi(jsonValue["floorid"].asCString());
    m_strFloorName = jsonValue["name"].asCString();

    Json::Value lstRoom = jsonValue["room"];
    for (u32_t i = 0; i < lstRoom.size(); i++) {
        Json::Value vFloor = lstRoom[i];
        if (!vFloor.isMember("roomid") || !vFloor.isMember("name")) { return FALSE; }
        Room_t room;
        room.roomid = std::stoi(vFloor["roomid"].asCString());
        room.name   = vFloor["name"].asCString();
        m_vecLstRoom.push_back(room);
    }
    return TRUE;
}

#endif /* !JSON_FLOOR_LSTROOMRES_HPP_ */
