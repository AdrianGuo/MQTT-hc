/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-11-01 12:40:00 (Tue, 01 Nov 2016)
 *
 ******************************************************************************/

#include "HandlerRequest.hpp"

HandlerRequest_p HandlerRequest::m_instance = NULL;

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
HandlerRequest_p
HandlerRequest::GetInstance() {
    if (m_instance == NULL) {
        m_instance = new HandlerRequest();
    }
    return m_instance;
}

/**
 * @func   RegisterHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HandlerRequest::RegisterHandler(
    u8_t byHandleId,
    HandlerRequestFunctor_t funcTor
) {
    m_mapHandlerRequest[byHandleId] = funcTor;
}

/**
 * @func   ProcessFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HandlerRequest::ProcessFunctor(
    ZwPacket_p pZwPacket
) {
    u8_t byHandleId = pZwPacket->GetFunctionId();
    m_mapHandlerRequest[byHandleId](pZwPacket);
}
