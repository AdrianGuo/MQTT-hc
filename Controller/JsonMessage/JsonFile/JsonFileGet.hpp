/*
 * JsonFileGet.hpp
 *
 *  Created on: Feb 22, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGET_HPP_
#define CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGET_HPP_


class JsonFileGet : public JsonMessageBase {
public:
    typedef struct {
        u8_t  	type;
        String  version;
    } FileGet_t, *FileGet_p;
private:
    FileGet_t m_File;
public:
    JsonFileGet() {}
    virtual ~JsonFileGet() {}

    static String GetStrCmd() { return "file=get"; }
    virtual void_t Refresh() { m_File = {}; }

    JsonCommand_p CreateJsonCommand(FileGet_t file);
    FileGet_t File() const { return m_File; }
};

typedef JsonFileGet  JsonFileGet_t;
typedef JsonFileGet* JsonFileGet_p;


/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonFileGet::CreateJsonCommand(
	FileGet_t file
) {
    JsonCommand_p pJsonCommand = new JsonCommand("file=get");
    Json::Value jsonValue;
    jsonValue["type"] = std::to_string(file.type);
    jsonValue["version"] = std::string(file.version.c_str());
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* CONTROLLER_JSONMESSAGE_JSONFILE_JSONFILEGET_HPP_ */
