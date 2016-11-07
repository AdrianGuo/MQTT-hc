#include <stddef.h>
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "MultilevelSwitchCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultilevelSwitchCmdClass::MultilevelSwitchCmdClass(
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
MultilevelSwitchCmdClass::CreateZwCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    return new MultilevelSwitchCmdClass(dwHomeID, byNodeID);;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultilevelSwitchCmdClass::~MultilevelSwitchCmdClass() {

}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
MultilevelSwitchCmdClass::GetMaxVersion() const {
   return  SWITCH_MULTILEVEL_VERSION_V4;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const u8_t
MultilevelSwitchCmdClass::GetZwCmdClassID() {
    return COMMAND_CLASS_SWITCH_MULTILEVEL_V4;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
MultilevelSwitchCmdClass::HandleMessage(
    u8_p pInBuffer,
    u8_t byLength
) {
    u8_t byValue = pInBuffer[2];
    ValueByte_p pOutValue = new ValueByte();
    pOutValue->SetValue(byValue);

    return pOutValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultilevelSwitchCmdClass::GetValue() {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 2;
    u8_p pbyBuffer = new u8_t[byLength];
    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_GET_V4;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
            FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassID());
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
MultilevelSwitchCmdClass::SetValue(
    Value& inParam
) {
    if (inParam.GetType() == ValueType_Byte) {
        ValueByte_p pTmpValue = (ValueByte_p) (&inParam);
        u32_t dwCount = 0;
        u8_t byValue = pTmpValue->GetValue();
        u8_t byZwNodeID = GetNodeID();
        u8_t byLevel = byValue;
        u8_t byLength = 3;
        u8_p pbyBuffer = new u8_t[byLength];

        if ((byValue > 0x63) && (byValue < 0xFF)) {
            byLevel = 0x63;
        }

        pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
        pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_SET_V4; // Command
        pbyBuffer[dwCount++] = byLevel;

        ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
        pZwMessage->ResetPacket(byLength + 4);
        pZwMessage->Push(byZwNodeID);
        pZwMessage->Push(byLength); // length
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
MultilevelSwitchCmdClass::SetValue(
    u8_t byParam
) {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLevel = byParam;
    u8_t byLength = 3;
    u8_p pbyBuffer = new u8_t[byLength];

    if ((byParam > 0x63) && (byParam < 0xFF)) {
        byLevel = 0x63;
    }

    pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_SET_V4; // Command
    pbyBuffer[dwCount++] = byLevel;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeID, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);
    pZwMessage->Push(byZwNodeID);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackID());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}
