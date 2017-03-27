/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:      Include file for application
 *
 * Author:           TrungTQ
 *
 * Last Changed By:  TrungTQ
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
#include <stddef.h>
#include <iostream>
#include <string>
#include "Typedefs.h"
#include "LogPlus.hpp"
#include "LogCommand.hpp"
#include "HelperHc.hpp"
#include "LThread.hpp"

#include "JsonCommand.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwSerialAPI.hpp"

#include "JsonDevGet.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevStt.hpp"
#include "JsonDevOff.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevRestart.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevResetRes.hpp"
#include "JsonDevInfo.hpp"
#include "JsonDevInfoRes.hpp"
#include "JsonDevLst.hpp"
#include "JsonDevLstRes.hpp"
#include "JsonDevLstAdd.hpp"
#include "JsonDevLstAddRes.hpp"
#include "JsonDevLstDel.hpp"
#include "JsonDevLstDelRes.hpp"
#include "JsonDevSync.hpp"
#include "JsonDevSyncRes.hpp"

#include "MultiChannelCmdClass.hpp"
#include "ZwDeviceValueMan.hpp"
#include "ZwNode.hpp"
#include "ZwDriver.hpp"
#include "ZwCtrller.hpp"

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

ZwCtrller::ZwCtrller(
    const_char_p portname
) : m_ZwDriver (portname) ,
    m_boAddNodeSend (FALSE),
    m_boDelNodeCmdSend (FALSE),
    m_boAddNodeRecv (FALSE),
    m_boDelNodeResReceived (FALSE),
    m_pJsonRecvZwaveSession (JsonRecvZwaveSession::CreateSession()),
    m_pJsonSendZwaveSession (JsonSendZwaveSession::CreateSession()),
    m_pJsonSelfZwaveSession (JsonSelfZwaveSession::CreateSession()) {

    m_pZwDeviceValueMan = ZwDeviceValueMan::GetInstance();
    m_pZwDeviceValueMan->Register();
    ZwNode::MapValueToCmdClass();

    m_pZwCtrllerThread = new LThread();
    m_pZwCtrllerLocker = new Locker();

    m_ZwCtrllerThreadFunctor =
    makeFunctor((threadFunctor_p) NULL, *this, &ZwCtrller::ZwCtrlllerThreadProc);
    m_pZwCtrllerThread->RegThreadFunctor(&m_ZwCtrllerThreadFunctor);

    m_evWaitMsgSignal.Reset();

    m_ZwCtrllerFunctor =
    makeFunctor((ZwCtrllerFunctor_p) NULL, *this, &ZwCtrller::PushJsonCommand);

    RegisterZwSession();
    RegisterHandler();
    RegisterProcess();
    CtrllerSendFunctor();
}

/**
 * @func   ~ZwCtrller
 * @brief  Destructor
 * @param  None
 * @retval None
 */
ZwCtrller::~ZwCtrller() {
    if (m_pZwCtrllerThread != NULL) {
        delete m_pZwCtrllerThread;
        m_pZwCtrllerThread = NULL;
    }

    if (m_pZwCtrllerLocker != NULL) {
        delete m_pZwCtrllerLocker;
        m_pZwCtrllerLocker = NULL;
    }
}

