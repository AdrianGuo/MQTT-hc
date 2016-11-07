#include <stddef.h>
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "BinarySwitchCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BinarySwitchCmdClass::BinarySwitchCmdClass(
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
BinarySwitchCmdClass::CreateZwCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    return new BinarySwitchCmdClass(dwHomeID, byNodeID);;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BinarySwitchCmdClass::~BinarySwitchCmdClass() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const u8_t
BinarySwitchCmdClass::GetZwCmdClassID() {
    return COMMAND_CLASS_SWITCH_BINARY_V2;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
BinarySwitchCmdClass::GetMaxVersion() const {
   return  SWITCH_BINARY_VERSION_V2;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
BinarySwitchCmdClass::HandleMessage(
    u8_p pInBuffer,
    u8_t byLength
) {
    u8_t byValue = pInBuffer[2];
    ValueByte_p pOutValue = new ValueByte();
    pOutValue->SetValue((byValue == 0) ? 0 : 1);

    return pOutValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
BinarySwitchCmdClass::GetValue() {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 2;
    u8_p pbyBuffer = new u8_t[byLength];
    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = SWITCH_BINARY_GET_V2;

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
BinarySwitchCmdClass::SetValue(
    Value& inParam
) {
    if (inParam.GetType() == ValueType_Byte) {
        ValueByte_p pTmpValue = (ValueByte_p) (&inParam);
        u32_t dwCount = 0;
        u8_t byValue = pTmpValue->GetValue();
        u8_t byZwNodeID = GetNodeID();
        u8_t byData = byValue;
        u8_t byLength = 3;
        u8_p pbyBuffer = new u8_t[byLength];

        if (byValue == 0) {
            byData = 0;
        } else if ((byValue < 0x64) || (byValue = 0xFF)) {
            byData = 0xFF;
        } else {
            if (pbyBuffer != NULL) {
                delete[] pbyBuffer;
                pbyBuffer = NULL;
            }
            return NULL;
        }

        pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
        pbyBuffer[dwCount++] = SWITCH_BINARY_SET_V2; // Command
        pbyBuffer[dwCount++] = byData;

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
BinarySwitchCmdClass::SetValue(
    bool_t boParam
) {
    u32_t dwCount = 0;
    u8_t byValue = boParam ? 0xFF : 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 3;
    u8_p pbyBuffer = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassID(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_BINARY_SET_V2; // Command
    pbyBuffer[dwCount++] = byValue;

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
