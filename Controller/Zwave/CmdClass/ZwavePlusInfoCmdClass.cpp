/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwPlusInfoCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 14 Nov 2016 14:25:24
 *
 ******************************************************************************/
#include <stddef.h>

#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ZwNode.hpp"
#include "ZwavePlusInfoCmdClass.hpp"

/**
 * @func   ZwavePlusInfoCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
ZwavePlusInfoCmdClass::ZwavePlusInfoCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (ZWAVE_PLUS_TX_OPTIONS),
    m_pDbModel (ZwDbModel::CreateModel("zwave.db")) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
ZwavePlusInfoCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t  byNodeId
) {
    return new ZwavePlusInfoCmdClass(dwHomeId, byNodeId);;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwavePlusInfoCmdClass::~ZwavePlusInfoCmdClass() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
ZwavePlusInfoCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    ValueDevice_p pValueDevice = NULL;
    u8_t byCommand = pbCommand[1];
    switch (byCommand) {
    case ZWAVEPLUS_INFO_REPORT:
        pValueDevice = HandleZwPlusInfoReport(&pbCommand[1], byLength - 2);
        break;
    }
    return pValueDevice;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
ZwavePlusInfoCmdClass::HandleZwPlusInfoReport(
    u8_p pbCommand,
    u8_t byLength
) {
    ValueDevice_p pValueDevice = NULL;
    ZwNode_p pZwNode = GetNode();
    u8_t byVersion  = pbCommand[0];
    u8_t byRoleType = pbCommand[1];
    u8_t byNodeType = pbCommand[2];
    u16_t wIconType = MERGE(pbCommand[3], pbCommand[4]);

    pZwNode->SetVersion (byVersion);
    pZwNode->SetRoleType(byRoleType);
    pZwNode->SetNodeType(byNodeType);
    pZwNode->SetIconType(wIconType);

    ZwDbDevice devicefind = m_pDbModel->Find<ZwDbDeviceItem>().
    Where("NodeId = ?").Bind(GetNodeId()).Where("Position = ?").Bind(0);

    if (devicefind.get() != NULL) {
        devicefind.Modify()->Version  = byVersion;
        devicefind.Modify()->RoleType = byRoleType;
        devicefind.Modify()->NodeType = byNodeType;
        devicefind.Modify()->IconType = wIconType;
        m_pDbModel->Add(devicefind);
        m_pDbModel->UpdateChanges();
    }

    return pValueDevice;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwavePlusInfoCmdClass::GetValue() {
    u32_t dwCount   = 0;
    u8_t byZwNodeId = GetNodeId();
    u8_t byLength   = 2;
    u8_p pbyBuffer  = new u8_t[byLength];
    pbyBuffer[dwCount++] = GetZwCmdClassId();
    pbyBuffer[dwCount++] = ZWAVEPLUS_INFO_GET;

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
