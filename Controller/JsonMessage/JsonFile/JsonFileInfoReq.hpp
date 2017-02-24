/*
 * JsonFileInfo.hpp
 *
 *  Created on: Jan 24, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFOREQ_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFOREQ_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileInfoReq : public JsonMessageBase {
public:
    JsonFileInfoReq() {}
    virtual ~JsonFileInfoReq() {}

    static String GetStrCmd() { return "file=inforeq"; }
    virtual void_t Refresh() {}

    JsonCommand_p CreateJsonCommand();
};

typedef JsonFileInfoReq  JsonFileInfo_t;
typedef JsonFileInfoReq* JsonFileInfo_p;

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFileInfoReq::CreateJsonCommand() {
    return new JsonCommand("file=inforeq", "{}");
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFOREQ_HPP_ */
