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

#ifndef JSON_FLOOR_LST_HPP_
#define JSON_FLOOR_LST_HPP_


#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorLst : public JsonMessageBase {
public:
    JsonFloorLst() {}
    virtual ~JsonFloorLst() {}

    static String GetStrCmd() { return "floor=lst"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand();
};

typedef JsonFloorLst  JsonFloorLst_t;
typedef JsonFloorLst* JsonFloorLst_p;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFloorLst::CreateJsonCommand() {
    Json::Reader reader;
    Json::Value root;
    reader.parse("{}", root, false);
    JsonCommand_p pJsonCommand = new JsonCommand("floor=lst");
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

#endif /* !JSON_FLOOR_LST_HPP_ */
