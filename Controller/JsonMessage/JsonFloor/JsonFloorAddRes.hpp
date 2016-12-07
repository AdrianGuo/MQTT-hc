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

#ifndef JSON_FLOOR_ADDRES_HPP_
#define JSON_FLOOR_ADDRES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorAddRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t floorid;
        i8_t  ret;
        String name;
    } Floor_t, *Floor_p;

    JsonFloorAddRes() {}
    virtual ~JsonFloorAddRes() {}

    static String GetStrCmd() { return "floor=addres"; }
    virtual void_t Refresh() { }
    JsonCommand_p CreateJsonCommand(Vector<Floor_t> vecLstFloor);
};

typedef JsonFloorAddRes  JsonFloorAddRes_t;
typedef JsonFloorAddRes* JsonFloorAddRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorAddRes::CreateJsonCommand(
    Vector<Floor_t> vecLstFloor
) {
    JsonCommand_p pJsonCommand = new JsonCommand("floor=addres");
    Json::Value jsonValue;

    for (u32_t i = 0; i < vecLstFloor.size(); i++) {
        Json::Value jsonRoom;
        jsonRoom["floorid"] = std::to_string(vecLstFloor[i].floorid);
        jsonRoom["name"]    = vecLstFloor[i].name.c_str();
        jsonRoom["ret"]     = std::to_string(vecLstFloor[i].ret);
        jsonValue["lst"].append(jsonRoom);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_FLOOR_ADDRES_HPP_ */
