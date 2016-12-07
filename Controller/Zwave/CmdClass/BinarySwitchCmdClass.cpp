#include <stddef.h>
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ValueByte.hpp"
#include "ValueSwitch.hpp"
#include "BinarySwitchCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
BinarySwitchCmdClass::BinarySwitchCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (
        TRANSMIT_OPTION_ACK |
        TRANSMIT_OPTION_AUTO_ROUTE |
        TRANSMIT_OPTION_EXPLORE
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
BinarySwitchCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    return new BinarySwitchCmdClass(dwHomeId, byNodeId);;
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
ValueDevice_p
BinarySwitchCmdClass::HandleBinarySwitchReport(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byLevel = pbCommand[0];
    ValueSwitch_p pValueSwitch = new ValueSwitch();

    pValueSwitch->SetLevel((byLevel > 0) ? 1 : 0);
    pValueSwitch->SetState((byLevel > 0) ? "on" : "off" );

    return pValueSwitch;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
BinarySwitchCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueSwitch = NULL;

    switch (byCommand) {
    case SWITCH_BINARY_REPORT_V2:
        pValueSwitch = HandleBinarySwitchReport(&pbCommand[2], byLength - 2);
        break;
    }

    return pValueSwitch;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
BinarySwitchCmdClass::GetValue() {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 2;
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = SWITCH_BINARY_GET_V2;

    ZwMessage_p pZwMessage = new
    ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
    FUNC_ID_APPLICATION_COMMAND_HANDLER, GetZwCmdClassId());
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength);
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

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
    bool_t boValue
) {
    u32_t dwCount   = 0;
    u8_t byValue    = boValue ? 0xFF : 0x00;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 3;
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_BINARY_SET_V2; // Command
    pbyBuffer[dwCount++] = byValue;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

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
    ValueDevice_p pValueDevice
) {
    ValueSwitch_p pValueSwitch = (ValueSwitch_p) pValueDevice;
    u32_t dwCount   = 0;
    u8_t byValue    = pValueSwitch->Level();
    u8_t byZwNodeId = GetNodeId();
    u8_t byData     = byValue;
    u8_t byLength   = 3;
    u8_p pbyBuffer  = new u8_t[byLength];

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

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_BINARY_SET_V2; // Command
    pbyBuffer[dwCount++] = byData;

    ZwMessage_p pZwMessage = new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
    pZwMessage->ResetPacket(byLength + 4);

    pZwMessage->Push(byZwNodeId);
    pZwMessage->Push(byLength); // length
    pZwMessage->Push(pbyBuffer, byLength);
    pZwMessage->Push(m_byTransmitOptions);
    pZwMessage->Push(pZwMessage->GetNextCallbackId());

    if (pbyBuffer != NULL) {
        delete[] pbyBuffer;
        pbyBuffer = NULL;
    }

    return pZwMessage;
}
