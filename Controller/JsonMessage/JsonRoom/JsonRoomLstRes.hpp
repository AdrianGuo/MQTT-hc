#ifndef JSON_ROOM_LSTRES_HPP_
#define JSON_ROOM_LSTRES_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLstRes : public JsonMessageBase {
public:
    typedef struct {
        i32_t  roomid;
        String name;
    } Room_t, *Room_p;
private:
    Vector<Room_t> m_vecLstRoom;
    bool_t ParseJsonValue(Json::Value& jsonValue);
public:
    JsonRoomLstRes() {}
    virtual ~JsonRoomLstRes() {}

    static String GetStrCmd() { return "room=lstres"; }
    virtual void_t Refresh() {}
    virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);
};

typedef JsonRoomLstRes  JsonRoomLstRes_t;
typedef JsonRoomLstRes* JsonRoomLstRes_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomLstRes::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
    return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRoomLstRes::ParseJsonValue(
    Json::Value& jsonValue
) {
    if (!jsonValue.isMember("lst")) { return FALSE; }
    Json::Value lstroom = jsonValue["lst"];
    for (u32_t i = 0; i < lstroom.size(); i++) {
        Json::Value vRoom = lstroom[i];
        if (!vRoom.isMember("roomid") || !vRoom.isMember("name")) { return FALSE; }
        Room_t room;
        room.roomid = std::stoi(vRoom["roomid"].asCString());
        room.name   = vRoom["name"].asCString();
        m_vecLstRoom.push_back(room);
    }
    return TRUE;
}

#endif /* !JSON_ROOMLSTRES_HPP_ */
