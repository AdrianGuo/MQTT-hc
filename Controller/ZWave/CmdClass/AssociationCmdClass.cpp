#include <stddef.h>
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ValueArray.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "AssociationCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
AssociationCmdClass::AssociationCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) : ZwCmdClass(dwHomeID, byNodeID) {
    m_byTransmitOptions = (TRANSMIT_OPTION_ACK | TRANSMIT_OPTION_AUTO_ROUTE | TRANSMIT_OPTION_EXPLORE);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
AssociationCmdClass::CreateZwCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    return new AssociationCmdClass(dwHomeID, byNodeID);
}

/**
 * @func
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
const u8_t
AssociationCmdClass::GetZwCmdClassID() {
    return COMMAND_CLASS_ASSOCIATION_V2;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
AssociationCmdClass::GetMaxVersion() const {
    return ASSOCIATION_VERSION_V2;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
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
AssociationCmdClass::GetValue(
    Value& inParam
) {
    if (inParam.GetType() == ValueType_Byte) {
        ValueByte_p pTempValue = (ValueByte_p) (&inParam);
        u32_t dwCount = 0;
        u8_t byZwNodeID = GetNodeID();
        u8_t byLength = 2;
        u8_p pbyBuffer = new u8_t[3];
        pbyBuffer[dwCount++] = GetZwCmdClassID();
        pbyBuffer[dwCount++] = ASSOCIATION_GET_V2;
        pbyBuffer[dwCount++] = pTempValue->GetValue();

        ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
                FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassID());
        pZwMessage->ResetPacket(byLength + 4);

        pZwMessage->Push(byZwNodeID);
        pZwMessage->Push(byLength);
        pZwMessage->Push(pbyBuffer, byLength);
        pZwMessage->Push(m_byTransmitOptions);
        pZwMessage->Push(pZwMessage->GetNextCallbackID());

        if (pbyBuffer != NULL) {
            delete pbyBuffer;
            pbyBuffer = NULL;
        }

        return pZwMessage;
    }
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
AssociationCmdClass::SetValue(
    Value& inParam
) {
    if (inParam.GetType() == ValueType_Array) {
        ValueArray_p pTmpValue = (ValueArray_p) (&inParam);
        u32_t dwCount = 0;
        u8_t byZwNodeID = GetNodeID();
        u8_t byLength = pTmpValue->GetLength() + 2;
        u8_p pbyBuffer = new u8_t[byLength];

        pbyBuffer[dwCount++] = GetZwCmdClassID();
        pbyBuffer[dwCount++] = ASSOCIATION_SET_V2;
        for (u8_t i = 0; i < pTmpValue->GetLength(); i++) {
            pbyBuffer[dwCount++] = pTmpValue->GetValue()[i];
        }

        ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
        pZwMessage->ResetPacket(byLength + 4);

        pZwMessage->Push(byZwNodeID);
        pZwMessage->Push(byLength);
        pZwMessage->Push(pbyBuffer, byLength);
        pZwMessage->Push(m_byTransmitOptions);
        pZwMessage->Push(pZwMessage->GetNextCallbackID());

        if (pbyBuffer != NULL) {
            delete[] pbyBuffer;
            pbyBuffer = NULL;
        }

        return pZwMessage;
    }
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
    u8_t byGroupID
) {
    u32_t dwCount = 0;
    u8_t byLength = 3;
    u8_t byZwNodeID = GetNodeID();
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = ASSOCIATION_GET_V2;
    pbyBuffer[dwCount++] = byGroupID;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

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
    u8_t byGroupID,
    u8_t byNodeID
) {
    u32_t dwCount = 0;
    u8_t byLength = 4;
    u8_t byZwNodeID = GetNodeID();
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = ASSOCIATION_SET_V2;
    pbyBuffer[dwCount++] = byGroupID;
    pbyBuffer[dwCount++] = byNodeID;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

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
    u8_t byGroupID,
    u8_t byNodeID
) {
    u32_t dwCount = 0;
    u8_t byLength = 4;
    u8_t byZwNodeID = GetNodeID();
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = ASSOCIATION_REMOVE_V2;
    pbyBuffer[dwCount++] = byGroupID;
    pbyBuffer[dwCount++] = byNodeID;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}
