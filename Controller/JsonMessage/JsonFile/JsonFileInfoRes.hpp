/*
 * JsonFileInfoRes.hpp
 *
 *  Created on: Feb 7, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFORES_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFORES_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileInfoRes : public JsonMessageBase {
public:
    typedef struct {
        String       host;
        String       port;
        String    	 user;
        String       password;
        String       path;
    } Server_t, *Server_p;

private:
    Server_t m_Server;
    bool_t ParseJsonValue(Json::Value& jsonValue);

public:
    JsonFileInfoRes() {}
    virtual ~JsonFileInfoRes() {}

    static String GetStrCmd() { return "file=infores"; }
    virtual void_t Refresh() { m_Server = {}; }

    bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
    Server_t ServerInfo() const { return m_Server; }
};

typedef JsonFileInfoRes  JsonFileInfoRes_t;
typedef JsonFileInfoRes* JsonFileInfoRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonFileInfoRes::ParseJsonCommand(
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
JsonFileInfoRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("host")||
    		!jsonValue.isMember("port")||
			!jsonValue.isMember("user")||
			!jsonValue.isMember("password")||
			!jsonValue.isMember("path")) { return FALSE; }
    m_Server.host = String(jsonValue["host"].asCString());
    m_Server.port = String(jsonValue["host"].asCString());
    m_Server.user = String(jsonValue["user"].asCString());
    m_Server.password = String(jsonValue["password"].asCString());
    m_Server.path = String(jsonValue["path"].asCString());

    return TRUE;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEINFORES_HPP_ */
