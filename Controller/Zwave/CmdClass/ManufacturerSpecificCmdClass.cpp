/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ManufacturerSpecificCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 23:23:07
 *
 ******************************************************************************/
#include <stddef.h>
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "ManufacturerSpecificCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ManufacturerSpecificCmdClass::ManufacturerSpecificCmdClass(
    u32_t dwHomeId,
    u8_t byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (
        TRANSMIT_OPTION_ACK |
        TRANSMIT_OPTION_AUTO_ROUTE |
        TRANSMIT_OPTION_EXPLORE),
    m_pDbModel (ZwDbModel::CreateModel("zwave.db")
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ManufacturerSpecificCmdClass::~ManufacturerSpecificCmdClass() {

}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
ManufacturerSpecificCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    return NULL;
}

/**
 * @func   GetValue
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ManufacturerSpecificCmdClass::GetValue() {
    return NULL;
}
