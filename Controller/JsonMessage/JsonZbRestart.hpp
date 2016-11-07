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

#ifndef CONTROLLER_JSONMESSAGE_JSONZBRESTART_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBRESTART_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbRestart {
private:
public:
    JsonZbRestart() {}
    virtual ~JsonZbRestart() {}
    static String GetStrCmd() { return "zb=restart"; }
};

typedef JsonZbRestart  JsonZbRestart_t;
typedef JsonZbRestart* JsonZbRestart_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONZBRESTART_HPP_ */
