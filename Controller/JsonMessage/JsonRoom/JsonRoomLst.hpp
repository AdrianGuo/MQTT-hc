#ifndef JSON_ROOM_LST_HPP_
#define JSON_ROOM_LST_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRoomLst : public JsonMessageBase {
public:
    JsonRoomLst() {}
    virtual ~JsonRoomLst() {}

    static String GetStrCmd() { return "room=lst"; }
    virtual JsonCommand_p CreateJsonCommand();
    virtual void_t Refresh() { }
};

typedef JsonRoomLst  JsonRoomLst_t;
typedef JsonRoomLst* JsonRoomLst_p;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p
JsonRoomLst::CreateJsonCommand() {
    Json::Reader reader;
    Json::Value root;
    reader.parse("{}", root, false);
    JsonCommand_p pJsonCommand = new JsonCommand("room", "lst");
    pJsonCommand->SetJsonObject(root);

    return pJsonCommand;
}

#endif /* !JSON_ROOMLST_HPP_ */
