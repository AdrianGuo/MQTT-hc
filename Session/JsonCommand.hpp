#ifndef JSONCOMMAND_HPP_
#define JSONCOMMAND_HPP_

#include "json-forwards.h"
#include "json.h"
#include "String.hpp"

class JsonCommand {
private:
    String m_strFullCommand;
    u32_t m_dwSrcFlag;
    u32_t m_dwDesFlag;

    bool_t m_boJsonAvailable;
    Json::Value m_jsonValue;

    u32_t m_dwClientId;

public:
    typedef enum {
        Server      = 0x0001,
        Local       = 0x0002,
        NetWork     = 0x0004,
        Coord       = 0x0008,
        Zwave       = 0x0020,
        Zigbee      = 0x0040,
        Client      = 0x0080
    } Flag;

    const static u32_t SrcDefault = Flag::Server;
    const static u32_t DesDefault = Flag::Coord;

    JsonCommand(String strFullCommand, String strJson = "",
                u32_t dwSrcFlag = SrcDefault, u32_t dwDesFlag = DesDefault);
    virtual ~JsonCommand();

    void_t SetFullCommand(String strFullCommand);
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

    void_t SetClientId(u32_t dwDesFlag);
    u32_t  GetClientId() const;

};

typedef JsonCommand  JsonCommand_t;
typedef JsonCommand* JsonCommand_p;

#endif /* !JSONCOMMAND_HPP_ */
