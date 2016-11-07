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

#ifndef JSON_ZWLSTDEL_HPP_
#define JSON_ZWLSTDEL_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

class JsonZwLstDel {
private:
public:
    JsonZwLstDel() {}
    virtual ~JsonZwLstDel() {}

    JsonCommand_p CreateJsonCommand(Vector<u8_t> vecNodeID);
    static String GetStrCmd() { return "zw=lstDel"; }
};

typedef JsonZwLstDel  JsonZwLstDel_t;
typedef JsonZwLstDel* JsonZwLstDel_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwLstDel::CreateJsonCommand(
    Vector<u8_t> vecNodeId
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "lstDel");

    Json::Value jsonValue;
    Json::Value jsonDevice;

    for (u32_t i = 0; i < vecNodeId.size(); i++) {
        jsonDevice["devID"] = std::to_string(vecNodeId[i]);
        jsonValue["dev"].append(jsonDevice);
    }

    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_ZWLSTDEL_HPP_ */
