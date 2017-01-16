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

#ifndef JSON_KALIVE_RES_HPP_
#define JSON_KALIVE_RES_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonKaliveRes : public JsonMessageBase {
public:
    JsonKaliveRes() {}
    virtual ~JsonKaliveRes() {}

    static String GetStrCmd() { return "kalive=res"; }
    virtual void_t Refresh() { }
};

typedef JsonKaliveRes  JsonKaliveRes_t;
typedef JsonKaliveRes* JsonKaliveRes_p;

#endif /* !JSON_KALIVE_RES_HPP_ */
