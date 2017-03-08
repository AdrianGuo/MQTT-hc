/*
 * ZbCtrller.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <stddef.h>
#include "LogPlus.hpp"
#include "LogCommand.hpp"
#include "LThread.hpp"

#include "ZbCtrller.hpp"

/**
 * @func   ZbCtrller
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZbCtrller::ZbCtrller(
    const_char_p chPortname
) : m_pZbDriver(ZbDriver::GetInstance(chPortname)),
    m_queSerRecvMsg(m_pZbDriver->GetSerRecvMsgQueue())
{
    m_pZbCtrllerThread = new LThread();
    m_pZbCtrllerLocker = new Locker();
    m_ZbCtrllerThreadFunctor = makeFunctor(
            (threadFunctor_p) NULL, *this, &ZbCtrller::ZbCtrlllerThreadProc);
    m_pZbCtrllerThread->RegThreadFunctor(&m_ZbCtrllerThreadFunctor);

    m_evWaitMsgSignal.Reset();

    m_pCtrllerFunctor = NULL;
    m_ZbCtrllerFunctor = makeFunctor(
            (ZbCtrllerFunctor_p) NULL, *this, &ZbCtrller::PushJsonCommand);

    CtrllerSendFunctor();
}

/**
 * @func   ~ZbCtrller
 * @brief  Destructor
 * @param  None
 * @retval None
 */
ZbCtrller::~ZbCtrller() {
    if (m_pZbCtrllerThread != NULL) {
        delete m_pZbCtrllerThread;
        m_pZbCtrllerThread = NULL;
    }

    if (m_pZbCtrllerLocker != NULL) {
        delete m_pZbCtrllerLocker;
        m_pZbCtrllerLocker = NULL;
    }
    if (m_pCtrllerFunctor != NULL) {
        delete m_pCtrllerFunctor;
        m_pCtrllerFunctor = NULL;
    }
    delete m_pZbDriver;
    delete m_pZbCtrllerThread;
    delete m_pZbCtrllerLocker;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::CtrllerSendFunctor() {
    m_pZbDriver->ZbDriverRecvFunctor(&m_ZbCtrllerFunctor);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::CtrllerRecvFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pCtrllerFunctor = pRecvFunctor;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::Connect() {
    if (m_pZbDriver->Connect()) {
        LOG_INFO("Zigbee connect success");
    } else {
        LOG_INFO("Zigbee connect fail");
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::Start() {
    if (m_pZbDriver->Start()) {
        LOG_INFO("START SUCCESS");
    } else {
        LOG_INFO("START FAIL");
    }

    if (m_pZbCtrllerThread != NULL) {
        m_pZbCtrllerThread->Start();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcessHandler(
    JsonCommand_p pJsonCommand
) {
	if(pJsonCommand == NULL) return;

    m_pZbCtrllerLocker->Lock();
    m_queSerRecvMsg.push(pJsonCommand);
    m_pZbCtrllerLocker->UnLock();
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::PushJsonCommand(
    EvAct evAction,
    void_p pInBuffer
) {
//    if (evAction == Set1) {
//        m_evWaitMsgSignal.Set();
//    } else if (evAction == Reset) {
//        m_evWaitMsgSignal.Reset();
//    } else if (evAction == Pushback) {
//        ZbMessage_p pZbMsg = (ZbMessage_p) pInBuffer;
//        m_pZbCtrllerLocker->Lock();
//        if (pZbMsg != NULL) { m_queSendZbMsg.push(pZbMsg); }
//        m_pZbCtrllerLocker->UnLock();
//
//        return;
//    }

    if ((m_pCtrllerFunctor != NULL) && (pInBuffer != NULL)) {
        (*m_pCtrllerFunctor)((JsonCommand_p) pInBuffer);
    }
}

/**
 * @func   ZbCtrlllerThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
ZbCtrller::ZbCtrlllerThreadProc(
    void_p pInBuffer
) {
    while (TRUE) {
    	JsonCommand_p pJsonCommand = NULL;
        m_pZbCtrllerLocker->Lock();
        if (!m_queSerRecvMsg.empty()) {
        	pJsonCommand = m_queSerRecvMsg.front();
            m_queSerRecvMsg.pop();
        }
        m_pZbCtrllerLocker->UnLock();

        if (pJsonCommand != NULL) {
            m_pZbDriver->ProcSerRecvMsg(pJsonCommand);
            delete pJsonCommand;
        }
        usleep(50000);
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::Init() {
    m_pZbDriver->Init();
}
