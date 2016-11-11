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
*******************************************************************************/


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <unistd.h>
#include "debug.hpp"
#include "json-forwards.h"
#include "json.h"

#include "JsonAuthReq.hpp"
#include "JsonAuthRes.hpp"
#include "JsonDevRep.hpp"
#include "JsonKaliveReq.hpp"
#include "JsonKaliveRes.hpp"
#include "JsonZbLstAdd.hpp"
#include "JsonZbLstDel.hpp"
#include "JsonZwLstAdd.hpp"
#include "JsonZwLstDel.hpp"
#include "JsonZbStt.hpp"
#include "JsonZbResetRes.hpp"
#include "NetCtrller.hpp"

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
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
NetCtrller::NetCtrller(
    SClient_p pSClient,
    const_char_p cMacID
) : m_pSessionClient (pSClient) {
    m_strMacID = cMacID;

    m_boIsAlived = FALSE;
    m_boIsConnected = FALSE;

    m_byAuthenRequestCount = 0;
    m_iKeepAliveTimerHandle = -1;
    m_iAuthenRequestTimerHandle = -1;
    m_iConnectionTimerHandle = -1;

    m_pNetCtrllerTimer = RTimer::getTimerInstance();
    m_pNetCtrllerThread = new LThread();
    m_pNetCtrllerLocker = new Locker();

    m_keepaliveTimerFunctor = makeFunctor(
            (timerFunctor_p) NULL, *this, &NetCtrller::HandleKeepAliveProcess);
    m_authenRequestTimerFunctor = makeFunctor(
            (timerFunctor_p) NULL, *this, &NetCtrller::HandleAuthenProcess);
    m_checkConectionTimerFunctor = makeFunctor(
            (timerFunctor_p) NULL, *this, &NetCtrller::HandleConnectionProcess);

    m_NetCtrllerThreadFunctor = makeFunctor(
            (threadFunctor_p) NULL, *this, &NetCtrller::NetCtrlllerThreadProc);
    m_pNetCtrllerThread->RegThreadFunctor(&m_NetCtrllerThreadFunctor);

    m_pJsonNetSession = JsonNetSession::CreateSession();
    RegisterJsonMessageInform();
}

/**
 * @func   
 * @brief  None
 * @param  None
 * @retval None
 */
