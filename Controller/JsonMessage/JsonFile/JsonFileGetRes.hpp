/*
 * JsonFileRes.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETRES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileGetRes : public JsonMessageBase {
public:
    typedef struct {
        String       type;
        String       version;
        String    	 path;
        String       md5;
    } FileInfo_t, *FileInfo_p;

private:
    FileInfo_t m_File;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFileGetRes() {}
    virtual ~JsonFileGetRes() {}

    static String GetStrCmd() { return "file=getres"; }
    virtual void_t Refresh() { m_File = {}; }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    FileInfo_t FileInfo() const { return m_File; }
};

typedef JsonFileGetRes  JsonFileRes_t;
typedef JsonFileGetRes* JsonFileRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFileGetRes::ParseJsonCommand(
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
JsonFileGetRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("type")||
    		!jsonValue.isMember("version")||
			!jsonValue.isMember("path")||
			!jsonValue.isMember("md5")) { return FALSE; }
    m_File.type 	= String(jsonValue["type"].asCString());
    m_File.version 	= String(jsonValue["version"].asCString());
    m_File.path 	= String(jsonValue["path"].asCString());
    m_File.md5 		= String(jsonValue["md5"].asCString());

    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETRES_HPP_ */