/**
 * @func   CtrllerSendFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::CtrllerSendFunctor() {
    m_ZwDriver.ZwDriverRecvFunctor(&m_ZwCtrllerFunctor);
}

/**
 * @func   CtrllerRecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::CtrllerRecvFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pCtrllerFunctor = pRecvFunctor;
    }
}

/**
 * @func   Process
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Process() {
}

/**
 * @func   Debug
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Debug() {
    m_ZwDriver.Debug();
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Start() {
    if (m_ZwDriver.Start()) {
        LOG_INFO("Zwave start success");
    } else {
        LOG_ERROR("Zwave start fail");
    }

    if (m_pZwCtrllerThread != NULL) {
        m_pZwCtrllerThread->Start();
    }
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Connect() {
    if (m_ZwDriver.Connect()) {
        LOG_INFO("Zwave connect success");
    } else {
        LOG_ERROR("Zwave connect fail");
    }
}

/**
 * @func   RegisterZwSession
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterZwSession() {
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevGet>(JsonDevGet::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevReset>(JsonDevReset::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonDevRestart>(JsonDevRestart::GetStrCmd());

    m_pJsonSendZwaveSession->MapJsonMessage<JsonDevStt>(JsonDevStt::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonDevLstAdd>(JsonDevLstAdd::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonDevLstDel>(JsonDevLstDel::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonDevResetRes>(JsonDevResetRes::GetStrCmd());
//    m_pJsonSendZwaveSession->MapJsonMessage<JsonDevRestartRes>(JsonDevRestartRes::GetStrCmd());

    m_pJsonSelfZwaveSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd());
    m_pJsonSelfZwaveSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd());
}

/**
 * @func   RegisterHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterHandler() {
    RegisterHandler(JsonDevAdd::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdAdd));
    RegisterHandler(JsonDevDel::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdDel));
    RegisterHandler(JsonDevReset::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdRst));
    RegisterHandler(JsonDevRestart::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdRsa));
    RegisterHandler(JsonDevGet::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdGet));
    RegisterHandler(JsonDevSet::GetStrCmd(),
    makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwaveCmdSet));
}

/**
 * @func   RegisterProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterProcess() {
    RegisterProc(ZwMessage::Command::SendData,
    makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdSendData));
    RegisterProc(ZwMessage::Command::AddDevice,
    makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwaveCmdAddNode));
    RegisterProc(ZwMessage::Command::StopAdd,
    makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwaveCmdAddNode));
    RegisterProc(ZwMessage::Command::RmvDevice,
    makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwaveCmdDelNode));
    RegisterProc(ZwMessage::Command::StopRmv,
    makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwaveCmdDelNode));
}

/**
 * @func   RegisterHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterHandler(
    String strJsonCommand,
    HandlerZwCmdFunctor_t funcTor
) {
    m_mapHandlerFunctor[strJsonCommand] = funcTor;
}

/**
 * @func   ProcessHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcessHandler(
    JsonCommand_p pJsonCommand
) {
    LOG_COMMAND(Log::Level::eDebug, pJsonCommand);
    String strJsonCommandName = pJsonCommand->GetFullCommand();

    MapHandlerFunctor::const_iterator it =
    m_mapHandlerFunctor.find(strJsonCommandName);
    if (it != m_mapHandlerFunctor.end()) {
        m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
    }
}

/**
 * @func   RegisterProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterProc(
    ZwMessage::Command zwCommand,
    ProcZwCmdFunctor_t funcTor
) {
    m_mapProcFunctor[zwCommand] = funcTor;
}

/**
 * @func   ProcessProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcessProc(
    ZwMessage_p pZwMessage
) {
    if (pZwMessage == NULL) { return; }

    ZwMessage::Command zwCommand = pZwMessage->GetZwCommad();
    MapProcFunctor::const_iterator it = m_mapProcFunctor.find(zwCommand);
    if (it != m_mapProcFunctor.end()) {
        m_mapProcFunctor[zwCommand](pZwMessage);
    }
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::PushJsonCommand(
    EvAction evAction,
    void_p   pInBuffer
) {
    if (evAction == EvAction::SSet) {
        m_evWaitMsgSignal.Set();
    } else if (evAction == EvAction::Reset) {
        m_evWaitMsgSignal.Reset();
    } else if (evAction == EvAction::Pushback) {
        ZwMessage_p pZwMessage = (ZwMessage_p) pInBuffer;
        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();
        return;
    }

    if ((m_pCtrllerFunctor != NULL) && (pInBuffer != NULL)) {
        JsonCommand_p pJsonCommand = (JsonCommand_p) pInBuffer;
        (*m_pCtrllerFunctor)(pJsonCommand);
    }
}

/**
 * @func   ZwCtrlllerThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
ZwCtrller::ZwCtrlllerThreadProc(
    void_p pInBuffer
) {
    while (TRUE) {
        ZwMessage_p pZwMessage = NULL;
        m_pZwCtrllerLocker->Lock();
        if (!m_queSendZwMsg.empty()) {
            pZwMessage = m_queSendZwMsg.front();
            m_queSendZwMsg.pop();
        }
        m_pZwCtrllerLocker->UnLock();
        ProcessProc(pZwMessage);
        usleep(50000);
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * @func   HandlerZwaveCmdAdd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdAdd(
    JsonCommand_p pJsonCommand
) {
    m_pZwCtrllerLocker->Lock();
    if (!m_boDelNodeCmdSend) {
        m_pZwCtrllerLocker->UnLock();

        JsonMessagePtr<JsonDevAdd> jsonZwAdd =
        m_pJsonRecvZwaveSession->GetJsonMapping<JsonDevAdd>();

        if (!jsonZwAdd->ParseJsonCommand(pJsonCommand)) {
            delete pJsonCommand;
            pJsonCommand = NULL;
            return;
        }

        if (jsonZwAdd->Act() == 0) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boAddNodeSend) {
                m_pZwCtrllerLocker->UnLock();

                ZwMessage_p pZwMessage = 
                new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, TRUE,
                TRUE, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, 0, ZwMessage::Command::AddDevice);
                pZwMessage->ResetPacket(2);
                pZwMessage->Push(ADD_NODE_ANY);
                pZwMessage->Push(pZwMessage->GetNextCallbackId());
                m_evWaitMsgSignal.Reset();

                m_pZwCtrllerLocker->Lock();
                m_queSendZwMsg.push(pZwMessage);
            }
        } else if (jsonZwAdd->Act() == 1) {
            ZwMessage_p pZwMessage = 
            new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, TRUE,
            TRUE, FUNC_ID_ZW_ADD_NODE_TO_NETWORK, 0, ZwMessage::Command::StopAdd);
            pZwMessage->ResetPacket(2);
            pZwMessage->Push(ADD_NODE_STOP);
            pZwMessage->Push(pZwMessage->GetNextCallbackId());
            
            m_pZwCtrllerLocker->Lock();
            if (!m_boAddNodeRecv) {
                m_evWaitMsgSignal.Set();
            }
            m_queSendZwMsg.push(pZwMessage);
        }
    }
    m_pZwCtrllerLocker->UnLock();
    delete pJsonCommand;
    pJsonCommand = NULL;
}

/**
 * @func   HandlerZwaveCmdDel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdDel(
    JsonCommand_p pJsonCommand
) {
    m_pZwCtrllerLocker->Lock();
    if (!m_boAddNodeSend) {
        m_pZwCtrllerLocker->UnLock();

        JsonMessagePtr<JsonDevDel> jsonZwDel =
        m_pJsonRecvZwaveSession->GetJsonMapping<JsonDevDel>();

        if (!jsonZwDel->ParseJsonCommand(pJsonCommand)) {
            delete pJsonCommand;
            pJsonCommand = NULL;
            return;
        }

        if (jsonZwDel->Act() == 0) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boDelNodeCmdSend) {
                m_pZwCtrllerLocker->UnLock();

                ZwMessage_p pZwMessage = 
                new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK, TRUE,
                TRUE, FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK, 0, ZwMessage::Command::RmvDevice);
                pZwMessage->ResetPacket(2);
                pZwMessage->Push(REMOVE_NODE_ANY);
                pZwMessage->Push(pZwMessage->GetNextCallbackId());

                m_evWaitMsgSignal.Reset();

                m_pZwCtrllerLocker->Lock();
                m_queSendZwMsg.push(pZwMessage);
            }
        } else if (jsonZwDel->Act() == 1) {
            ZwMessage_p pZwMessage = 
            new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK, TRUE,
            TRUE, FUNC_ID_ZW_REMOVE_NODE_FROM_NETWORK, 0, ZwMessage::Command::StopRmv);
            pZwMessage->ResetPacket(2);
            pZwMessage->Push(REMOVE_NODE_STOP);
            pZwMessage->Push(pZwMessage->GetNextCallbackId());

            m_pZwCtrllerLocker->Lock();
            if (!m_boDelNodeResReceived) {
                m_pZwCtrllerLocker->UnLock();
                m_evWaitMsgSignal.Set();
                m_pZwCtrllerLocker->Lock();
            }
            m_queSendZwMsg.push(pZwMessage);
        }
    }
    m_pZwCtrllerLocker->UnLock();
    delete pJsonCommand;
    pJsonCommand = NULL;
}

/**
 * @func   HandlerZwaveCmdRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdRst(
    JsonCommand_p pJsonCommand
) {
    ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand);
    m_pZwCtrllerLocker->Lock();
    if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
    m_pZwCtrllerLocker->UnLock();
    delete pJsonCommand;
    pJsonCommand = NULL;
}

/**
 * @func   HandlerZwaveCmdRst
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdRsa(
    JsonCommand_p pJsonCommand
) {
    ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand);
    m_pZwCtrllerLocker->Lock();
    if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
    m_pZwCtrllerLocker->UnLock();
    delete pJsonCommand;
    pJsonCommand = NULL;
}

/**
 * @func   HandlerZwaveCmdSet
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdSet(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevSet> jsonZwSet =
    m_pJsonRecvZwaveSession->GetJsonMapping<JsonDevSet>();

    if (!jsonZwSet->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    Vector<JsonDevSet::Device_t> lstSetZwDevice = jsonZwSet->LstDev();

    for (u32_t i = 0; i < lstSetZwDevice.size(); i++) {
        u8_t byNodeId = (u8_t) lstSetZwDevice[i].devid;
        u8_t byOrder  = lstSetZwDevice[i].order;
        u8_t byType   = lstSetZwDevice[i].type;

        ValueDevice_p pValueDevice =
        m_pZwDeviceValueMan->CreateValueDevice(byType, lstSetZwDevice[i].value);

        ZwNode_p pZwaveRootNode = m_ZwDriver.GetZwaveNode(byNodeId);

        if ((pZwaveRootNode == NULL) || (pValueDevice == NULL)) { return; }

        ZwMessage_p pZwMessage =
        pZwaveRootNode->SetEnpointValue(byOrder, pValueDevice);

        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();

        delete pValueDevice;
        pValueDevice = NULL;
    }
}

/**
 * @func   HandlerZwaveCmdGet
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwaveCmdGet(
    JsonCommand_p pJsonCommand
) {
    Json::Value root = pJsonCommand->GetJsonOjbect();

    if (!root.isMember("dev")) { return; }

    Json::Value array = root["dev"];

    for (u32_t i = 0; i < root.size(); i++) {
        Json::Value dev = array[i];
        u8_t byDevId = std::stoi(dev["devid"].asString());
        ZwMessage_p pZwMessage = m_ZwDriver.GetStateZwNode(byDevId);

        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();
    }
}

/**
 * @func   ProcZwaveCmdAddNode
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwaveCmdAddNode(
    ZwMessage_p pZwMessage
) {
    if (pZwMessage == NULL) { return; }

    if (pZwMessage->GetBuffer() == NULL) { return; }

    if (pZwMessage->GetZwCommad() == ZwMessage::Command::AddDevice) {
        m_evWaitMsgSignal.Reset();
        m_pZwCtrllerLocker->Lock();
        m_boAddNodeSend = TRUE;
        m_boAddNodeRecv = FALSE;
        m_pZwCtrllerLocker->UnLock();
    }
    m_ZwDriver.ProcSendMessage(pZwMessage);

    m_pZwCtrllerLocker->Lock();
    if (m_boAddNodeSend) {
        if (!m_boAddNodeRecv) {
            LOG_INFO("wait add: ++");
        } else {
            LOG_INFO("wait add: --");
        }
        m_pZwCtrllerLocker->UnLock();

        if (!m_evWaitMsgSignal.Wait(AddNodeTimeout)) {
            LOG_INFO("add timeout");
            ProcSelfStopAddNode();
        } else {
            LOG_INFO("wait add: --");
            m_pZwCtrllerLocker->Lock();
            m_boAddNodeSend = FALSE;
            m_boAddNodeRecv = TRUE;
        }
    }

    m_pZwCtrllerLocker->UnLock();
    m_evWaitMsgSignal.Reset();
}

/**
 * @func   ProcZwaveCmdDelNode
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwaveCmdDelNode(
    ZwMessage_p pZwMessage
) {
    if (pZwMessage == NULL) { return; }

    if (pZwMessage->GetBuffer() == NULL) { return; }

    if (pZwMessage->GetZwCommad() == ZwMessage::Command::RmvDevice) {
        m_evWaitMsgSignal.Reset();
        m_pZwCtrllerLocker->Lock();
        m_boDelNodeCmdSend = TRUE;
        m_boDelNodeResReceived = FALSE;
        m_pZwCtrllerLocker->UnLock();
    }
    m_ZwDriver.ProcSendMessage(pZwMessage);

    m_pZwCtrllerLocker->Lock();
    if (m_boDelNodeCmdSend) {
        LOG_INFO("wait del: ++");
        m_pZwCtrllerLocker->UnLock();

        if (!m_evWaitMsgSignal.Wait(RmvNodeTimeout)) {
            LOG_INFO("del timeout");
            ProcSelfStopRmvNode();
        } else {
            m_pZwCtrllerLocker->Lock();
            LOG_INFO("wait del: --");
            m_boDelNodeCmdSend      = FALSE;
            m_boDelNodeResReceived  = TRUE;
        }
    }
    m_pZwCtrllerLocker->UnLock();
    m_evWaitMsgSignal.Reset();
}

/**
 * @func   ProcZwaveCmdRestart
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwaveCmdRestart(
    ZwMessage_p pZwMessage
) {

}

/**
 * @func   ProcSelfStopAddNode
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcSelfStopAddNode() {
    JsonMessagePtr<JsonDevAdd> jsonZwAdd =
    m_pJsonSelfZwaveSession->GetJsonMapping<JsonDevAdd>();

    JsonCommand_p pJsonCommand = jsonZwAdd->CreateJsonCommand(1);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
    (*m_pCtrllerFunctor)(pJsonCommand);
}

/**
 * @func   ProcSelfStopRmvNode
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcSelfStopRmvNode() {
    JsonMessagePtr<JsonDevDel> jsonZwAdd =
    m_pJsonSelfZwaveSession->GetJsonMapping<JsonDevDel>();

    JsonCommand_p pJsonCommand = jsonZwAdd->CreateJsonCommand(1);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
    (*m_pCtrllerFunctor)(pJsonCommand);
}

/**
 * @func   ProcZwCmdSendData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdSendData(
    ZwMessage_p pZwMessage
) {
    m_ZwDriver.ProcSendMessage(pZwMessage);
}

/**
 * @func   Init
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Init() {
    m_ZwDriver.InitDriver();
}
