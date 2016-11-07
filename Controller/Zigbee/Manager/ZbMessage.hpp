/*
 * ZbMessage.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBMESSAGE_HPP_
#define ZBMESSAGE_HPP_

#include <typedefs.h>
#include <String.hpp>
#include <ZbPacket.hpp>
#include <JsonCommand.hpp>

class ZbMessage : public ZbPacket {
public:
    typedef enum {
        SendData = 0,
        AddDevice,
        StopAdd,
        RmvDevice,
        StopRmv,
        SetDevice,
        GetEnPoints,
        GetCapability
    } Command;

private:
    u8_t    m_bySeqMsg;
    u8_t    m_byCmdID;
    u8_t    m_byZCLCmd;
    u16_t   m_wZDOCmd;
    void_p  m_pJsonMessage;
    Command m_ZbCommand;

    bool_t  m_boIsEncrypted;

public:
    ZbMessage(void_p pJsonMessage,
            Command zbCommand = Command::SendData);

    virtual ~ZbMessage();

    void_t  Classify(ZbMessage::Command);

    void_t  SetCmdID(u8_t);
    u8_t    GetCmdID() const;

    void_t  SetSeqMsg(u8_t);
    u8_t    GetSeqMsg() const;

    void_t  SetZCLCmd(u8_t);
    u8_t    GetZCLCmd() const;

    void_t  SetZDOCmd(u16_t);
    u16_t   GetZDOCmd() const;

    void_t  SetZbCommand(Command);
    Command GetZbCommad() const;

    void_p  GetJsonMessageObject() const;
};

typedef ZbMessage ZbMessage_t;
typedef ZbMessage* ZbMessage_p;

#endif /* ZBMESSAGE_HPP_ */
