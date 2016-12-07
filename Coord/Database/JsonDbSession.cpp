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
#include "JsonDbSession.hpp"

JsonRecvDbSession_p JsonRecvDbSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonRecvDbSession_p
JsonRecvDbSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonRecvDbSession();
    }
    return m_pInstance;
}


JsonSendDbSession_p JsonSendDbSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonSendDbSession_p
JsonSendDbSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonSendDbSession();
    }
    return m_pInstance;
}
