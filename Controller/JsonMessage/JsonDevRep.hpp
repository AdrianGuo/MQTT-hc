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

#ifndef JSON_DEV_REP_HPP_
#define JSON_DEV_REP_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevRep {
public:
    JsonDevRep() {}
    virtual ~JsonDevRep() {}

    JsonCommand_p CreateJsonCommand(int_t byNodeID, int_t byValue);
    static String GetStrCmd() { return "dev=rep"; }
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonDevRep::CreateJsonCommand(
    int_t byNodeID,
    int_t byValue
) {
    JsonCommand_p pJsonCommand = new JsonCommand("dev", "rep");

    Json::Value jsonValue;
    Json::Value jsonDevice;
    jsonDevice["devID"] = std::to_string(byNodeID);
    jsonDevice["val"] = std::to_string(byValue);
    jsonValue["dev"].append(jsonDevice);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

typedef JsonDevRep  JsonDevRep_t;
typedef JsonDevRep* JsonDevRep_p;

#endif /* !JSON_DEV_REP_HPP_ */
