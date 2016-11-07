#ifndef ZWMESSAGE_HPP_
#define ZWMESSAGE_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "Packet.hpp"
#include "JsonCommand.hpp"

class ZwMessage : public Packet {
public:
    typedef enum {
        SendData = 0,
        AddDevice,
        StopAdd,
        RmvDevice,
        StopRmv,
        GetEnPoints,
        GetCapability
    } Command;
private:
    static u8_t sByNextCallbackID;

    u8_t m_byTargetNodeID;
    u8_t m_byTypeMessage; /* REQ: Request, RES: Response */
    u8_t m_byFunctionID;
    u8_t m_byCallbackID;
    u8_t m_byExpectedFunctionID;
    u8_t m_byExpectedCmdClassID;
    u8_t m_byEndpointID;
    ZwMessage::Command m_ZwCommand;

    bool_t m_boCallbackRequired;
    bool_t m_boReplyRequired;
    bool_t m_boIsEncrypted;

public:

    ZwMessage(JsonCommand_p pJsonCommand,
              Command zwCommand = Command::SendData);
    ZwMessage(u8_t byTargetNodeID,
              u8_t byTypeMsg,
              u8_t byFunctionID,
              bool_t boCallbackRequired,
              bool_t boReplyRequired = TRUE,
              u8_t byExpectedFunctionID = 0,
              u8_t byExpectedCmdClassID = 0,
              Command zwCommand = Command::SendData);

    virtual ~ZwMessage();

    void_t SetTargetNodeID(u8_t byTargetNodeID);
    void_t SetTypeMessage(u8_t byTypeMessage);
    void_t SetFunctionID(u8_t byFunctionID);
    void_t SetCallbackID(u8_t byCallbackID);
    void_t SetCallbackRequired(bool_t boCallbackRequired);
    void_t SetExpectedReply(bool_t boExpectedReply);
    void_t SetExpectedFunctionID(u8_t byExpectedFunctionID);
    void_t SetExpectedCmdClassID(u8_t byExpectedCmdClassID);
    void_t SetZwCommand(Command zwCommand);

    u8_t GetTargetNodeID() const;
    u8_t GetTypeMessage() const;
    u8_t GetFunctionID() const;
    u8_t GetCallbackID() const;
    bool_t IsCallbackRequired() const;
    bool_t IsExpectedFunctionID() const;
    u8_t GetExpectedFunctionID() const;
    u8_t GetExpectedCmdClassID() const;
    Command GetZwCommad() const;

    u8_t GetNextCallbackID();
};

typedef ZwMessage  ZwMessage_t;
typedef ZwMessage* ZwMessage_p;

#endif /* !ZWMESSAGE_HPP_ */
