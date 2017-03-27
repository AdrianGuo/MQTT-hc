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

#ifndef JSON_DEV_LSTRES_HPP_
#define JSON_DEV_LSTRES_HPP_

#include "Typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonDevLstRes : public JsonMessageBase {
public:
    JsonDevLstRes() {}
    virtual ~JsonDevLstRes() {}

    static String GetStrCmd() { return "dev=lstres"; }
    virtual void_t Refresh() { }
};

typedef JsonDevLstRes  JsonDevLstRes_t;
typedef JsonDevLstRes* JsonDevLstRes_p;


#endif /* !JSON_DEV_LSTRES_HPP_ */
