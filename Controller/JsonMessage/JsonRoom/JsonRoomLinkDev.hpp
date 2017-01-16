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

#ifndef JSON_ROOMLINKDEV_HPP_
#define JSON_ROOMLINKDEV_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLinkDev : public JsonMessageBase {
public:
    typedef struct {
        i32_t devid;
        u8_t  order;
        u8_t  netwk;
    } Device_t, *Device_p;
private:
    Vector<Device_t> m_vecLstDev;
    i32_t m_idwRoomId;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomLinkDev() : m_idwRoomId (0) {}
    virtual ~JsonRoomLinkDev() {}

    static String GetStrCmd() { return "room=linkdev"; }
    virtual void_t Refresh() { m_vecLstDev.clear(); m_idwRoomId = -1; }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    i32_t RoomId() const { return m_idwRoomId; }
    Vector<Device_t> LstDev() const { return m_vecLstDev; }

};

typedef JsonRoomLinkDev  JsonRoomLinkDev_t;
typedef JsonRoomLinkDev* JsonRoomLinkDev_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomLinkDev::ParseJsonCommand(
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
JsonRoomLinkDev::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("roomid") || !jsonValue.isMember("dev")) { return FALSE; }
    m_idwRoomId = std::stoi(jsonValue["roomid"].asCString());
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

#endif /* !JSON_ROOMLINKDEV_HPP_ */
