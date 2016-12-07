/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Crc16EncapCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 16 Nov 2016 00:19:35
 *
 ******************************************************************************/
#include <stddef.h>
#include "HelperHc.hpp"
#include "ZwDefs.hpp"
#include "ZwNode.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "Crc16EncapCmdClass.hpp"

/**
 * @func   CrC16EncapCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
CrC16EncapCmdClass::CrC16EncapCmdClass(
    u32_t dwHomeId,
    u8_t byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (
        TRANSMIT_OPTION_ACK |
        TRANSMIT_OPTION_AUTO_ROUTE |
        TRANSMIT_OPTION_EXPLORE
) {
}

/**
 * @func   ~CrC16EncapCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
CrC16EncapCmdClass::~CrC16EncapCmdClass() {

}

/**
 * @func   HandleCrc16Encap
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
CrC16EncapCmdClass::HandleCrc16Encap(
    u8_p pbCommand,
    u8_t byLength
) {
    ValueDevice_p pValueDevice = NULL;
    u16_t wCrcMsg =
    MERGE(pbCommand[byLength - 2], pbCommand[byLength - 1]);
    u16_t wCrcCal = CRC16(pbCommand, byLength - 2);

    if (wCrcMsg == wCrcCal) { // Correct CRC
        u8_t byCmdClassId = pbCommand[2];
        ZwNode_p pZwNode = GetNode();
        pValueDevice =
        pZwNode->ApplicationCommandHandler(&pbCommand[2], byLength - 2);
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
CrC16EncapCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueDevice = NULL;

    switch (byCommand) {
    case CRC_16_ENCAP:
        pValueDevice = HandleCrc16Encap(&pbCommand[2], byLength - 2);
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
ZwMessage_p
CrC16EncapCmdClass::GetValue() {
    return NULL;
}
