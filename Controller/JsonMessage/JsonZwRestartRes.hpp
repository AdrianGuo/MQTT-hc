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

#ifndef JSON_ZWRESTARTRES_HPP_
#define JSON_ZWRESTARTRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwRestartRes {
public:
    JsonZwRestartRes() {}
    virtual ~JsonZwRestartRes() {}
    static String GetStrCmd() { return "rz=restartres"; }
};

typedef JsonZwRestartRes  JsonZwRestartRes_t;
typedef JsonZwRestartRes* JsonZwRestartRes_p;

#endif /* !JSON_ZWRESTARTRES_HPP_ */
