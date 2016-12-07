/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: NotificationCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 23:29:34
 *
 ******************************************************************************/
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "NotificationCmdClass.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
NotificationCmdClass::NotificationCmdClass(
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
NotificationCmdClass::~NotificationCmdClass() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdClass_p
NotificationCmdClass::CreateZwCmdClass(
    u32_t dwHomeId,
    u8_t byNodeId
) {
    return new NotificationCmdClass(dwHomeId, byNodeId);
}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
NotificationCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueDevice = NULL;
    switch (byCommand) {
    case NOTIFICATION_REPORT_CO2_V3:
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
NotificationCmdClass::GetValue() {
    return NULL;
}
