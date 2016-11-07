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

#ifndef JSONZWLSTADDRES_HPP_
#define JSONZWLSTADDRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwLstAddRes {
public:
    JsonZwLstAddRes() {}
    virtual ~JsonZwLstAddRes() {}
    static String GetStrCmd() { return "zw=lstAddRes"; }
};

typedef JsonZwLstAddRes  JsonZwLstAddRes_t;
typedef JsonZwLstAddRes* JsonZwLstAddRes_p;


#endif /* JSONZWLSTADDRES_HPP_ */
