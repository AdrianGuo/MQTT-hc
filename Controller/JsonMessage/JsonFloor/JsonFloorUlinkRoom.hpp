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

#ifndef JSON_FLOOR_ULINKROOM_HPP_
#define JSON_FLOOR_ULINKROOM_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorUlinkRoom : public JsonMessageBase {
public:
    typedef struct {
        i32_t roomid;
    } Room_t, *Room_p;
private:
    Vector<Room_t> m_vecLstRoom;
    i32_t m_idwFloorId;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFloorUlinkRoom() : m_idwFloorId (-1) {}
    virtual ~JsonFloorUlinkRoom() {}

    static String GetStrCmd() { return "floor=ulinkroom"; }
    virtual void_t Refresh() { m_vecLstRoom.clear(); m_idwFloorId = -1;  }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t FloorId() const { return m_idwFloorId; }
    Vector<Room_t> LstRoom() const { return m_vecLstRoom; }
};

typedef JsonFloorUlinkRoom  JsonFloorUlinkRoom_t;
typedef JsonFloorUlinkRoom* JsonFloorUlinkRoom_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFloorUlinkRoom::ParseJsonCommand(
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
JsonFloorUlinkRoom::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("floorid") || !jsonValue.isMember("room")) { return FALSE; }
    m_idwFloorId = std::stoi(jsonValue["floorid"].asCString());
    Json::Value lstdev = jsonValue["dev"];
    for (u32_t i = 0; i < lstdev.size(); i++) {
        Json::Value vLstDev = lstdev[i];
        if (!vLstDev.isMember("roomid")) { return FALSE; }
        Room_t room;
        room.roomid = std::stoi(vLstDev["roomid"].asCString());
        m_vecLstRoom.push_back(room);
    }
    return TRUE;
}

#endif /* !JSON_FLOOR_ULINKROOM_HPP_ */
