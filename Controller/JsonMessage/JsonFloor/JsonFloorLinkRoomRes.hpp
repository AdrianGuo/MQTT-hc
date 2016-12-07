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

#ifndef JSON_FLOOR_LINKROOMRES_HPP_
#define JSON_FLOOR_LINKROOMRES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorLinkRoomRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t roomid;
        i8_t  ret;
    } Room_t, *Room_p;

    JsonFloorLinkRoomRes() {}
    virtual ~JsonFloorLinkRoomRes() {}

    static String GetStrCmd() { return "floor=linkroomres"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(i32_t idwFloorId, Vector<Room_t> vecLstRoom);
};

typedef JsonFloorLinkRoomRes  JsonFloorLinkRoomRes_t;
typedef JsonFloorLinkRoomRes* JsonFloorLinkRoomRes_p;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorLinkRoomRes::CreateJsonCommand(
    i32_t idwFloorId,
    Vector<Room_t> vecLstRoom
) {
    JsonCommand_p pJsonCommand = new JsonCommand("floor=linkroomres");
    Json::Value jsonValue;

    jsonValue["floorid"] = std::to_string(idwFloorId);

    for (u32_t i = 0; i < vecLstRoom.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["roomid"] = std::to_string(vecLstRoom[i].roomid);
        jsonDevice["ret"]    = std::to_string(vecLstRoom[i].ret);
        jsonValue["room"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_FLOOR_LINKROOMRES_HPP_ */
