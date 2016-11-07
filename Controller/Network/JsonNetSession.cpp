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
#include "JsonNetSession.hpp"

JsonNetSession_p JsonNetSession::m_instance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonNetSession_p
JsonNetSession::CreateSession() {
    if (m_instance == NULL) {
        m_instance = new JsonNetSession();
    }
    return m_instance;
}
