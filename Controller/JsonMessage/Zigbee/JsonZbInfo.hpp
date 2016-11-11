/*
 * JsonZbInfo.hpp
 *
 *  Created on: Nov 8, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONZBINFO_HPP_
#define CONTROLLER_JSONMESSAGE_JSONZBINFO_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonZbInfo {
public:
    JsonZbInfo() {}
    virtual ~JsonZbInfo() {}
    static String GetStrCmd() { return "zb=info"; }
};

typedef JsonZbInfo  JsonZbInfo_t;
typedef JsonZbInfo* JsonZbInfo_p;


#endif /* CONTROLLER_JSONMESSAGE_JSONZBINFO_HPP_ */
