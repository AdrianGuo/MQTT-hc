#include <stddef.h>
#include "ZwDefs.hpp"
#include "ValueByte.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "BasicCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BasicCmdClass::BasicCmdClass(
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
BasicCmdClass::CreateZwCmdClass(
    u32_t dwHomeID,
    u8_t  byNodeID
) {
    return new BasicCmdClass(dwHomeID, byNodeID);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BasicCmdClass::~BasicCmdClass() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const u8_t
BasicCmdClass::GetZwCmdClassID() {
    return COMMAND_CLASS_BASIC_V2;
}

/**
* @func
* @brief  None
* @param  None
* @retval None
*/
u8_t
BasicCmdClass::GetMaxVersion() const {
   return  BASIC_VERSION_V2;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Value_p
BasicCmdClass::HandleMessage(
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
BasicCmdClass::GetValue() {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byLength = 2;
    u8_p pbyBuffer = new u8_t[2];
    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = BASIC_GET_V2;

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
BasicCmdClass::SetValue(
    Value& inParam
) {
    if (inParam.GetType() == ValueType_Byte) {
        ValueByte_p pTmpValue = (ValueByte_p) (&inParam);
        u32_t dwCount = 0;
        u8_t byValue = pTmpValue->GetValue();
        u8_t byZwNodeID = GetNodeID();
        u8_t byData = 0;
        u8_t byLength = 3;
        u8_p pbyBuffer = new u8_t[3];

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

        pbyBuffer[dwCount++] = GetZwCmdClassID();
        pbyBuffer[dwCount++] = BASIC_SET_V2;
        pbyBuffer[dwCount++] = byData;

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
BasicCmdClass::SetValue(
    u8_t byParam
) {
    u32_t dwCount = 0;
    u8_t byZwNodeID = GetNodeID();
    u8_t byData = 0;
    u8_t byLength = 3;
    u8_p pbyBuffer = new u8_t[3];

    if (byParam == 0) {
        byData = 0;
    } else if ((byParam < 0x64) || (byParam = 0xFF)) {
        byData = 0xFF;
    } else {
        return NULL;
    }

    pbyBuffer[dwCount++] = GetZwCmdClassID();
    pbyBuffer[dwCount++] = BASIC_SET_V2;
    pbyBuffer[dwCount++] = byData;

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
