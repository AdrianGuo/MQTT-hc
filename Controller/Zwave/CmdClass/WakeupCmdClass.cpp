/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: WakeupCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 23:19:53
 *
 ******************************************************************************/
#include <stddef.h>
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "WakeupCmdClass.hpp"

/**
 * @func   WakeupCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
WakeupCmdClass::WakeupCmdClass(
    u32_t dwHomeId,
    u8_t byNodeId
) : ZwCmdClass (dwHomeId, byNodeId),
    m_byTransmitOptions (ZWAVE_PLUS_TX_OPTIONS) {
}

/**
 * @func   ~WakeupCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
WakeupCmdClass::~WakeupCmdClass() {

}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
WakeupCmdClass::HandleMessage(u8_p pbCommand, u8_t byLength) {
    return NULL;
}

/**
 * @func   GetValue
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
WakeupCmdClass::GetValue() {
    return NULL;
}
