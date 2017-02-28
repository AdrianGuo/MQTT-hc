/*
 * JsonFilePut.hpp
 *
 *  Created on: Feb 27, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEPUT_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEPUT_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonFilePut : public JsonMessageBase {
public:
    typedef struct {
    	String  type;
        String  version;
        String	path;
    } FileInfo_t, *FileGet_p;

private:
    FileInfo_t m_File;

public:
    JsonFilePut() {}
    virtual ~JsonFilePut() {}

    static String GetStrCmd() { return "file=put"; }
    virtual void_t Refresh() { m_File = {}; }

    JsonCommand_p CreateJsonCommand(FileInfo_t file);
    FileInfo_t FileInfo() const { return m_File; }
};

typedef JsonFilePut  JsonFilePut_t;
typedef JsonFilePut* JsonFilePut_p;


/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFilePut::CreateJsonCommand(
	FileInfo_t file
) {
    JsonCommand_p pJsonCommand = new JsonCommand("file=put");
    Json::Value jsonValue;
    jsonValue["type"] 		= std::string(file.type.c_str());
    jsonValue["version"] 	= std::string(file.version.c_str());
    jsonValue["path"] 		= std::string(file.path.c_str());
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEPUT_HPP_ */
