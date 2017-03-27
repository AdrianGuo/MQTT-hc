#ifndef JSON_ROOM_LSTDEV_HPP_
#define JSON_ROOM_LSTDEV_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLstDev : public JsonMessageBase {
public:
    JsonRoomLstDev() {}
    virtual ~JsonRoomLstDev() {}

    static String GetStrCmd() { return "room=lstdev"; }
    virtual void_t Refresh() { }
    virtual JsonCommand_p CreateJsonCommand(i32_t idwRoomId);
};

typedef JsonRoomLstDev  JsonRoomLstDev_t;
typedef JsonRoomLstDev* JsonRoomLstDev_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomLstDev::CreateJsonCommand(
    i32_t idwRoomId
) {
    JsonCommand_p pJsonCommand = new JsonCommand("room", "lstdev");
    Json::Value jsonValue;

    jsonValue["roomid"] = std::to_string(idwRoomId);
    pJsonCommand->SetJsonObject(jsonValue);

    return pJsonCommand;
}

#endif /* !JSON_ROOMLSTDEV_HPP_ */
