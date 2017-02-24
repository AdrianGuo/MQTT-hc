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
 * Last Changed By:  ChinhMT
 * Revision:         Revision: 1.0
 * Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
 *
 * Note:
 *
 ******************************************************************************/


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <unistd.h>
#include "LogPlus.hpp"

#include "JsonAuthReq.hpp"
#include "JsonAuthRes.hpp"
#include "JsonKaliveReq.hpp"
#include "JsonKaliveRes.hpp"
#include "IO.hpp"

#include "NetManager.hpp"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/

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
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/
/**
 * @func   NetManager
 * @brief  None
 * @param  None
 * @retval None
 */
NetManager::NetManager(
    const_char_p cMacId
) : m_strMacId (cMacId),
    m_pSessionClient (NULL),
    m_pNetCtrllerTimer (RTimer::getTimerInstance()),
    m_dwAuthenRequestCount (0),
    m_dwKaliveRequestCount (0),
    m_iKeepAliveTimerHandle (-1),
    m_iAuthenRequestTimerHandle (-1),
    m_iConnectionTimerHandle (-1),
    m_boIsAlived (FALSE),
    m_boIsConnected (FALSE),
    m_pJsonNetSession (JsonNetSession::CreateSession()) {
    m_keepaliveTimerFunctor = makeFunctor(
    (TimerFunctor_p) NULL, *this, &NetManager::HandleKeepAliveProcess);
    m_authenRequestTimerFunctor = makeFunctor(
    (TimerFunctor_p) NULL, *this, &NetManager::HandleAuthenProcess);
    m_checkConectionTimerFunctor = makeFunctor(
    (TimerFunctor_p) NULL, *this, &NetManager::HandleConnectProcess);

    RegisterNetSession();
}

/**
 * @func   ~NetManager
 * @brief  None
 * @param  None
 * @retval None
 */
NetManager::~NetManager() {
    if (m_pJsonNetSession != NULL) {
        delete m_pJsonNetSession;
        m_pJsonNetSession = NULL;
    }

    m_strMacId.clear();

    while (!m_queNetCtrllerJsonCommand.empty()) {
        JsonCommand_p pJsonCommand = m_queNetCtrllerJsonCommand.front();
        m_queNetCtrllerJsonCommand.pop();
        delete pJsonCommand;
    }
}

