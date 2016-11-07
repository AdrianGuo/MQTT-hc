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

#ifndef JSON_ZWLSTDELRES_HPP_
#define JSON_ZWLSTDELRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

class JsonZwLstDelRes {
private:
public:
    JsonZwLstDelRes() {}
    virtual ~JsonZwLstDelRes() {}
    static String GetStrCmd() { return "zw=lstDelres"; }
};

typedef JsonZwLstDelRes  JsonZwLstDelRes_t;
typedef JsonZwLstDelRes* JsonZwLstDelRes_p;

#endif /* !JSON_ZWLSTDELRES_HPP_ */
