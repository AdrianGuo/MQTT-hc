#include "debug.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwSerialAPI.hpp"
#include "JsonZwAdd.hpp"
#include "JsonZwDel.hpp"
#include "JsonZwReset.hpp"
#include "JsonZwRestart.hpp"
#include "ZwMessage.hpp"

u8_t ZwMessage::sByNextCallbackID = 0;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::ZwMessage(
    JsonCommand_p pJsonCommand,
    Command zwCommand
) {
    if (pJsonCommand->GetFullCommand() == JsonZwAdd::GetStrCmd()) {
        m_byTargetNodeID = 0xFF;
        m_byTypeMessage = REQUEST;
        m_byFunctionID = FUNC_ID_ZW_ADD_NODE_TO_NETWORK;
        m_byExpectedFunctionID = m_byFunctionID;
        m_byExpectedCmdClassID = 0;
        m_boCallbackRequired = TRUE;
        m_boReplyRequired = TRUE;
        m_boIsEncrypted = FALSE;
        Packet::ResetPacket(2);
        if (pJsonCommand->GetJsonOjbect()["act"] == "0") {
            Packet::Push(ADD_NODE_ANY);
            Packet::Push(GetNextCallbackID());
        } else if (pJsonCommand->GetJsonOjbect()["act"] == "1") {
            Packet::Push(ADD_NODE_ANY);
            Packet::Push(GetNextCallbackID());
        } else if (pJsonCommand->GetJsonOjbect()["act"] == "2") {
            Packet::Push(ADD_NODE_STOP);
            Packet::Push(GetNextCallbackID());
        } else {
            Packet::Push(0xFF);
            Packet::Push(0);
        }
    } else if (pJsonCommand->GetFullCommand() == JsonZwDel::GetStrCmd()) {
        m_byTargetNodeID = 0xFF;
        m_byTypeMessage = REQUEST;
        m_byFunctionID = FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK;
        m_byExpectedFunctionID = m_byFunctionID;
        m_byExpectedCmdClassID = 0;
        m_boCallbackRequired = TRUE;
        m_boReplyRequired = TRUE;
        m_boIsEncrypted = FALSE;
        Packet::ResetPacket(2);
        if (pJsonCommand->GetJsonOjbect()["act"] == "0") {
            Packet::Push(REMOVE_NODE_ANY);
            Packet::Push(GetNextCallbackID());
        } else if (pJsonCommand->GetJsonOjbect()["act"] == "1") {
            Packet::Push(REMOVE_NODE_ANY);
            Packet::Push(GetNextCallbackID());
        } else if (pJsonCommand->GetJsonOjbect()["act"] == "2") {
            Packet::Push(REMOVE_NODE_STOP);
            Packet::Push(0);
        } else {
            Packet::Push(0xFF);
            Packet::Push(0);
        }
    } else if (pJsonCommand->GetFullCommand() == JsonZwReset::GetStrCmd()) {
        m_byTargetNodeID = 0xFF;
        m_byTypeMessage = REQUEST;
        m_byFunctionID = FUNC_ID_ZW_SET_DEFAULT;
        m_byExpectedFunctionID = m_byFunctionID;
        m_byExpectedCmdClassID = 0;
        m_boCallbackRequired = TRUE;
        m_boReplyRequired = TRUE;
        m_boIsEncrypted = FALSE;
        Packet::ResetPacket(1);
        Packet::Push(GetNextCallbackID());
    } else if (pJsonCommand->GetFullCommand() == JsonZwRestart::GetStrCmd()) {
        m_byTargetNodeID = 0xFF;
        m_byTypeMessage = REQUEST;
        m_byFunctionID = 0;
        m_byExpectedFunctionID = m_byFunctionID;
        m_byExpectedCmdClassID = 0;
        m_boCallbackRequired = FALSE;
        m_boReplyRequired = FALSE;
        m_boIsEncrypted = FALSE;
    }
    m_ZwCommand = zwCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::ZwMessage(
    u8_t byTargetNodeID,
    u8_t byTypeMsg,
    u8_t byFunctionID,
    bool_t boCallbackRequired,
    bool_t boReplyRequired,         // TRUE
    u8_t byExpectedFunctionID,      // 0
    u8_t byExpectedCmdClassID,      // 0
    Command zwCommand
) {
    m_byTargetNodeID = byTargetNodeID;
    m_byTypeMessage = byTypeMsg;
    m_byFunctionID = byFunctionID;
    m_byCallbackID = 0;
    m_byEndpointID = 0;
    m_boCallbackRequired = boCallbackRequired;
    m_boReplyRequired = boReplyRequired;

    if (boReplyRequired) {
        m_byExpectedFunctionID = (byExpectedFunctionID != 0) ? byExpectedFunctionID : byFunctionID;
    }

    m_byExpectedCmdClassID = byExpectedCmdClassID;
    m_boIsEncrypted = FALSE;

    m_ZwCommand = zwCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::~ZwMessage() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetTargetNodeID(
    u8_t byTargetNodeID
) {
    m_byTargetNodeID = byTargetNodeID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t ZwMessage::SetTypeMessage(
    u8_t byTypeMessage
) {
    m_byTypeMessage = byTypeMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t ZwMessage::SetFunctionID(
    u8_t byFunctionID
) {
    m_byFunctionID = byFunctionID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetCallbackID(
    u8_t byCallbackID
) {
    m_byCallbackID = byCallbackID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetCallbackRequired(
    bool_t boCallbackRequired
) {
    m_boCallbackRequired = boCallbackRequired;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedReply(
    bool_t boExpectedReply
) {
    m_boReplyRequired = boExpectedReply;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedFunctionID(
    u8_t byExpectedFunctionID
) {
    m_byExpectedFunctionID = byExpectedFunctionID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedCmdClassID(
    u8_t byExpectedCmdClassID
) {
    m_byExpectedCmdClassID = byExpectedCmdClassID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetZwCommand(
    Command zwCommand
) {
    m_ZwCommand = zwCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetTargetNodeID() const {
    return m_byTargetNodeID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetTypeMessage() const {
    return m_byTypeMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetFunctionID() const {
    return m_byFunctionID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetCallbackID() const {
    return m_byCallbackID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwMessage::IsCallbackRequired() const {
    return m_boCallbackRequired;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwMessage::IsExpectedFunctionID() const {
    return m_boReplyRequired;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetExpectedFunctionID() const {
    return m_byExpectedFunctionID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetExpectedCmdClassID() const {
    return m_byExpectedCmdClassID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::Command
ZwMessage::GetZwCommad() const {
    return m_ZwCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetNextCallbackID() {
    sByNextCallbackID++;
    if (sByNextCallbackID > 127) {
        sByNextCallbackID = 1;
    }
    m_byCallbackID = sByNextCallbackID;

    return m_byCallbackID;
}
