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

#ifndef JSON_ROOM_LSTDEVRES_HPP_
#define JSON_ROOM_LSTDEVRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLstDevRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t  order;
        u8_t  netwk;
    } Device_t, *Device_p;
private:
    Vector<Device_t> m_vecLstDevice;
    i32_t  m_idwRoomId;
    String m_strRoomName;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomLstDevRes() : m_idwRoomId (-1) {}
    virtual ~JsonRoomLstDevRes() {}
    static String GetStrCmd() { return "room=lstdevres"; }

    virtual void_t Refresh() { m_vecLstDevice.clear(); m_idwRoomId = -1; m_strRoomName = "";}
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t RoomId() const { return m_idwRoomId; }
    String RoomName() const { return m_strRoomName; }
    Vector<Device_t> LstRoom() const { return m_vecLstDevice; }
};

typedef JsonRoomLstDevRes  JsonRoomLstDevRes_t;
typedef JsonRoomLstDevRes* JsonRoomLstDevRes_p;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomLstDevRes::ParseJsonCommand(
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
JsonRoomLstDevRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("roomid") ||
        !jsonValue.isMember("name")   ||
        !jsonValue.isMember("dev")) { return FALSE; }

    m_idwRoomId = std::stoi(jsonValue["roomid"].asCString());
    m_strRoomName = jsonValue["name"].asCString();

    Json::Value lstDevice = jsonValue["dev"];
    for (u32_t i = 0; i < lstDevice.size(); i++) {
        Json::Value vDevice = lstDevice[i];
        if (!vDevice.isMember("devid") ||
            !vDevice.isMember("ord")   ||
            !vDevice.isMember("net")) { return FALSE; }
        Device_t device;
        device.devid = std::stoi(vDevice["roomid"].asCString());
        device.order = std::stoi(vDevice["ord"].asCString());
        device.netwk = std::stoi(vDevice["net"].asCString());
        m_vecLstDevice.push_back(device);
    }
    return TRUE;
}




#endif /* !JSON_ROOM_LSTDEVRES_HPP_ */
