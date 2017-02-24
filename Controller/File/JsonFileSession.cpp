/*
 * JsonRecvFileSession.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#include <File/JsonFileSession.hpp>
#include <File/JsonFileSession.hpp>

JsonRecvFileSession_p JsonRecvFileSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonRecvFileSession_p
JsonRecvFileSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonRecvFileSession();
    }
    return m_pInstance;
}


JsonSendFileSession_p JsonSendFileSession::m_pInstance = NULL;

/**
 * @func   CreateSession
 * @brief  None
 * @param  None
 * @retval None
 */
JsonSendFileSession_p
JsonSendFileSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonSendFileSession();
    }
    return m_pInstance;
}


