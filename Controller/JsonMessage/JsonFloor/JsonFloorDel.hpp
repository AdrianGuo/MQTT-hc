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

#ifndef JSON_FLOOR_DEL_HPP_
#define JSON_FLOOR_DEL_HPP_


#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorDel : public JsonMessageBase {
public:
    typedef struct {
        i32_t  floorid;
    } Floor_t, *Floor_p;
private:
    Vector<Floor_t> m_vecLstFloor;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFloorDel() {}
    virtual ~JsonFloorDel() {}

    static String GetStrCmd() { return "floor=del"; }
    virtual void_t Refresh() { m_vecLstFloor.clear(); }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    Vector<Floor_t> ListFloor() const { return m_vecLstFloor; }
};

typedef JsonFloorDel  JsonFloorDel_t;
typedef JsonFloorDel* JsonFloorDel_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFloorDel::ParseJsonCommand(
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
JsonFloorDel::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("lst")) { return FALSE; }
    Json::Value lstroom = jsonValue["lst"];
    for (u32_t i = 0; i < lstroom.size(); i++) {
        Json::Value vRoom = lstroom[i];
        if (!vRoom.isMember("floorid")) { return FALSE; }
        Floor_t floor;
        floor.floorid = std::stoi(vRoom["floorid"].asCString());
        m_vecLstFloor.push_back(floor);
    }
    return TRUE;
}

#endif /* !JSON_FLOOR_DEL_HPP_ */
