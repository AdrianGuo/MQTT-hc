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

#ifndef CONTROLLER_JSONMESSAGE_JSONFLOORULINKDEVRES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFLOORULINKDEVRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorUlinkDevRes {
public:
    JsonFloorUlinkDevRes() {}
    virtual ~JsonFloorUlinkDevRes() {}
    static String GetStrCmd() const { return "floor=ulinkdevres"; }
};

typedef JsonFloorUlinkDevRes  JsonFloorUlinkDevRes_t;
typedef JsonFloorUlinkDevRes* JsonFloorUlinkDevRes_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONFLOORULINKDEVRES_HPP_ */
