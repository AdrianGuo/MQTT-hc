#include "debug.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwSerialAPI.hpp"


#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevRestart.hpp"

#include "ZwMessage.hpp"

u8_t ZwMessage::sByNextCallbackId = 0;


/**
 * @func   CreateMessageSendData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::CreateMessageSendData(JsonCommand_p pJsonCommand) {

}

/**
 * @func   CreateMessageAddDevice
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::CreateMessageAddDevice(
    JsonCommand_p pJsonCommand
) {
    m_byTargetNodeId = 0xFF;
    m_byTypeMessage = REQUEST;
    m_byFunctionId = FUNC_ID_ZW_ADD_NODE_TO_NETWORK;
    m_byExpectedFunctionId = m_byFunctionId;
    m_byExpectedCmdClassId = 0;
    m_boCallbackRequired = TRUE;
    m_boReplyRequired = TRUE;
    m_boIsEncrypted = FALSE;
    JsonDevAdd jsonZwAdd;
    jsonZwAdd.ParseJsonCommand(pJsonCommand);
    Packet::ResetPacket(2);
    if (jsonZwAdd.Act() == 0) {
        Packet::Push(ADD_NODE_ANY);
        Packet::Push(GetNextCallbackId());
    } else if (jsonZwAdd.Act() == 1) {
        Packet::Push(ADD_NODE_STOP);
        Packet::Push(GetNextCallbackId());
    } else {
        Packet::Push(0xFF);
        Packet::Push(0);
    }
}

/**
 * @func   CreateMessageRmvDevice
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::CreateMessageDelDevice(
    JsonCommand_p pJsonCommand
) {
    m_byTargetNodeId = 0xFF;
    m_byTypeMessage = REQUEST;
    m_byFunctionId = FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK;
    m_byExpectedFunctionId = m_byFunctionId;
    m_byExpectedCmdClassId = 0;
    m_boCallbackRequired = TRUE;
    m_boReplyRequired = TRUE;
    m_boIsEncrypted = FALSE;
    Packet::ResetPacket(2);
    JsonDevDel jsonZwDel;
    jsonZwDel.ParseJsonCommand(pJsonCommand);
    if (jsonZwDel.Act() == 0) {
        Packet::Push(REMOVE_NODE_ANY);
        Packet::Push(GetNextCallbackId());
    } else if (jsonZwDel.Act() == 1) {
        Packet::Push(REMOVE_NODE_STOP);
        Packet::Push(0);
    } else {
        Packet::Push(0xFF);
        Packet::Push(0);
    }
}

/**
 * @func   CreateMessageRstDevice
 * @brief  Create ZwMessage for Json command dev=reset
 * @param  None
 * @retval None
 */
void_t
ZwMessage::CreateMessageRstDevice(
    JsonCommand_p pJsonCommand
) {
    m_byTargetNodeId = 0xFF;
    m_byTypeMessage = REQUEST;
    m_byFunctionId = FUNC_ID_ZW_SET_DEFAULT;
    m_byExpectedFunctionId = m_byFunctionId;
    m_byExpectedCmdClassId = 0;
    m_boCallbackRequired = TRUE;
    m_boReplyRequired = TRUE;
    m_boIsEncrypted = FALSE;
    Packet::ResetPacket(1);
    Packet::Push(GetNextCallbackId());
}

/**
 * @func   CreateMessageRsaDevice
 * @brief  Create ZwMessage for Json command dev=restart
 * @param  None
 * @retval None
 */
void_t
ZwMessage::CreateMessageRsaDevice(
    JsonCommand_p pJsonCommand
) {
    m_byTargetNodeId = 0xFF;
    m_byTypeMessage = REQUEST;
    m_byFunctionId = 0;
    m_byExpectedFunctionId = m_byFunctionId;
    m_byExpectedCmdClassId = 0;
    m_boCallbackRequired = FALSE;
    m_boReplyRequired = FALSE;
    m_boIsEncrypted = FALSE;
}

