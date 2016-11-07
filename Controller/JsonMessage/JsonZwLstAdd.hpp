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
 * Last Changed:     Date: 2016-10-31 19:40:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_ZWLSTADD_HPP_
#define JSON_ZWLSTADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

class JsonZwLstAdd {
public:
    typedef struct {
        u8_t NodeId;
        u8_t NodeType;
        u8_t Order;
    } LstAdd_t, *LstAdd_p;

    JsonZwLstAdd() {}
    virtual ~JsonZwLstAdd() {}
    JsonCommand_p CreateJsonCommand(Vector<LstAdd_t> lstAdd);
    static String GetStrCmd() { return "zw=lstAdd"; }
private:
    Vector<LstAdd_t> m_lstAdd;
};

typedef JsonZwLstAdd  JsonZwLstAdd_t;
typedef JsonZwLstAdd* JsonZwLstAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwLstAdd::CreateJsonCommand(
    Vector<LstAdd_t> lstAdd
) {
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "lstAdd");
    Json::Value jsonValue;

    for (u32_t i = 0; i < lstAdd.size(); i++) {
        Json::Value jsonDevice;
        jsonDevice["devID"] = std::to_string(lstAdd[i].NodeId);
        jsonDevice["type"]  = std::to_string(lstAdd[i].NodeType);
        jsonDevice["ord"]   = std::to_string(lstAdd[i].Order);
        jsonValue["dev"].append(jsonDevice);
    }
    pJsonCommand->SetJsonObject(jsonValue);
    return pJsonCommand;
}

#endif /* !JSON_ZWLSTADD_HPP_ */
