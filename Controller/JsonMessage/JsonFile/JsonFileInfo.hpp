/*
 * JsonFileInfo.hpp
 *
 *  Created on: Jan 24, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFO_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFO_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileInfo : public JsonMessageBase {
private:
    Vector<Device_t> m_vecLstDev;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFileInfo() {}
    virtual ~JsonFileInfo() {}

    static String GetStrCmd() { return "file=inforeq"; }
    virtual void_t Refresh() {}

    JsonCommand_p CreateJsonCommand();
};

typedef JsonFileInfo  JsonFileInfo_t;
typedef JsonFileInfo* JsonFileInfo_p;

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFileInfo::CreateJsonCommand() {
    return new JsonCommand("file=inforeq", "{}");
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFO_HPP_ */
