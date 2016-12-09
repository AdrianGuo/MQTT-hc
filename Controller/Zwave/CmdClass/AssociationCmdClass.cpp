#include <stddef.h>

#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ValueArray.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "AssociationCmdClass.hpp"

/**
 * @func   AssociationCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
AssociationCmdClass::AssociationCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) : ZwCmdClass(dwHomeId, byNodeId),
    m_byTransmitOptions (
        TRANSMIT_OPTION_ACK |
        TRANSMIT_OPTION_AUTO_ROUTE |
        TRANSMIT_OPTION_EXPLORE) {
}

/**
 * @func   CreateZwCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
AssociationCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    return new AssociationCmdClass(dwHomeId, byNodeId);
}

/**
 * @func   CreateZwCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
AssociationCmdClass::~AssociationCmdClass() {

}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
AssociationCmdClass::HandleMessage(
    u8_p pInBuffer,
    u8_t byLength
) {
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
AssociationCmdClass::Get(
    u8_t byGroupId
) {
    u32_t dwCount   = 0;
    u8_t byLength   = 3;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = ASSOCIATION_GET_V2;
    pbyBuffer[dwCount++] = byGroupId;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
    FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassId());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) { delete[] pbyBuffer; pbyBuffer = NULL; }

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
AssociationCmdClass::Set(
    u8_t byGroupId,
    u8_t byNodeId
) {
    u32_t dwCount   = 0;
    u8_t byLength   = 4;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = ASSOCIATION_SET_V2;
    pbyBuffer[dwCount++] = byGroupId;
    pbyBuffer[dwCount++] = byNodeId;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) { delete[] pbyBuffer; pbyBuffer = NULL; }

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
AssociationCmdClass::Set(
    u8_t byGroupId,
    Vector<Node_t> vecNodeId
) {
    u32_t dwCount   = 0;
    u8_t byLength   = vecNodeId.size() + 3;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = ASSOCIATION_SET_V2;
    pbyBuffer[dwCount++] = byGroupId;
    for (u32_t i = 0; i < vecNodeId.size(); i++) {
        pbyBuffer[dwCount++] = vecNodeId[i].id;
    }

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) { delete[] pbyBuffer; pbyBuffer = NULL; }

    return pZwMessage;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
AssociationCmdClass::Remove(
    u8_t byGroupId,
    u8_t byNodeId
) {
    u32_t dwCount   = 0;
    u8_t byLength   = 4;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = ASSOCIATION_REMOVE_V2;
    pbyBuffer[dwCount++] = byGroupId;
    pbyBuffer[dwCount++] = byNodeId;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) { delete[] pbyBuffer; pbyBuffer = NULL; }

    return pZwMessage;
}
