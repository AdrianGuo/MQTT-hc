/*
 * JsonFwInfoRes.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFORES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFORES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFwInfoRes : public JsonMessageBase {
private:
    String m_strVersion;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFwInfoRes() {}
    virtual ~JsonFwInfoRes() {}

    static String GetStrCmd() { return "fw=infores"; }
    virtual void_t Refresh() {}

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    JsonCommand_p CreateJsonCommand(String strVersion);
    String Version() const { return m_strVersion; }
};

typedef JsonFwInfoRes  JsonFwInfoRes_t;
typedef JsonFwInfoRes* JsonFwInfoRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFwInfoRes::ParseJsonCommand(
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
JsonFwInfoRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("version")) { return FALSE; }
    m_strVersion = String(jsonValue["version"].asCString());

    return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFwInfoRes::CreateJsonCommand(
	String strVersion
) {
    JsonCommand_p pJsonCommand = new JsonCommand("fw=infores");
    Json::Value jsonValue;
    jsonValue["version"] = std::string(strVersion.c_str());
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}


#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFWINFORES_HPP_ */
