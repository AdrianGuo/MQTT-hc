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

#ifndef JSON_ZWREP_HPP_
#define JSON_ZWREP_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwRep {
public:
    JsonZwRep() {}
    virtual ~JsonZwRep() {}

    JsonCommand_p CreateJsonCommand();
    static String GetStrCmd() { return "zw=rep"; }

};

typedef JsonZwRep  JsonZwRep_t;
typedef JsonZwRep* JsonZwRep_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonZwRep::CreateJsonCommand() {
    return NULL;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONZWREP_HPP_ */
