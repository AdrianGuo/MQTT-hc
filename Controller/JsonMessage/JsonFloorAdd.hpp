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

#ifndef JSON_FLOOR_ADD_HPP_
#define JSON_FLOOR_ADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFloorAdd {
public:
    JsonFloorAdd() {}
    virtual ~JsonFloorAdd() {}

    static String GetStrCmd() const { return "floor=add"; }
};

typedef JsonFloorAdd  JsonFloorAdd_t;
typedef JsonFloorAdd* JsonFloorAdd_p;

#endif /* !JSON_FLOOR_ADD_HPP_ */
