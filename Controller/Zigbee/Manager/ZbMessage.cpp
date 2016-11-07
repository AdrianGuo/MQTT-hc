/*
 * ZbMessage.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbMessage.hpp>
#include <ZbSerialAPI.hpp>
#include <ZbPacket.hpp>
#include <debug.hpp>
#include <zcl.hpp>
#include <zcl_lumi.hpp>
#include <ZbMessage.hpp>

ZbMessage::ZbMessage(
    void_p pJsonMessage,
    Command         zbCommand
) : ZbPacket() {
    m_bySeqMsg      = ++ZbPacket::m_bySeqOfFrame;
    m_byCmdID       = 0;
    m_wZDOCmd       = 0;
    m_byZCLCmd      = 0;
    m_pJsonMessage  = pJsonMessage;
    m_ZbCommand     = zbCommand;

    m_boIsEncrypted = FALSE;

    Classify(zbCommand);
}

ZbMessage::~ZbMessage(){
}

void_t
ZbMessage::SetCmdID(
    u8_t byCmdID
){
    m_byCmdID = byCmdID;
}

u8_t
ZbMessage::GetCmdID() const{
    return m_byCmdID;
}

void_t
ZbMessage::SetZDOCmd(
    u16_t byZDOCmd
){
    m_wZDOCmd = byZDOCmd;
}

u16_t
ZbMessage::GetZDOCmd() const{
    return m_wZDOCmd;
}

void_t
ZbMessage::SetZCLCmd(
    u8_t byGeneralCmd
){
    m_byZCLCmd = byGeneralCmd;
}

u8_t
ZbMessage::GetZCLCmd() const{
    return m_byZCLCmd;
}

void_t
ZbMessage::SetSeqMsg(
    u8_t bySeqMsg
){
    m_bySeqMsg = bySeqMsg;
}

u8_t
ZbMessage::GetSeqMsg() const{
    return m_bySeqMsg;
}

void_t
ZbMessage::SetZbCommand(
    Command zbCommand
) {
    m_ZbCommand = zbCommand;
}

ZbMessage::Command
ZbMessage::GetZbCommad() const {
    return m_ZbCommand;
}

void_p
ZbMessage::GetJsonMessageObject() const {
    return m_pJsonMessage;
}

void_t
ZbMessage::Classify(
    ZbMessage::Command zbCommand
){
    switch (zbCommand) {
    case ZbMessage::Command::AddDevice:
        m_byCmdID = NWK_JOIN;
        break;

    case ZbMessage::Command::RmvDevice:
        m_byCmdID = ZDO_CMD_REQ;
        break;

    default:
        break;
    }

    ZbPacket::SetSeqOfFrame(m_bySeqMsg);
    ZbPacket::SetCmdID(m_byCmdID);
}
