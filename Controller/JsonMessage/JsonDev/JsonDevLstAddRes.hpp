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
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef JSON_DEV_LSTADDRES_HPP_
#define JSON_DEV_LSTADDRES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevLstAddRes : public JsonMessageBase {
public:
    JsonDevLstAddRes() {}
    virtual ~JsonDevLstAddRes() {}

    static String GetStrCmd() { return "dev=lstaddres"; }
    virtual void_t Refresh() { }
};

typedef JsonDevLstAddRes  JsonDevLstAddRes_t;
typedef JsonDevLstAddRes* JsonDevLstAddRes_p;


#endif /* !JSON_DEV_LSTADDRES_HPP_ */
