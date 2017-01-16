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

#ifndef JSON_DEV_LST_HPP_
#define JSON_DEV_LST_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevLst : public JsonMessageBase {
public:
    JsonDevLst() {}
    virtual ~JsonDevLst() {}

    static String GetStrCmd() { return "dev=lst"; }
    virtual void_t Refresh() { }
};

typedef JsonDevLst  JsonDevLst_t;
typedef JsonDevLst* JsonDevLst_p;


#endif /* !JSON_DEV_LST_HPP_ */
