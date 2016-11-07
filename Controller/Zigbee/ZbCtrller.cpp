/*
 * ZbCtrller.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */
#include <stddef.h>
#include "debug.hpp"
#include "LThread.hpp"

#include "JsonCommand.hpp"
#include "ZbSerialAPI.hpp"

#include "JsonZbAdd.hpp"
#include "JsonZbDel.hpp"
#include "JsonDevSet.hpp"

#include "ZbCtrller.hpp"

/**
 * @func   ZbCtrller
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZbCtrller::ZbCtrller(
    ZbDriver_p pZbDriver
) : m_valueJsonChecker() {
    m_pZbDriver = pZbDriver;

    m_pZbCtrllerThread = new LThread();
    m_pZbCtrllerLocker = new Locker();
    m_ZbCtrllerThreadFunctor = makeFunctor(
            (threadFunctor_p) NULL, *this, &ZbCtrller::ZbCtrlllerThreadProc);
    m_pZbCtrllerThread->RegThreadFunctor(&m_ZbCtrllerThreadFunctor);

    m_evWaitMsgSignal.Reset();

    m_ZbCtrllerFunctor = makeFunctor(
            (ZbCtrllerFunctor_p) NULL, *this, &ZbCtrller::PushJsonCommand);

    m_pJsonZigbeeSession = JsonSendZigbeeSession::CreateSession();
    ZbCtrllerSendFunctor();

    RegisterJsonMessageInform();
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
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::ZbCtrllerSendFunctor() {
    if (m_pZbDriver != NULL) {
        m_pZbDriver->ZbDriverRecvFunctor(&m_ZbCtrllerFunctor);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbCtrller::ICtrllerRecvFunctor(
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
ZbCtrller::Process() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::Start() {
    if (m_pZbDriver != NULL) {
        if (m_pZbDriver->Start()) {
            DEBUG1("START SUCCESS");
        } else {
            DEBUG1("START FAIL");
        }
    }

    if (m_pZbCtrllerThread != NULL) {
        m_pZbCtrllerThread->Start();
    }
}

/**
 * @func   RegisterInform
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbCtrller::RegisterJsonMessageInform() {
    m_valueJsonChecker.Register(JsonZbAdd::GetStrCmd());
    m_valueJsonChecker.Register(JsonZbDel::GetStrCmd());
    m_valueJsonChecker.Register(JsonDevSet::GetStrCmd());

    m_pJsonZigbeeSession->MapJsonMessage<JsonZbAdd>(JsonZbAdd::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbDel>(JsonZbDel::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd());


    RegisterHandler(JsonZbAdd::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerZbCmdAdd));
    RegisterHandler(JsonZbDel::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerZbCmdDel));
//    RegisterHandler(JsonDevSet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdDevSet));

    RegisterProc(ZbMessage::Command::AddDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcZbCmdSendAdd));
    RegisterProc(ZbMessage::Command::RmvDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcZbCmdSendDel));
//    RegisterProc(ZbMessage::Command::SetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdSendDevSet));
}

/**
 * @func   IsZigbeeCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbCtrller::LoadCmdClass(
    JsonCommand_p pJsonCommand
) {
    bool_t boRetVal = FALSE;
    if (pJsonCommand->GetDesFlag() && JsonCommand::Flag::Zigbee != 0) {
        if (m_valueJsonChecker.HasCommand(*pJsonCommand)) {
            ProcessHandler(pJsonCommand);
            boRetVal = TRUE;
        }
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
ZbCtrller::RegisterProc(
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
ZbCtrller::ProcessProc(
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
    EvAction evAction,
    void_p pInBuffer
) {
    if (evAction == Set1) {
        m_evWaitMsgSignal.Set();
    } else if (evAction == Reset) {
        m_evWaitMsgSignal.Reset();
    } else if (evAction == Pushback) {
        ZbMessage_p pZbMsg = (ZbMessage_p) pInBuffer;
        m_pZbCtrllerLocker->Lock();
        if (pZbMsg != NULL) { m_queSendZbMsg.push(pZbMsg); }
        m_pZbCtrllerLocker->UnLock();

        return;
    }

    if ((m_pICtrllerFunctor != NULL) && (pInBuffer != NULL)) {
        m_pICtrllerFunctor->operator ()((JsonCommand_p) pInBuffer);
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
            ProcessProc(pZbMessage);

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
ZbCtrller::HandlerZbCmdAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonZbAdd>* jsonZbAdd = m_pJsonZigbeeSession->GetJsonMapping<JsonZbAdd>();
    JsonZbAdd_p pJsonZbAdd = jsonZbAdd->Object();
    pJsonZbAdd->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonZbAdd,
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
ZbCtrller::HandlerZbCmdDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonZbDel>* jsonZbDel = m_pJsonZigbeeSession->GetJsonMapping<JsonZbDel>();
    JsonZbDel_p pJsonZbDel = jsonZbDel->Object();
    pJsonZbDel->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonZbDel,
            ZbMessage::Command::RmvDevice);

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
ZbCtrller::ProcZbCmdSendAdd(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }

    if (pZbMessage->GetZbCommad() == ZbMessage::Command::AddDevice) {
        JsonZbAdd_p pJsonZbAdd = (JsonZbAdd_p) pZbMessage->GetJsonMessageObject();

        u8_t byTime = 0x00;
        if(pJsonZbAdd->Act() == 0) {
            byTime = 0xFE;
        } else if(pJsonZbAdd->Act() == 1) {
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
ZbCtrller::ProcZbCmdSendDel(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }

    if (pZbMessage->GetZbCommad() == ZbMessage::Command::RmvDevice) {
        JsonZbAdd_p pJsonZbDel = (JsonZbAdd_p) pZbMessage->GetJsonMessageObject();


//        if(pJsonZbDel->Act() == 0) {
//
//        } else if(pJsonZbDel->Act() == 1) {
//
//        }

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
ZbCtrller::IniZbCtrller() {
    if (m_pZbDriver != NULL) { m_pZbDriver->InitDriver(); }
}
