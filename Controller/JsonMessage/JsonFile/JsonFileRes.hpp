/*
 * JsonFileRes.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILERES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILERES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileRes : public JsonMessageBase {
public:
    typedef struct {
        u8_t       	 type;
        String       version;
        String    	 path;
        String       md5;
    } FileRes_t, *FileRes_p;

private:
    FileRes_t m_File;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFileRes() {}
    virtual ~JsonFileRes() {}

    static String GetStrCmd() { return "file=res"; }
    virtual void_t Refresh() { m_File = {}; }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    FileRes_t FileInfo() const { return m_File; }
};

typedef JsonFileRes  JsonFileRes_t;
typedef JsonFileRes* JsonFileRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFileRes::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFileRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("type")||
    		!jsonValue.isMember("version")||
			!jsonValue.isMember("path")||
			!jsonValue.isMember("md5")) { return FALSE; }
    m_File.type = atoi(jsonValue["type"].asCString());
    m_File.version = String(jsonValue["version"].asCString());
    m_File.path = String(jsonValue["path"].asCString());
    m_File.md5 = String(jsonValue["md5"].asCString());

    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILERES_HPP_ */
