/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SensorBinaryCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 23:44:56
 *
 ******************************************************************************/
#include "ZwDefs.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwTransportAPI.hpp"
#include "SensorBinaryCmdClass.hpp"

/**
 * @func   SensorBinaryCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
SensorBinaryCmdClass::SensorBinaryCmdClass(
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
 * @func   ~SensorBinaryCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
SensorBinaryCmdClass::~SensorBinaryCmdClass() {

}

/**
 * @func   HandleMessage
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDevice_p
SensorBinaryCmdClass::HandleMessage(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommand = pbCommand[1];
    ValueDevice_p pValueDevice = NULL;
    return pValueDevice;
}
