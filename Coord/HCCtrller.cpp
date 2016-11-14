/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
*
* Description:      Include file for application
*
* Author:
*
* Last Changed By:  TrungTQ
* Revision:         Revision: 1.0
* Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
*
* Note:             
*
*******************************************************************************/

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stddef.h>
#include "debug.hpp"
#include "HCCtrller.hpp"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/
#ifndef DEBUG_HCCTRLLER
#define debug_ctrller(x)
#else /* DEBUG_HCCTRLLER */
#define debug_ctrller(x)                    DEBUG1(x)
#endif /* DEBUG_HCCTRLLER */
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/
/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
HCCtrller::HCCtrller(
    SClient_p pSClient
) {
    m_pSClient = pSClient;
    m_pHCCtrllerLocker = new Locker();
    m_HCCtrllerFunctor = makeFunctor((HCCtrllerFunctor_p) NULL, *this, &HCCtrller::RecvCommandFromSession);
    m_ICtrllerFunctor  = makeFunctor((ICtrllerFunctor_p)  NULL, *this, &HCCtrller::RecvCommandFromModules);
    SendFunctor();
}

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
HCCtrller::~HCCtrller() {
    if (m_pHCCtrllerLocker != NULL) {
        delete m_pHCCtrllerLocker;
        m_pHCCtrllerLocker = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::SendFunctor() {
    if (m_pSClient != NULL) {
        m_pSClient->SClientRecvFunctor(&m_HCCtrllerFunctor);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::AddCtrller(
    ICtrller_p pCtrller
) {
    m_pHCCtrllerLocker->Lock();
    if (pCtrller != NULL) {
        m_vecCtrller.push_back(pCtrller);
        pCtrller->ICtrllerRecvFunctor(&m_ICtrllerFunctor);
    }
    m_pHCCtrllerLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
HCCtrller::RecvCommandFromSession(
    JsonCommand_p pJsonCommand
) {
    m_pHCCtrllerLocker->Lock();
    if (pJsonCommand->IsJsonAvailable()) {
        m_queJsonCommand.push(pJsonCommand);
        m_pHCCtrllerLocker->UnLock();
        return TRUE;
    }
    m_pHCCtrllerLocker->UnLock();
    return FALSE;
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
HCCtrller::RecvCommandFromModules(
    JsonCommand_p pJsonCommand
) {
    m_pHCCtrllerLocker->Lock();
    if (pJsonCommand->IsJsonAvailable()) {
        m_queJsonCommand.push(pJsonCommand);
        m_pHCCtrllerLocker->UnLock();
        return TRUE;
    }
    m_pHCCtrllerLocker->UnLock();
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
HCCtrller::Process() {
    JsonCommand_p pJsonCommand = NULL;

    m_pHCCtrllerLocker->Lock();
    if (!m_queJsonCommand.empty()) {
        pJsonCommand = m_queJsonCommand.front();
    }
    m_pHCCtrllerLocker->UnLock();

    if (pJsonCommand != NULL) {
        bool_t boPendingFlag = TRUE;
        for (u32_t i = 0; i < m_vecCtrller.size(); i++) {
            if (m_vecCtrller[i]->LoadCmdClass(pJsonCommand)) {
                boPendingFlag = FALSE;

                m_pHCCtrllerLocker->Lock();
                m_queJsonCommand.pop();
                m_pHCCtrllerLocker->UnLock();

                break;
            }
        }

        if (boPendingFlag) {
            m_pHCCtrllerLocker->Lock();
            m_quePedingJsonCommand.push(pJsonCommand);
            m_queJsonCommand.pop();
            m_pHCCtrllerLocker->UnLock();
        }
    }

    for (u32_t i = 0; i < m_vecCtrller.size(); i++) {
        m_vecCtrller[i]->Process();
    }
}

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
