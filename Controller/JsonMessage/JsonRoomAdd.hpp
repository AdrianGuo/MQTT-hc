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

#ifndef JSON_ROOMADD_HPP_
#define JSON_ROOMADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomAdd {
public:
    JsonRoomAdd() {}
    virtual ~JsonRoomAdd() {}
    static String GetStrCmd() const { return "room=add"; }
};

typedef JsonRoomAdd  JsonRoomAdd_t;
typedef JsonRoomAdd* JsonRoomAdd_p;

#endif /* !JSON_ROOMADD_HPP_ */
