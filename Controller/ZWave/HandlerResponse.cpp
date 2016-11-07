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

#include "HandlerResponse.hpp"

HandlerResponse_p HandlerResponse::m_instance = NULL;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
HandlerResponse_p
HandlerResponse::GetInstance() {
    if (m_instance == NULL) {
        m_instance = new HandlerResponse();
    }
    return m_instance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HandlerResponse::RegisterHandler(
    u8_t byHandleId,
    HandlerResponseFunctor_t funcTor
) {
    m_mapHandlerResponse[byHandleId] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HandlerResponse::ProcessFunctor(
    ZwPacket_p pZwPacket
) {
    u8_t byHandleId = pZwPacket->GetFunctionID();
    m_mapHandlerResponse[byHandleId](pZwPacket);
}
