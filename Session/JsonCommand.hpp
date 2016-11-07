#ifndef JSONCOMMAND_HPP_
#define JSONCOMMAND_HPP_

#include "json-forwards.h"
#include "json.h"
#include "String.hpp"

class JsonCommand {
private:
    String m_strCmdClass;
    String m_strCommand;

    u32_t m_dwSrcFlag;
    u32_t m_dwDesFlag;

    bool_t m_boJsonAvailable;
    Json::Value m_jsonValue;

public:
    typedef enum {
        NetWork     = 0x0000,
        Database    = 0x0001,
        Zwave       = 0x0002,
        Zigbee      = 0x0004
    } Flag;

    const static u32_t SrcDefault = Flag::NetWork;
    const static u32_t DesDefault = Flag::Database | Flag::Zwave | Flag::Zigbee;

    JsonCommand();
    JsonCommand(String strCmdClass, String strCommand, String strJson = "",
                u32_t dwSrcFlag = SrcDefault, u32_t dwDesFlag = DesDefault);
    virtual ~JsonCommand();

    void_t SetCmdClass(String strCmdClass);
    String GetCmdClass() const;

    void_t SetCommand(String strCmdClass);
    String GetCommand() const;

    String GetFullCommand() const;

    void_t SetJsonObject(String strJson);
    void_t SetJsonObject(Json::Value& jsonValue);

    bool_t IsJsonAvailable();
    Json::Value& GetJsonOjbect();
    String GetJsonValue();

    void_t SetSrcFlag(u32_t dwSrcFlag);
    u32_t  GetSrcFlag() const;

    void_t SetDesFlag(u32_t dwDesFlag);
    u32_t  GetDesFlag() const;

};

typedef JsonCommand  JsonCommand_t;
typedef JsonCommand* JsonCommand_p;

#endif /* !JSONCOMMAND_HPP_ */
