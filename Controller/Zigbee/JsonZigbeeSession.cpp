/*
 * JsonZigbeeSession.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <JsonZigbeeSession.hpp>

JsonRecvZigbeeSession_p JsonRecvZigbeeSession::m_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonRecvZigbeeSession_p
JsonRecvZigbeeSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonRecvZigbeeSession();
    }
    return m_pInstance;
}


JsonSendZigbeeSession_p JsonSendZigbeeSession::m_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonSendZigbeeSession_p
JsonSendZigbeeSession::CreateSession() {
    if (m_pInstance == NULL) {
        m_pInstance = new JsonSendZigbeeSession();
    }
    return m_pInstance;
}

