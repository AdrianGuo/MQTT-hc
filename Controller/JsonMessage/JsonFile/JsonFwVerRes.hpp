/*
 * JsonFwInfoRes.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERRES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFwVerRes : public JsonMessageBase {
public:
    typedef struct {
    	String     	 type;
        String       version;
    } VerInfo_t, *VerInfo_p;

private:
    VerInfo_t m_strtVer;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFwVerRes() {}
    virtual ~JsonFwVerRes() {}

    static String GetStrCmd() { return "fw=verres"; }
    virtual void_t Refresh() {}

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(VerInfo_t verInfo);
    VerInfo_t Version() const { return m_strtVer; }
};

typedef JsonFwVerRes  JsonFwInfoRes_t;
typedef JsonFwVerRes* JsonFwInfoRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwVerRes::ParseJsonCommand(
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
JsonFwVerRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("version") ||
    		!jsonValue.isMember("type") ) { return FALSE; }
    m_strtVer.type 		= String(jsonValue["type"].asCString());
    m_strtVer.version 	= String(jsonValue["version"].asCString());

    return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFwVerRes::CreateJsonCommand(
	VerInfo_t verInfo
) {
    JsonCommand_p pJsonCommand = new JsonCommand("fw=verres");
    Json::Value jsonValue;
    jsonValue["type"] 		= std::string(verInfo.type.c_str());
    jsonValue["version"] 	= std::string(verInfo.version.c_str());
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWVERRES_HPP_ */
