#include <stddef.h>
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ValueByte.hpp"
#include "ValueDimmer.hpp"
#include "MultilevelSwitchCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
MultilevelSwitchCmdClass::MultilevelSwitchCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (ZWAVE_PLUS_TX_OPTIONS) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
MultilevelSwitchCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    return new MultilevelSwitchCmdClass(dwHomeId, byNodeId);;
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
ValueDevice_p
MultilevelSwitchCmdClass::HandleMultilevelSwitchReport(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byValue = pbCommand[0];
    ValueDimmer_p pValueDimmer = new ValueDimmer();

    pValueDimmer->SetLevel((byValue > 100) ? 99 : byValue);
    pValueDimmer->SetState((byValue > 0) ? "on" : "off" );

    return pValueDimmer;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
MultilevelSwitchCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueDimmer = NULL;

    switch (byCommand) {
    case SWITCH_MULTILEVEL_REPORT_V4:
        pValueDimmer = HandleMultilevelSwitchReport(&pbCommand[2], byLength - 2);
        break;
    }
    return pValueDimmer;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
MultilevelSwitchCmdClass::GetValue() {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 2;
    u8_p pbyBuffer  = new u8_t[byLength];
    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_GET_V4;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE, TRUE,
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
MultilevelSwitchCmdClass::SetValue(
    u8_t byParam
) {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLevel    = byParam;
    u8_t byLength   = 3;
    u8_p pbyBuffer  = new u8_t[byLength];

    if ((byParam > 0x63) && (byParam < 0xFF)) {
        byLevel = 0x63;
    }

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_SET_V4; // Command
    pbyBuffer[dwCount++] = byLevel;

    ZwMessage_p pZwMessage =
    new ZwMessage(byZwNodeId, REQUEST, FUNC_ID_ZW_SEND_DATA, TRUE);
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
MultilevelSwitchCmdClass::SetValue(
    ValueDevice_p pValueDevice
) {
    ValueDimmer_p pValueDimmer = (ValueDimmer_p) pValueDevice;
    u32_t dwCount   = 0;
    u8_t byValue    = pValueDimmer->Level();
    u8_t byZwNodeId = GetNodeId();
    u8_t byLevel    = byValue;
    u8_t byLength   = 3;
    u8_p pbyBuffer  = new u8_t[byLength];

    if ((byValue > 0x63) && (byValue < 0xFF)) {
        byLevel = 0x63;
    }

    pbyBuffer[dwCount++] = GetZwCmdClassId(); // Comand Class
    pbyBuffer[dwCount++] = SWITCH_MULTILEVEL_SET_V4; // Command
    pbyBuffer[dwCount++] = byLevel;

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
