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

#ifndef JSON_FLOOR_SYNCRES_HPP_
#define JSON_FLOOR_SYNCRES_HPP_


#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorSyncRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  floorid;
        String name;
    } Floor_t, *Floor_p;

    JsonFloorSyncRes() {}
    virtual ~JsonFloorSyncRes() {}

    static String GetStrCmd() { return "floor=syncres"; }
    virtual void_t Refresh() { }
    JsonCommand_p CreateJsonCommand(Vector<Floor_t> vecLstRoom);
};

typedef JsonFloorSyncRes  JsonFloorSyncRes_t;
typedef JsonFloorSyncRes* JsonFloorSyncRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorSyncRes::CreateJsonCommand(
    Vector<Floor_t> vecLstRoom
) {
    JsonCommand_p pJsonCommand = new JsonCommand("floor=syncres");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstRoom.size(); i++) {
        Json::Value jsonRoom;
        jsonRoom["floorid"] = std::to_string(vecLstRoom[i].floorid);
        jsonRoom["name"]   = vecLstRoom[i].name.c_str();
        jsonValue["lst"].append(jsonRoom);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFLOOR_JSONFLOORSYNCRES_HPP_ */
