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

#ifndef JSON_KALIVE_REQ_HPP_
#define JSON_KALIVE_REQ_HPP_


#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonKaliveReq {
public:
    JsonKaliveReq() {}
    virtual ~JsonKaliveReq() {}

    static String GetStrCmd() { return "kalive=req"; }
    JsonCommand_p CreateJsonCommand();
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonKaliveReq::CreateJsonCommand() {
    Json::Reader reader;
    Json::Value root;
    reader.parse("{}", root, false);
    JsonCommand_p pJsonCommand = new JsonCommand("kalive", "req");
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

typedef JsonKaliveReq  JsonKaliveReq_t;
typedef JsonKaliveReq* JsonKaliveReq_p;

#endif /* !JSON_KALIVE_REQ_HPP_ */
