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

#ifndef JSON_FLOOR_SYNCROOMRES_HPP_
#define JSON_FLOOR_SYNCROOMRES_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorSyncRoomRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;

    JsonFloorSyncRoomRes() {}
    virtual ~JsonFloorSyncRoomRes() {}

    static String GetStrCmd() { return "floor=syncroomres"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(i32_t idwFloorId,
            String strFloorName, Vector<Room_t> vecLstRoom);
};

typedef JsonFloorSyncRoomRes  JsonFloorSyncRoomRes_t;
typedef JsonFloorSyncRoomRes* JsonFloorSyncRoomRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorSyncRoomRes::CreateJsonCommand(
    i32_t idwFloorId,
    String strFloorName,
    Vector<Room_t> vecLstRoom
) {
    JsonCommand_p pJsonCommand = new JsonCommand("floor=syncroomres");
    Json::Value jsonValue;

    jsonValue["floorid"] = std::to_string(idwFloorId);
    jsonValue["name"] = strFloorName.c_str();

    for (u32_t i = 0; i < vecLstRoom.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devid"] = std::to_string(vecLstRoom[i].roomid);
        jsonDevice["name"]  = vecLstRoom[i].name.c_str();
        jsonValue["room"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}




#endif /* CONTROLLER_JSONMESSAGE_JSONFLOOR_JSONFLOORSYNCROOMRES_HPP_ */
