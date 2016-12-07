/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: VersionCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 16 Nov 2016 06:37:15
 *
 ******************************************************************************/
#include "ZwDefs.hpp"
#include "ZwNode.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "VersionCmdClass.hpp"

/**
 * @func   VersionCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
VersionCmdClass::VersionCmdClass(
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
 * @func   ~VersionCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
VersionCmdClass::~VersionCmdClass() {

}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
VersionCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueDevice = NULL;
    switch (byCommand) {
    case VERSION_REPORT_V2:
        break;
    case  VERSION_COMMAND_CLASS_GET_V2:
        {
            u8_t byCmdClassId = pbCommand[2];
            u8_t byVersion    = pbCommand[3];
            ZwNode_p pZwNode = GetNode();
            ZwCmdClass_p pZwCmdClass = pZwNode->GetZwCmdClass(byCmdClassId);
            pZwCmdClass->SetVersion(byVersion);
        }
        break;
    }

    return pValueDevice;
}

/**
 * @func   GetValue
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
VersionCmdClass::GetValue() {
    u32_t dwCount   = 0;
    u8_t byLength   = 2;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = VERSION_GET_V2;

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
VersionCmdClass::GetVersionCmdClass(
    u8_t byCmdClassId
) {
    u8_t byCount    = 0;
    u8_t byLength   = 3;
    u8_t byZwNodeId = GetNodeId();
    u8_p pbyBuffer  = new u8_t[byLength];

    pbyBuffer[byCount++] = GetZwCmdClassId();
    pbyBuffer[byCount++] = VERSION_COMMAND_CLASS_GET_V2;
    pbyBuffer[byCount++] = byCmdClassId;

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
