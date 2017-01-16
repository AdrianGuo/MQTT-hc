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

#ifndef JSON_ROOM_SYNCDEV_HPP_
#define JSON_ROOM_SYNCDEV_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomSyncDev : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t  order;
        u8_t  netwk;
    } Device_t, *Device_p;
private:
    Vector<Device_t> m_vecLstDev;
    i32_t m_idwRoomId;
    String m_strRoomName;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomSyncDev() : m_idwRoomId (0) {}
    virtual ~JsonRoomSyncDev() {}

    static String GetStrCmd() { return "room=syncdev"; }
    virtual void_t Refresh() { m_vecLstDev.clear(); m_idwRoomId = -1; m_strRoomName = ""; }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t RoomId() const { return m_idwRoomId; }
    String RoomName() const { return m_strRoomName; }
    Vector<Device_t> LstDev() const { return m_vecLstDev; }
};

typedef JsonRoomSyncDev  JsonRoomSyncDev_t;
typedef JsonRoomSyncDev* JsonRoomSyncDev_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomSyncDev::ParseJsonCommand(
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
JsonRoomSyncDev::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("roomid") ||
            !jsonValue.isMember("name")   ||
            !jsonValue.isMember("dev")) { return FALSE; }

    m_idwRoomId = std::stoi(jsonValue["roomid"].asCString());
    m_strRoomName = jsonValue["name"].asCString();
    Json::Value lstdev = jsonValue["dev"];

    for (u32_t i = 0; i < lstdev.size(); i++) {
        Json::Value vLstDev = lstdev[i];
        if (!vLstDev.isMember("devid") ||
            !vLstDev.isMember("ord")   ||
            !vLstDev.isMember("net")) { return FALSE; }
        Device_t device;
        device.devid = std::stoi(vLstDev["devid"].asCString());
        device.order = std::stoi(vLstDev["ord"].asCString());
        device.netwk   = std::stoi(vLstDev["net"].asCString());
        m_vecLstDev.push_back(device);
    }
    return TRUE;
}

#endif /* !JSON_ROOM_SYNCDEV_HPP_ */
