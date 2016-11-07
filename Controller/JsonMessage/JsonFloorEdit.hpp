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

#ifndef CONTROLLER_JSONMESSAGE_JSONFLOOREDIT_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFLOOREDIT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorEdit {
public:
    JsonFloorEdit() {}
    virtual ~JsonFloorEdit() {}
    static String GetStrCmd() const { return "floor=edit"; }
};

typedef JsonFloorEdit  JsonFloorEdit_t;
typedef JsonFloorEdit* JsonFloorEdit_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONFLOOREDIT_HPP_ */
