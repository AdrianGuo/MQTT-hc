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

#ifndef JSON_FLOOR_LSTROOM_HPP_
#define JSON_FLOOR_LSTROOM_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorLstRoom : public JsonMessageBase {
public:
    JsonFloorLstRoom() {}
    virtual ~JsonFloorLstRoom() {}

    static String GetStrCmd() { return "floor=lstroom"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(i32_t idwFloorId);
};

typedef JsonFloorLstRoom  JsonFloorLstRoom_t;
typedef JsonFloorLstRoom* JsonFloorLstRoom_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorLstRoom::CreateJsonCommand(
    i32_t idwFloorId
) {
    JsonCommand_p pJsonCommand = new JsonCommand("floor=lstroom");
    Json::Value jsonValue;

    jsonValue["floorid"] = std::to_string(idwFloorId);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_FLOOR_LSTROOM_HPP_ */
