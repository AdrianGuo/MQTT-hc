/*
 * ZbCtrller.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <stddef.h>
#include "debug.hpp"
#include "LogPlus.hpp"
#include "LThread.hpp"
#include "JsonCommand.hpp"
#include "zcl_lumi.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevGet.hpp"
#include "JsonDevInfo.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevRestart.hpp"

#include "ZbCtrller.hpp"

/**
 * @func   ZbCtrller
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZbCtrller::ZbCtrller(
    const_char_p chPortname
) {
    m_pZbDriver = ZbDriver::GetInstance(chPortname);

    m_pZbCtrllerThread = new LThread();
    m_pZbCtrllerLocker = new Locker();
    m_ZbCtrllerThreadFunctor = makeFunctor(
            (threadFunctor_p) NULL, *this, &ZbCtrller::ZbCtrlllerThreadProc);
    m_pZbCtrllerThread->RegThreadFunctor(&m_ZbCtrllerThreadFunctor);

    m_evWaitMsgSignal.Reset();

    m_ZbCtrllerFunctor = makeFunctor(
            (ZbCtrllerFunctor_p) NULL, *this, &ZbCtrller::PushJsonCommand);

    m_pJsonRecvSession = JsonRecvZigbeeSession::CreateSession();
    CtrllerSendFunctor();

    RegisterZbSession();
    RegisterHandler();
    RegisterProcess();
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
 * @func   RegisterZwSession
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterZbSession() {
    m_pJsonRecvSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevGet>(JsonDevGet::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevInfo>(JsonDevInfo::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevReset>(JsonDevReset::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevRestart>(JsonDevRestart::GetStrCmd());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterHandler() {
    RegisterHandler(JsonDevAdd::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdAdd));
    RegisterHandler(JsonDevDel::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdDel));
    RegisterHandler(JsonDevSet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdSet));
    RegisterHandler(JsonDevGet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdGet));
    RegisterHandler(JsonDevInfo::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdInfo));
    RegisterHandler(JsonDevReset::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdReset));
    RegisterHandler(JsonDevRestart::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdRestart));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterProcess() {
    RegisterProcess(ZbMessage::Command::AddDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdAdd));
    RegisterProcess(ZbMessage::Command::RemoveDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdDel));
    RegisterProcess(ZbMessage::Command::SetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdSet));
    RegisterProcess(ZbMessage::Command::GetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdGet));
    RegisterProcess(ZbMessage::Command::InfoReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdInfo));
    RegisterProcess(ZbMessage::Command::ResetReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdReset));
    RegisterProcess(ZbMessage::Command::RestartReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdRestart));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterHandler(
    String strJsonCommand,
    HandlerZbCmdFunctor_t funcTor
) {
    m_mapHandlerFunctor[strJsonCommand] = funcTor;
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
    String strJsonCommandName = pJsonCommand->GetFullCommand();
    MapHandlerFunctor::const_iterator_t it = m_mapHandlerFunctor.find(strJsonCommandName);
    if (it != m_mapHandlerFunctor.end()) {
        m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterProcess(
    ZbMessage::Command ZbCommand,
    ProcZbCmdFunctor_t funcTor
) {
    m_mapProcFunctor[ZbCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcessProcess(
    ZbMessage_p pZbMessage
) {
    ZbMessage::Command ZbCommand = pZbMessage->GetZbCommad();
    MapProcFunctor::const_iterator_t it = m_mapProcFunctor.find(ZbCommand);
    if (it != m_mapProcFunctor.end()) {
        m_mapProcFunctor[ZbCommand](pZbMessage);
    }
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
        ZbMessage_p pZbMessage = NULL;
        m_pZbCtrllerLocker->Lock();
        if (!m_queSendZbMsg.empty()) {
            pZbMessage = m_queSendZbMsg.front();
            m_queSendZbMsg.pop();
        }
        m_pZbCtrllerLocker->UnLock();

        if (pZbMessage != NULL)
            ProcessProcess(pZbMessage);

        pZbMessage = NULL;
        delete pZbMessage;
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
ZbCtrller::HandlerCmdAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonDevAdd>* jsonDevAdd = m_pJsonRecvSession->GetJsonMapping<JsonDevAdd>();
    JsonDevAdd_p pJsonDevAdd = jsonDevAdd->Object();
    pJsonDevAdd->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonDevAdd,
            ZbMessage::Command::AddDevice);

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonDevDel>* jsonDevDel = m_pJsonRecvSession->GetJsonMapping<JsonDevDel>();
    JsonDevDel_p pJsonDevDel = jsonDevDel->Object();
    pJsonDevDel->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonDevDel,
            ZbMessage::Command::RemoveDevice);

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdSet(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonDevSet>* jsonDevSet = m_pJsonRecvSession->GetJsonMapping<JsonDevSet>();
    JsonDevSet_p pJsonDevSet = jsonDevSet->Object();
    pJsonDevSet->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonDevSet,
            ZbMessage::Command::SetDevice);
    pZbMessage->SetClientId(pJsonCommand->GetClientId());

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdGet(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonDevGet>* jsonDevGet = m_pJsonRecvSession->GetJsonMapping<JsonDevGet>();
    JsonDevGet_p pJsonDevGet = jsonDevGet->Object();
    pJsonDevGet->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonDevGet,
            ZbMessage::Command::GetDevice);
    pZbMessage->SetClientId(pJsonCommand->GetClientId());

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdInfo(
    JsonCommand_p pJsonCommand
) {
    ZbMessage_p pZbMessage = new ZbMessage(NULL,
            ZbMessage::Command::InfoReq);

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdReset(
    JsonCommand_p pJsonCommand
) {
    ZbMessage_p pZbMessage = new ZbMessage(NULL,
            ZbMessage::Command::ResetReq);

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::HandlerCmdRestart(
    JsonCommand_p pJsonCommand
) {
    ZbMessage_p pZbMessage = new ZbMessage(NULL,
            ZbMessage::Command::ResetReq);

    m_pZbCtrllerLocker->Lock();
    m_queSendZbMsg.push(pZbMessage);
    m_pZbCtrllerLocker->UnLock();

    delete pJsonCommand;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdAdd(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }

    if (pZbMessage->GetZbCommad() == ZbMessage::Command::AddDevice) {
        JsonDevAdd_p pJsonDevAdd = (JsonDevAdd_p) pZbMessage->GetJsonMessageObject();

        u8_t byTime = 0x00;
        if(pJsonDevAdd->Act() == 0) {
            byTime = 0xFE;
        } else if(pJsonDevAdd->Act() == 1) {
            byTime = 0x00;
        }

        pZbMessage->Push(byTime);
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdDel(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }

    if (pZbMessage->GetZbCommad() == ZbMessage::Command::RemoveDevice) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdSet(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::SetDevice) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdGet(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::GetDevice) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdInfo(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::InfoReq) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdReset(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::ResetReq) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ProcCmdRestart(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::RestartReq) {
        m_pZbDriver->ProcSendMessage(pZbMessage);
    }
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
