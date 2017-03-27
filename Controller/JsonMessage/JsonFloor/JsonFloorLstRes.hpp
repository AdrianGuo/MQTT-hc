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

#ifndef JSON_FLOOR_LSTRES_HPP_
#define JSON_FLOOR_LSTRES_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorLstRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  floorid;
        String name;
    } Floor_t, *Floor_p;
private:
    Vector<Floor_t> m_vecLstFloor;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonFloorLstRes() {}
    virtual ~JsonFloorLstRes() {}

    static String GetStrCmd() { return "floor=lstres"; }
    virtual void_t Refresh() { m_vecLstFloor.clear(); }
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    virtual Vector<Floor_t> ListFloor() const { return m_vecLstFloor; }
};

typedef JsonFloorLstRes  JsonFloorLstRes_t;
typedef JsonFloorLstRes* JsonFloorLstRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFloorLstRes::ParseJsonCommand(
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
JsonFloorLstRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("lst")) { return FALSE; }
    Json::Value lstfloor = jsonValue["lst"];
    for (u32_t i = 0; i < lstfloor.size(); i++) {
        Json::Value vFloor = lstfloor[i];
        if (!vFloor.isMember("floorid") || !vFloor.isMember("name")) { return FALSE; }
        Floor_t floor;
        floor.floorid = std::stoi(vFloor["floorid"].asCString());
        floor.name    = vFloor["name"].asCString();
        m_vecLstFloor.push_back(floor);
    }
    return TRUE;
}

#endif /* !JSON_FLOOR_LSTRES_HPP_ */
