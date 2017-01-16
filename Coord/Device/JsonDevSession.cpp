/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: JsonDevSession.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 09:33:59
 *
 ******************************************************************************/
#include "JsonDevSession.hpp"

JsonRecvDevSession_p JsonRecvDevSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonRecvDevSession_p
JsonRecvDevSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonRecvDevSession();
    }
    return m_pInstance;
}


JsonSendDevSession_p JsonSendDevSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonSendDevSession_p
JsonSendDevSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonSendDevSession();
    }
    return m_pInstance;
}
