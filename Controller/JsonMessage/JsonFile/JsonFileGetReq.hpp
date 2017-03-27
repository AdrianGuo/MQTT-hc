/*
 * JsonFileGet.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETREQ_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETREQ_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFileGetReq : public JsonMessageBase {
public:
    typedef struct {
    	String  type;
        String  version;
    } FileInfo_t, *Fileinfo_p;

private:
    FileInfo_t m_File;

public:
    JsonFileGetReq() {}
    virtual ~JsonFileGetReq() {}

    static String GetStrCmd() { return "file=getreq"; }
    virtual void_t Refresh() { m_File = {}; }

    JsonCommand_p CreateJsonCommand(FileInfo_t file);
    FileInfo_t FileInfo() const { return m_File; }
};

typedef JsonFileGetReq  JsonFileGet_t;
typedef JsonFileGetReq* JsonFileGet_p;


/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFileGetReq::CreateJsonCommand(
	FileInfo_t file
) {
    JsonCommand_p pJsonCommand = new JsonCommand("file=getreq");
    Json::Value jsonValue;
    jsonValue["type"] 		= std::string(file.type.c_str());
    jsonValue["version"] 	= std::string(file.version.c_str());
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGETREQ_HPP_ */
