/*
 * JsonFileInfo.hpp
 *
 *  Created on: Jan 24, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEACCREQ_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEACCREQ_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileAccReq : public JsonMessageBase {
public:
    JsonFileAccReq() {}
    virtual ~JsonFileAccReq() {}

    static String GetStrCmd() { return "file=accreq"; }
    virtual void_t Refresh() {}

    JsonCommand_p CreateJsonCommand();
};

typedef JsonFileAccReq  JsonFileInfo_t;
typedef JsonFileAccReq* JsonFileInfo_p;

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFileAccReq::CreateJsonCommand() {
    return new JsonCommand("file=accreq", "{}");
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEACCREQ_HPP_ */
