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
    static u8_t sByNextCallbackId;

    u8_t m_byTargetNodeId;
    u8_t m_byTypeMessage; /* REQ: Request, RES: Response */
    u8_t m_byFunctionId;
    u8_t m_byCallbackId;
    u8_t m_byExpectedFunctionId;
    u8_t m_byExpectedCmdClassId;
    u8_t m_byExprectedEndpointId;

    bool_t m_boCallbackRequired;
    bool_t m_boReplyRequired;
    bool_t m_boIsEncrypted;

    ZwMessage::Command m_ZwCommand;
public:

    ZwMessage(JsonCommand_p pJsonCommand,
              Command zwCommand = Command::SendData);
    ZwMessage(u8_t   byTargetNodeId,
              u8_t   byTypeMsg,
              u8_t   byFunctionId,
              bool_t boCallbackRequired,
              bool_t boReplyRequired = TRUE,
              u8_t   byExpectedFunctionId = 0,
              u8_t   byExpectedCmdClassId = 0,
              Command zwCommand = Command::SendData);

    virtual ~ZwMessage();

    void_t SetTargetNodeId      (u8_t byTargetNodeId);
    void_t SetTypeMessage       (u8_t byTypeMessage);
    void_t SetFunctionId        (u8_t byFunctionId);
    void_t SetCallbackId        (u8_t byCallbackId);
    void_t SetExpectedFunctionId(u8_t byExpectedFunctionId);
    void_t SetExpectedCmdClassId(u8_t byExpectedCmdClassId);
    void_t SetExpectedEndpointId(u8_t byExpectedEndpointId);
    void_t SetCallbackRequired  (bool_t boCallbackRequired);
    void_t SetExpectedReply     (bool_t boExpectedReply);
    void_t SetZwCommand         (Command zwCommand);

    u8_t GetTargetNodeId() const;
    u8_t GetTypeMessage()  const;
    u8_t GetFunctionId()   const;
    u8_t GetCallbackId()   const;

    u8_t GetExpectedFunctionId() const;
    u8_t GetExpectedCmdClassId() const;
    u8_t GetExpectedEndpointId() const;

    bool_t IsCallbackRequired()   const;
    bool_t IsExpectedFunctionId() const;

    Command GetZwCommad() const;
    u8_t GetNextCallbackId();
};

typedef ZwMessage  ZwMessage_t;
typedef ZwMessage* ZwMessage_p;

#endif /* !ZWMESSAGE_HPP_ */