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

#ifndef JSON_ZWRESET_HPP_
#define JSON_ZWRESET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZwReset {
public:
    JsonZwReset() {}
    virtual ~JsonZwReset() {}
    static String GetStrCmd() { return "zw=reset"; }
};

typedef JsonZwReset  JsonZwReset_t;
typedef JsonZwReset* JsonZwReset_p;

#endif /* !JSON_ZWRESET_HPP_ */
