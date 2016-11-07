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
 * Last Changed:     Date: 2016-10-31 10:15:00 (Mon, 31 Oct 2016)
 *
 ******************************************************************************/
#include "JsonZwaveSession.hpp"

JsonRecvZwaveSession_p JsonRecvZwaveSession::m_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonRecvZwaveSession_p
JsonRecvZwaveSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonRecvZwaveSession();
    }
    return m_pInstance;
}


JsonSendZwaveSession_p JsonSendZwaveSession::m_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonSendZwaveSession_p
JsonSendZwaveSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonSendZwaveSession();
    }
    return m_pInstance;
}