/**
 * @func   RegisterNetSession
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::RegisterNetSession() {
    m_pJsonNetSession->MapJsonMessage<JsonAuthRes>(JsonAuthRes::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonAuthReq>(JsonAuthReq::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonKaliveRes>(JsonKaliveRes::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonKaliveReq>(JsonKaliveReq::GetStrCmd());
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::Process() {
    m_pNetCtrllerTimer->Process();
}

/**
 * @func   SetFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
NetManager::SetFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pICtrllerFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   SetSessionClient
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::SetSessionClient(
    SClient_p pSClient
) {
    m_pSessionClient = pSClient;
}
/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::Connect() {
    StartHandleConnect();
}

/**
 * @func   IsAuthenticated
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
NetManager::IsAuthenticated() const {
    return m_boAuthenticated;
}

/**
 * @func   HandlerNetCmdAuthRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::HandlerNetCmdAuthRes(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonAuthRes> jsonAuthRes =
    m_pJsonNetSession->GetJsonMapping<JsonAuthRes>();

    jsonAuthRes->ParseJsonCommand(pJsonCommand);

    if (jsonAuthRes->Ret() == 0) {
        LOG_INFO("authen success");
        m_boAuthenticated = TRUE;
        Notify(Reach);
    } else if (jsonAuthRes->Ret() == 1) {
        LOG_INFO("authen not active");
        Notify(NotReach);
    } else if (jsonAuthRes->Ret() == 2) {
        LOG_INFO("authen expired");
        Notify(NotReach);
    } else {
        LOG_INFO("authen error");
        Notify(NotReach);
    }
}

/**
 * @func   ProcessAuthenReq
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::ProcessAuthenReq() {
    LOG_INFO("authen request: %d", m_dwAuthenRequestCount);
    JsonMessagePtr<JsonAuthReq> jsonAuthReq =
    m_pJsonNetSession->GetJsonMapping<JsonAuthReq>();

    JsonCommand_p pJsonCommand = jsonAuthReq->CreateJsonCommand(m_strMacId);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    if ((m_pSessionClient != NULL) && (!m_boAuthenticated)) {
        m_pSessionClient->JsCommandToPacket(pJsonCommand);
    }
}

/**
 * @func   ProcessKaliveReq
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::ProcessKaliveReq() {
    LOG_INFO("keepalive request %d", m_dwKaliveRequestCount);

    JsonMessagePtr<JsonKaliveReq> jsonKaliveReq =
    m_pJsonNetSession->GetJsonMapping<JsonKaliveReq>();

    JsonCommand_p pJsonCommand = jsonKaliveReq->CreateJsonCommand();
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    if (m_boAuthenticated) {
        (*m_pICtrllerFunctor)(pJsonCommand);
    } else {
        delete pJsonCommand;
        pJsonCommand = NULL;
    }
}

/**
 * @func   HandlerNetCmdKaliveRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::HandlerNetCmdKaliveRes(
    JsonCommand_p pJsonCommand
) {
    m_boIsAlived = TRUE;
    LOG_INFO("keepalive success");
}

/**
 * @func   HandleConnectionProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::HandleConnectProcess(
    void_p pBuffer
) {
    if (m_boIsConnected || m_pSessionClient == NULL) {
        LOG_ERROR("init connection error");
        return;
    }

    if (m_pSessionClient->Connect()) {
        LOG_INFO("connect success");
        m_boIsConnected = TRUE;

        if (m_pSessionClient->Start()) {
            LOG_INFO("new thread success");
        } else {
            LOG_ERROR("new thread fail");
        }

        StopHandleConnect();
        m_boAuthenticated = FALSE;
        Notify(NotReach);
        ProcessAuthenReq();
        StartHandleAuthen();
    } else {
        LOG_ERROR("connect fail");
    }
}

/**
 * @func   HandleAuthenProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::HandleAuthenProcess(
    void_p pBuffer
) {
    if (m_boAuthenticated) {
        m_boIsAlived = FALSE;
        StopHandleAuthen();
        ProcessKaliveReq();
        StartHandleKeepAlive();
    } else {
        m_dwAuthenRequestCount++;

        if (m_dwAuthenRequestCount < AuthenMax) {
            ProcessAuthenReq();
        } else {
            m_dwAuthenRequestCount = 0;
            if (m_pSessionClient->Close()) {
                LOG_INFO("close success");
                m_boIsConnected = FALSE;
                m_boAuthenticated = FALSE;
                Notify(NotReach);
                StopHandleAuthen();
                StartHandleConnect();
            } else {
                LOG_ERROR("close fail");
            }
        }
    }
}

/**
 * @func   HandleKeepAliveProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::HandleKeepAliveProcess(
    void_p pBuffer
) {
    if (m_boIsAlived) {
        LOG_INFO("keepalive");
        m_boIsAlived = FALSE; // reset avlie state
        m_boIsConnected = TRUE;
        m_dwKaliveRequestCount++;
        ProcessKaliveReq(); // resend keepalive
    } else {
        LOG_WARN("lose connect");
        m_dwKaliveRequestCount = 0;
        if (m_pSessionClient->Close()) {
            LOG_INFO("close success");
            m_boIsConnected = FALSE;
            m_boAuthenticated = FALSE;
            Notify(NotReach);
            StopHandleKeepAlive();
            StartHandleConnect();
        } else {
            LOG_ERROR("close fail");
        }
    }
}

/**
 * @func   HandleStartConnect
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StartHandleConnect() {
    if (m_iConnectionTimerHandle == -1) {
        m_iConnectionTimerHandle = m_pNetCtrllerTimer->StartTimer(
        RTimer::Repeat::Forever, ConnectTimeout, &m_checkConectionTimerFunctor, NULL);
        LOG_INFO("start timer %2d [connect]", m_iConnectionTimerHandle);
    } else {
        LOG_INFO("connect working");
    }
}

/**
 * @func   HandleStopConnect
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StopHandleConnect() {
    if (m_iConnectionTimerHandle != -1) {
        LOG_INFO(" stop timer %2d [connect]", m_iConnectionTimerHandle);
        m_pNetCtrllerTimer->CancelTimer(m_iConnectionTimerHandle);
        m_iConnectionTimerHandle = -1;
    } else {
        LOG_INFO("connect stopped");
    }
}

/**
 * @func   HandleStartAuthen
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StartHandleAuthen() {
    if (m_iAuthenRequestTimerHandle == -1) {
        m_iAuthenRequestTimerHandle = m_pNetCtrllerTimer->StartTimer(
        RTimer::Repeat::Forever, AuthenTimeout, &m_authenRequestTimerFunctor, NULL);
        LOG_INFO("start timer %2d [authen]", m_iAuthenRequestTimerHandle);
    } else {
        LOG_INFO("authen working");
    }
}

/**
 * @func   HandleStopAuthen
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StopHandleAuthen() {
    if (m_iAuthenRequestTimerHandle != -1) {
        LOG_INFO(" stop timer %2d [authen]", m_iAuthenRequestTimerHandle);
        m_pNetCtrllerTimer->CancelTimer(m_iAuthenRequestTimerHandle);
        m_iAuthenRequestTimerHandle = -1;
    } else {
        LOG_INFO("authen stopped");
    }
}

/**
 * @func   HandleStartKeepAlive
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StartHandleKeepAlive() {
    if (m_iKeepAliveTimerHandle == -1) {
        m_iKeepAliveTimerHandle = m_pNetCtrllerTimer->StartTimer(
        RTimer::Repeat::Forever, KaliveTimeout, &m_keepaliveTimerFunctor, NULL);
        LOG_INFO("start timer %2d [kalive]", m_iKeepAliveTimerHandle);
    } else {
        LOG_INFO("kalive working");
    }
}

/**
 * @func   HandleStopKeepAlive
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetManager::StopHandleKeepAlive() {
    if (m_iKeepAliveTimerHandle != -1) {
        LOG_INFO(" stop timer %2d [kalive]", m_iKeepAliveTimerHandle);
        m_pNetCtrllerTimer->CancelTimer(m_iKeepAliveTimerHandle);
        m_iKeepAliveTimerHandle = -1;
    } else {
        LOG_INFO ("kalive working");
    }
}