NetCtrller::~NetCtrller() {
    if (m_pNetCtrllerThread != NULL) {
        delete m_pNetCtrllerThread;
        m_pNetCtrllerThread = NULL;
    }

    if (m_pNetCtrllerLocker != NULL) {
        delete m_pNetCtrllerLocker;
        m_pNetCtrllerLocker = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
NetCtrller::NetCtrlllerThreadProc(
    void_p pBuffer
) {
    while (TRUE) {
        JsonCommand_p pJsonCommand = NULL;

        m_pNetCtrllerLocker->Lock();
        if (!m_queNetCtrllerJsonCommand.empty()) {
            pJsonCommand = m_queNetCtrllerJsonCommand.front();
            m_queNetCtrllerJsonCommand.pop();
        }
        m_pNetCtrllerLocker->UnLock();

        if (pJsonCommand != NULL) { ProcessHandler(pJsonCommand); }
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::Process() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
NetCtrller::ICtrllerRecvFunctor(
    ICtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pICtrllerFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::InitProcess() {
    m_iConnectionTimerHandle = m_pNetCtrllerTimer->StartTimer(
                RTimer::Repeat::Forever, 2, &m_checkConectionTimerFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::Start() {
    if (m_pNetCtrllerThread != NULL) {
        m_pNetCtrllerThread->Start();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::RegisterJsonMessageInform() {
    m_valueJsonChecker.Register(JsonAuthRes::GetStrCmd());
    m_valueJsonChecker.Register(JsonKaliveRes::GetStrCmd());
    m_valueJsonChecker.Register(JsonZwLstAdd::GetStrCmd(), JsonCommand::Flag::Zwave);
    m_valueJsonChecker.Register(JsonZwLstDel::GetStrCmd(), JsonCommand::Flag::Zwave);
    m_valueJsonChecker.Register(JsonZbLstAdd::GetStrCmd(), JsonCommand::Flag::Zigbee);
    m_valueJsonChecker.Register(JsonZbLstDel::GetStrCmd(), JsonCommand::Flag::Zigbee);
    m_valueJsonChecker.Register(JsonDevRep::GetStrCmd(), JsonCommand::Flag::Database);
    m_valueJsonChecker.Register(JsonZbStt::GetStrCmd(), JsonCommand::Flag::Zigbee);
    m_valueJsonChecker.Register(JsonZbResetRes::GetStrCmd(), JsonCommand::Flag::Zigbee);

    m_pJsonNetSession->MapJsonMessage<JsonAuthRes>(JsonAuthRes::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonAuthReq>(JsonAuthReq::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonKaliveRes>(JsonKaliveRes::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonKaliveReq>(JsonKaliveReq::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZwLstAdd>(JsonZwLstAdd::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZwLstDel>(JsonZwLstDel::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZwLstDel>(JsonZwLstDel::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZbLstAdd>(JsonZbLstAdd::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZbLstDel>(JsonZbLstDel::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZbResetRes>(JsonZbResetRes::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonDevRep>(JsonDevRep::GetStrCmd());
    m_pJsonNetSession->MapJsonMessage<JsonZbStt>(JsonZbStt::GetStrCmd());

    RegisterHandler(JsonAuthRes::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdAuthRes));
    RegisterHandler(JsonKaliveRes::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdKaliveRes));
    RegisterHandler(JsonZwLstAdd::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonZwLstDel::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonZbLstAdd::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonZbLstDel::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonDevRep::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonZbStt::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));
    RegisterHandler(JsonZbResetRes::GetStrCmd(),
            makeFunctor((HandlerNetCmdFunctor_p) NULL, *this, &NetCtrller::HandlerNetCmdCommon));

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::RegisterHandler(
    String strJsonCommand,
    HandlerNetCmdFunctor_t funcTor
) {
    m_mapHandleFunc[strJsonCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::ProcessHandler(
    JsonCommand_p pJsonCommand
) {
    String strJsonCommandName = pJsonCommand->GetFullCommand();
    MapHandleFunc::const_iterator_t it = m_mapHandleFunc.find(strJsonCommandName);
    if (it != m_mapHandleFunc.end()) {
        m_mapHandleFunc[strJsonCommandName](pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
NetCtrller::LoadCmdClass(
    JsonCommand_p pJsonCommand
) {
    bool_t boRetVal = FALSE;
    if (pJsonCommand->GetDesFlag() && JsonCommand::Flag::NetWork != 0) {
//        if (m_valueJsonChecker.HasCommand(*pJsonCommand)) {
            m_pNetCtrllerLocker->Lock();
            m_queNetCtrllerJsonCommand.push(pJsonCommand);
            m_pNetCtrllerLocker->UnLock();
            boRetVal = TRUE;
//        }
    }
    return boRetVal;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandlerNetCmdAuthRes(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonAuthRes> jsonAuthRes = m_pJsonNetSession->GetJsonMapping<JsonAuthRes>();
    jsonAuthRes->ParseJsonCommand(pJsonCommand);

    if (jsonAuthRes->Ret() == 0) {
        m_pNetCtrllerLocker->Lock();
        DEBUG1("authen success");
        m_boAuthenticated = TRUE;
        m_pNetCtrllerLocker->UnLock();
    } else if (jsonAuthRes->Ret() == 1) {
        DEBUG1("authen not active");
    } else if (jsonAuthRes->Ret() == 2) {
        DEBUG1("authen expired");
    } else {
        DEBUG2("authen error");
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::ProcessAuthenReq() {
    DEBUG1("authen request");
    m_byAuthenRequestCount++;

    JsonMessagePtr<JsonAuthReq> jsonAuthReq = m_pJsonNetSession->GetJsonMapping<JsonAuthReq>();
    JsonCommand_p pJsonCommand = jsonAuthReq->CreateJsonCommand(m_strMacID);
    if (m_pSessionClient != NULL) {
        m_pSessionClient->JsCommandToPacket(pJsonCommand);
    } else {
        delete pJsonCommand;
        pJsonCommand = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::ProcessKaliveReq() {
    DEBUG1("keepalive request");

    JsonMessagePtr<JsonKaliveReq> jsonKaliveReq = m_pJsonNetSession->GetJsonMapping<JsonKaliveReq>();
    JsonCommand_p pJsonCommand = jsonKaliveReq->CreateJsonCommand();

    if ((m_pSessionClient != NULL) && m_boAuthenticated) {
        m_pSessionClient->JsCommandToPacket(pJsonCommand);
    } else {
        delete pJsonCommand;
        pJsonCommand = NULL;
    }
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandlerNetCmdKaliveRes(
    JsonCommand_p pJsonCommand
) {
    m_pNetCtrllerLocker->Lock();
    m_boIsAlived = TRUE;
    DEBUG1("keepalive success");
    m_pNetCtrllerLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandlerNetCmdCommon(
    JsonCommand_p pJsonCommand
) {
    if (m_pSessionClient != NULL) {
        m_pSessionClient->JsCommandToPacket(pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandleConnectionProcess(
    void_p pBuffer
) {
    if ((!m_boIsConnected) && (m_pSessionClient != NULL)) {
        if (m_pSessionClient->Connect()) {
            m_pNetCtrllerLocker->Lock();
            DEBUG1("connect success");
            m_boIsConnected = TRUE;
            m_pNetCtrllerLocker->UnLock();

            if (m_pSessionClient->Start()) {
                DEBUG1("new thread");
            } else {
                DEBUG1("thread fail");
            }

            m_pNetCtrllerTimer->CancelTimer(m_iConnectionTimerHandle);
            m_iConnectionTimerHandle = -1;
            DEBUG1("cancel connection handle");

            m_pNetCtrllerLocker->Lock();
            m_boAuthenticated = FALSE;
            m_pNetCtrllerLocker->UnLock();

            ProcessAuthenReq();

            if (m_iAuthenRequestTimerHandle == -1) {
                m_iAuthenRequestTimerHandle = m_pNetCtrllerTimer->StartTimer(
                        RTimer::Repeat::Forever, AuthenTimeout, &m_authenRequestTimerFunctor, NULL);
            }
        } else {
            DEBUG1("connect fail");
        }
    } else {
        DEBUG1("init connection error");
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandleAuthenProcess(
    void_p pBuffer
) {
    m_pNetCtrllerLocker->Lock();
    if (m_boAuthenticated) {
        m_boIsAlived = FALSE;
        m_pNetCtrllerLocker->UnLock();

        m_pNetCtrllerTimer->CancelTimer(m_iAuthenRequestTimerHandle);
        m_iAuthenRequestTimerHandle = -1;

        ProcessKaliveReq();

        m_iKeepAliveTimerHandle = m_pNetCtrllerTimer->StartTimer(
                RTimer::Repeat::Forever, KaliveTimeout, &m_keepaliveTimerFunctor, NULL);

        return;
    } else {
        m_pNetCtrllerLocker->UnLock();

        ProcessAuthenReq();

        if (m_byAuthenRequestCount >= 99) {
            if (m_iAuthenRequestTimerHandle != -1) { // Stop Authen
                m_pNetCtrllerTimer->CancelTimer(m_iAuthenRequestTimerHandle);
                m_iAuthenRequestTimerHandle = -1;
            }
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
NetCtrller::HandleKeepAliveProcess(
    void_p pBuffer
) {
    m_pNetCtrllerLocker->Lock();
    if (m_boIsAlived) {
        DEBUG1("keep alive");
        m_boIsAlived = FALSE; // reset avlie state
        m_boIsConnected = TRUE;
        m_pNetCtrllerLocker->UnLock();

        ProcessKaliveReq(); // resend keepalive
    } else {
        DEBUG1("lose connect");
        if (m_pSessionClient->Close()) {
            DEBUG1("close success");
        } else {
            DEBUG1("close fail");
        }

        m_boIsConnected = FALSE;
        m_boAuthenticated = FALSE;
        m_pNetCtrllerLocker->UnLock();

        m_pNetCtrllerTimer->CancelTimer(m_iKeepAliveTimerHandle);
        m_iKeepAliveTimerHandle = -1;

        if (m_iConnectionTimerHandle == -1) {
            m_iConnectionTimerHandle = m_pNetCtrllerTimer->StartTimer(
                    RTimer::Repeat::Forever, ConnectTimeout, &m_checkConectionTimerFunctor, NULL);
        }
    }
}
