/*
 * ZbCtrller.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */
#include <JsonIrDel.hpp>
#include <stddef.h>
#include "debug.hpp"
#include "LThread.hpp"

#include "JsonCommand.hpp"
#include "ZbSerialAPI.hpp"

#include "zcl_lumi.hpp"
#include "JsonZbAdd.hpp"
#include "JsonZbDel.hpp"
#include "JsonZbSet.hpp"
#include "JsonZbGet.hpp"
#include "JsonIrLearn.hpp"
#include "JsonIrSet.hpp"
#include "JsonIrDel.hpp"
#include "JsonIrEna.hpp"
#include "JsonZbInfo.hpp"
#include "JsonZbReset.hpp"
#include "JsonZbRestart.hpp"
#include "JsonManualRemove.hpp"

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
    m_valueJsonChecker.Register(JsonZbSet::GetStrCmd());
    m_valueJsonChecker.Register(JsonZbGet::GetStrCmd());
    m_valueJsonChecker.Register(JsonZbInfo::GetStrCmd());
    m_valueJsonChecker.Register(JsonZbReset::GetStrCmd());
    m_valueJsonChecker.Register(JsonZbRestart::GetStrCmd());
    m_valueJsonChecker.Register(JsonIrLearn::GetStrCmd());
    m_valueJsonChecker.Register(JsonIrSet::GetStrCmd());
    m_valueJsonChecker.Register(JsonIrDel::GetStrCmd());
    m_valueJsonChecker.Register(JsonIrEna::GetStrCmd());
    m_valueJsonChecker.Register(JsonManualRemove::GetStrCmd());

    m_pJsonZigbeeSession->MapJsonMessage<JsonZbAdd>(JsonZbAdd::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbDel>(JsonZbDel::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbSet>(JsonZbSet::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbGet>(JsonZbGet::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbInfo>(JsonZbInfo::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbReset>(JsonZbReset::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonZbRestart>(JsonZbRestart::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonIrLearn>(JsonIrLearn::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonIrSet>(JsonIrSet::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonIrDel>(JsonIrDel::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonIrEna>(JsonIrEna::GetStrCmd());
    m_pJsonZigbeeSession->MapJsonMessage<JsonManualRemove>(JsonManualRemove::GetStrCmd());


    RegisterHandler(JsonZbAdd::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdAdd));
    RegisterHandler(JsonZbDel::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdDel));
    RegisterHandler(JsonZbSet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdSet));
    RegisterHandler(JsonZbGet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdGet));
    RegisterHandler(JsonZbInfo::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdInfo));
    RegisterHandler(JsonZbReset::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdReset));
    RegisterHandler(JsonZbRestart::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdRestart));
    RegisterHandler(JsonIrLearn::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdIrLearn));
    RegisterHandler(JsonIrSet::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdIrSet));
    RegisterHandler(JsonIrDel::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdIrDel));
    RegisterHandler(JsonIrEna::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdIrEna));
    RegisterHandler(JsonManualRemove::GetStrCmd(), makeFunctor((HandlerZbCmdFunctor_p) NULL, *this, &ZbCtrller::HandlerCmdIrEna));

    RegisterProcess(ZbMessage::Command::AddDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdAdd));
    RegisterProcess(ZbMessage::Command::RmvDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdDel));
    RegisterProcess(ZbMessage::Command::SetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdSet));
    RegisterProcess(ZbMessage::Command::GetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdGet));
    RegisterProcess(ZbMessage::Command::InfoReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdInfo));
    RegisterProcess(ZbMessage::Command::ResetReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdReset));
    RegisterProcess(ZbMessage::Command::RestartReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdRestart));
    RegisterProcess(ZbMessage::Command::IrLearn, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdIrLearn));
    RegisterProcess(ZbMessage::Command::IrSet, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdIrSet));
    RegisterProcess(ZbMessage::Command::IrDel, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdIrDel));
    RegisterProcess(ZbMessage::Command::IrEna, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdIrEna));
    RegisterProcess(ZbMessage::Command::ManualRmv, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbCtrller::ProcCmdManualRemove));
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
    EvAction evAction,
    void_p pInBuffer
) {
//    DEBUG1("3. PushJsonCommand");
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
ZbCtrller::HandlerCmdDel(
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
ZbCtrller::HandlerCmdSet(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonZbSet>* jsonZbSet = m_pJsonZigbeeSession->GetJsonMapping<JsonZbSet>();
    JsonZbSet_p pJsonZbSet = jsonZbSet->Object();
    pJsonZbSet->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonZbSet,
            ZbMessage::Command::SetDevice);

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
    JsonMessageMap<JsonZbGet>* jsonZbGet = m_pJsonZigbeeSession->GetJsonMapping<JsonZbGet>();
    JsonZbGet_p pJsonZbGet = jsonZbGet->Object();
    pJsonZbGet->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonZbGet,
            ZbMessage::Command::GetDevice);

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
ZbCtrller::HandlerCmdIrLearn(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonIrLearn>* jsonIrLearn = m_pJsonZigbeeSession->GetJsonMapping<JsonIrLearn>();
    JsonIrLearn_p pJsonIrLearn = jsonIrLearn->Object();
    pJsonIrLearn->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonIrLearn,
            ZbMessage::Command::IrLearn);

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
ZbCtrller::HandlerCmdIrSet(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonIrSet>* jsonIrSet = m_pJsonZigbeeSession->GetJsonMapping<JsonIrSet>();
    JsonIrSet_p pJsonIrSet = jsonIrSet->Object();
    pJsonIrSet->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonIrSet,
            ZbMessage::Command::IrSet);

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
ZbCtrller::HandlerCmdIrDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonIrDel>* jsonIrDel = m_pJsonZigbeeSession->GetJsonMapping<JsonIrDel>();
    JsonIrDel_p pJsonIrDel = jsonIrDel->Object();
    pJsonIrDel->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonIrDel,
            ZbMessage::Command::IrSet);

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
ZbCtrller::HandlerCmdIrEna(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonIrEna>* jsonIrEna = m_pJsonZigbeeSession->GetJsonMapping<JsonIrEna>();
    JsonIrEna_p pJsonIrEna = jsonIrEna->Object();
    pJsonIrEna->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonIrEna,
            ZbMessage::Command::IrSet);

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
ZbCtrller::HandlerCmdManualRemove(
    JsonCommand_p pJsonCommand
) {
    JsonMessageMap<JsonManualRemove>* jsonManualRemove = m_pJsonZigbeeSession->GetJsonMapping<JsonManualRemove>();
    JsonManualRemove_p pJsonManualRemove = jsonManualRemove->Object();
    pJsonManualRemove->ParseJsonCommand(pJsonCommand);

    ZbMessage_p pZbMessage = new ZbMessage(pJsonManualRemove,
            ZbMessage::Command::ManualRmv);

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
        JsonZbAdd_p pJsonZbAdd = (JsonZbAdd_p) pZbMessage->GetJsonMessageObject();

        u8_t byTime = 0x00;
        if(pJsonZbAdd->Return() == 0) {
            byTime = 0xFE;
        } else if(pJsonZbAdd->Return() == 1) {
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

    if (pZbMessage->GetZbCommad() == ZbMessage::Command::RmvDevice) {
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
ZbCtrller::ProcCmdIrLearn(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::IrLearn) {
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
ZbCtrller::ProcCmdIrSet(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::IrSet) {
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
ZbCtrller::ProcCmdIrDel(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::IrDel) {
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
ZbCtrller::ProcCmdIrEna(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::IrEna) {
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
ZbCtrller::ProcCmdManualRemove(
    ZbMessage_p pZbMessage
) {
    if (pZbMessage == NULL) { return; }
    if (pZbMessage->GetZbCommad() == ZbMessage::Command::ManualRmv) {
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
