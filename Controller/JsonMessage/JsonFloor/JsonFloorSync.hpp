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

#ifndef JSON_FLOOR_SYNC_HPP_
#define JSON_FLOOR_SYNC_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorSync : public JsonMessageBase {
public:
    typedef struct {
        i32_t  floorid;
        String name;
    } Floor_t, *Floor_p;
private:
    Vector<Floor_t> m_vecLstFloor;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFloorSync() {}
    virtual ~JsonFloorSync() {}

    static String GetStrCmd() { return "floor=sync"; }
    virtual void_t Refresh() { m_vecLstFloor.clear(); }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
};

typedef JsonFloorSync  JsonFloorSync_t;
typedef JsonFloorSync* JsonFloorSync_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFloorSync::ParseJsonCommand(
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
JsonFloorSync::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("lst")) { return FALSE; }
    Json::Value lstroom = jsonValue["lst"];
    for (u32_t i = 0; i < lstroom.size(); i++) {
        Json::Value vRoom = lstroom[i];
        if (!vRoom.isMember("floorid") || !vRoom.isMember("name")) { return FALSE; }
        Floor_t floor;
        floor.floorid = std::stoi(vRoom["floorid"].asCString());
        floor.name   = vRoom["name"].asCString();
        m_vecLstFloor.push_back(floor);
    }
    return TRUE;
}

#endif /* !JSON_FLOOR_SYNC_HPP_ */