/**
 * @func   ZwMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::ZwMessage(
    JsonCommand_p pJsonCommand,
    Command zwCommand
) {
    if (pJsonCommand->GetFullCommand() == JsonDevAdd::GetStrCmd()) {
        CreateMessageAddDevice(pJsonCommand);
    } else if (pJsonCommand->GetFullCommand() == JsonDevDel::GetStrCmd()) {
        CreateMessageDelDevice(pJsonCommand);
    } else if (pJsonCommand->GetFullCommand() == JsonDevReset::GetStrCmd()) {
        CreateMessageRstDevice(pJsonCommand);
    } else if (pJsonCommand->GetFullCommand() == JsonDevRestart::GetStrCmd()) {
        CreateMessageRsaDevice(pJsonCommand);
    }
    m_ZwCommand = zwCommand;
}

/**
 * @func   ZwMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::ZwMessage(
    u8_t byTargetNodeId,
    u8_t byTypeMsg,
    u8_t byFunctionId,
    bool_t boCallbackRequired,
    bool_t boReplyRequired,         // TRUE
    u8_t byExpectedFunctionId,      // 0
    u8_t byExpectedCmdClassId,      // 0
    Command zwCommand
) : m_byTargetNodeId (byTargetNodeId),
    m_byTypeMessage (byTypeMsg),
    m_byFunctionId (byFunctionId),
    m_byCallbackId (0),
    m_byExpectedFunctionId (0),
    m_byExpectedEndpointId (0),
    m_byExpectedCmdClassId (byExpectedCmdClassId),
    m_boCallbackRequired (boCallbackRequired),
    m_boReplyRequired (boReplyRequired),
    m_boIsEncrypted (FALSE),
    m_byCountResend (0),
    m_ZwCommand (zwCommand) {
    if (boReplyRequired) {
        m_byExpectedFunctionId =
        (byExpectedFunctionId != 0) ? byExpectedFunctionId : byFunctionId;
    }
}

/**
 * @func   ~ZwMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::~ZwMessage() {
}

/**
 * @func   SetTargetNodeId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetTargetNodeId(
    u8_t byTargetNodeId
) {
    m_byTargetNodeId = byTargetNodeId;
}

/**
 * @func   SetTypeMessage
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
 * @func   SetFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetFunctionId(
    u8_t byFunctionId
) {
    m_byFunctionId = byFunctionId;
}

/**
 * @func   SetCallbackId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetCallbackId(
    u8_t byCallbackId
) {
    m_byCallbackId = byCallbackId;
}

/**
 * @func   SetCallbackRequired
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
 * @func   SetExpectedReply
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
 * @func   SetExpectedFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedFunctionId(
    u8_t byExpectedFunctionId
) {
    m_byExpectedFunctionId = byExpectedFunctionId;
}

/**
 * @func   SetExpectedCmdClassId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedCmdClassId(
    u8_t byExpectedCmdClassId
) {
    m_byExpectedCmdClassId = byExpectedCmdClassId;
}

/**
 * @func   SetExpectedEndpointId
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::SetExpectedEndpointId(
    u8_t byExpectedEndpointId
) {
    m_byExpectedEndpointId = byExpectedEndpointId;
}

/**
 * @func   SetZwCommand
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
 * @func   GetTargetNodeId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetTargetNodeId() const {
    return m_byTargetNodeId;
}

/**
 * @func   GetTypeMessage
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetTypeMessage() const {
    return m_byTypeMessage;
}

/**
 * @func   GetFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetFunctionId() const {
    return m_byFunctionId;
}

/**
 * @func   GetCallbackId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetCallbackId() const {
    return m_byCallbackId;
}

/**
 * @func   IsCallbackRequired
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwMessage::IsCallbackRequired() const {
    return m_boCallbackRequired;
}

/**
 * @func   IsExpectedFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwMessage::IsExpectedFunctionId() const {
    return m_boReplyRequired;
}

/**
 * @func   GetExpectedFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetExpectedFunctionId() const {
    return m_byExpectedFunctionId;
}

/**
 * @func   GetExpectedCmdClassId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetExpectedCmdClassId() const {
    return m_byExpectedCmdClassId;
}

/**
 * @func   GetExpectedEndpointId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetExpectedEndpointId() const {
    return m_byExpectedEndpointId;
}

/**
 * @func   GetZwCommad
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage::Command
ZwMessage::GetZwCommad() const {
    return m_ZwCommand;
}

/**
 * @func   GetCountResend
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetCountResend() const {
    return m_byCountResend;
}

/**
 * @func   GetCountResend
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwMessage::IncCountResend() {
    m_byCountResend++;
}

/**
 * @func   GetNextCallbackId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwMessage::GetNextCallbackId() {
    sByNextCallbackId++;
    if (sByNextCallbackId > 127) {
        sByNextCallbackId = 1;
    }
    m_byCallbackId = sByNextCallbackId;

    return m_byCallbackId;
}
