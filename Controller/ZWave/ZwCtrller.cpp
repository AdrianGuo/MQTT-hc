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
#include <stddef.h>
#include "debug.hpp"
#include "ZwNode.hpp"
#include "LThread.hpp"

#include "JsonCommand.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwSerialAPI.hpp"
#include "MultiChannelCmdClass.hpp"

#include "JsonZwAdd.hpp"
#include "JsonZwAddRes.hpp"
#include "JsonZwDel.hpp"
#include "JsonZwDelRes.hpp"
#include "JsonZwGet.hpp"
#include "JsonZwGetRes.hpp"
#include "JsonZwLstAdd.hpp"
#include "JsonZwLstAddRes.hpp"
#include "JsonZwLstDel.hpp"
#include "JsonZwLstDelRes.hpp"
#include "JsonZwReset.hpp"
#include "JsonZwResetRes.hpp"
#include "JsonZwRestart.hpp"
#include "JsonZwRestartRes.hpp"

#include "JsonDevGet.hpp"
#include "JsonDevSet.hpp"

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
/**
 * @func   ZwCtrller
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ZwCtrller::ZwCtrller(
    ZwDriver_p pZwDriver
) : m_valueJsonChecker() {
    m_pZwDriver = pZwDriver;

    m_boAddNodeCmdSend = FALSE;
    m_boDelNodeCmdSend = FALSE;
    m_boAddNodeResReceived = FALSE;
    m_boDelNodeResReceived = FALSE;

    m_pZwCtrllerThread = new LThread();
    m_pZwCtrllerLocker = new Locker();
    m_ZwCtrllerThreadFunctor = makeFunctor(
            (threadFunctor_p) NULL, *this, &ZwCtrller::ZwCtrlllerThreadProc);
    m_pZwCtrllerThread->RegThreadFunctor(&m_ZwCtrllerThreadFunctor);

    m_evWaitMsgSignal.Reset();

    m_ZwCtrllerFunctor = makeFunctor(
            (ZwCtrllerFunctor_p) NULL, *this, &ZwCtrller::PushJsonCommand);

    m_pJsonRecvZwaveSession = JsonRecvZwaveSession::CreateSession();
    m_pJsonSendZwaveSession = JsonSendZwaveSession::CreateSession();
    RegisterJsonMessageInform();

    ZwCtrllerSendFunctor();
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ZwCtrllerSendFunctor() {
    if (m_pZwDriver != NULL) {
        m_pZwDriver->ZwDriverRecvFunctor(&m_ZwCtrllerFunctor);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwCtrller::ICtrllerRecvFunctor(
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
ZwCtrller::Process() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::Start() {
    if (m_pZwDriver != NULL) {
        if (m_pZwDriver->Start()) {
            DEBUG1("START SUCCESS");
        } else {
            DEBUG1("START FAIL");
        }
    }

    if (m_pZwCtrllerThread != NULL) {
        m_pZwCtrllerThread->Start();
    }
}

/**
 * @func   RegisterInform
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::RegisterJsonMessageInform() {
    m_valueJsonChecker.Register(JsonZwAdd::GetStrCmd());
    m_valueJsonChecker.Register(JsonZwDel::GetStrCmd());
    m_valueJsonChecker.Register(JsonZwReset::GetStrCmd());
    m_valueJsonChecker.Register(JsonZwRestart::GetStrCmd());
    m_valueJsonChecker.Register(JsonDevSet::GetStrCmd(), JsonCommand::Flag::Database);
    m_valueJsonChecker.Register(JsonDevGet::GetStrCmd(), JsonCommand::Flag::Database);

    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwAdd>(JsonZwAdd::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwDel>(JsonZwDel::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwGet>(JsonZwGet::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwLstAddRes>(JsonZwLstAddRes::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwLstDelRes>(JsonZwLstDelRes::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwReset>(JsonZwReset::GetStrCmd());
    m_pJsonRecvZwaveSession->MapJsonMessage<JsonZwRestart>(JsonZwRestart::GetStrCmd());

    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwAddRes>(JsonZwAddRes::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwDelRes>(JsonZwDelRes::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwGetRes>(JsonZwGetRes::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwLstAdd>(JsonZwLstAdd::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwLstDel>(JsonZwLstDel::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwResetRes>(JsonZwResetRes::GetStrCmd());
    m_pJsonSendZwaveSession->MapJsonMessage<JsonZwRestartRes>(JsonZwRestartRes::GetStrCmd());

    RegisterHandler(JsonZwAdd::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwCmdAdd));
    RegisterHandler(JsonZwDel::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwCmdDel));
    RegisterHandler(JsonZwReset::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwCmdReset));
    RegisterHandler(JsonZwRestart::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerZwCmdRestart));
    RegisterHandler(JsonDevGet::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerDevCmdGet));
    RegisterHandler(JsonDevSet::GetStrCmd(),
            makeFunctor((HandlerZwCmdFunctor_p) NULL, *this, &ZwCtrller::HandlerDevCmdSet));

    RegisterProc(ZwMessage::Command::SendData,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdSendData));
    RegisterProc(ZwMessage::Command::AddDevice,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdAddNode));
    RegisterProc(ZwMessage::Command::StopAdd,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdAddNode));
    RegisterProc(ZwMessage::Command::RmvDevice,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdDelNode));
    RegisterProc(ZwMessage::Command::StopRmv,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdDelNode));
    RegisterProc(ZwMessage::Command::GetEnPoints,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdGetEndpoints));
    RegisterProc(ZwMessage::Command::GetCapability,
            makeFunctor((ProcZwCmdFunctor_p) NULL, *this, &ZwCtrller::ProcZwCmdGetCapability));

}

/**
 * @func   IsZwaveCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwCtrller::LoadCmdClass(
    JsonCommand_p pJsonCommand
) {
    bool_t boRetVal = FALSE;
    if (pJsonCommand->GetDesFlag() && JsonCommand::Flag::Zwave != 0) {
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
ZwCtrller::RegisterHandler(
    String strJsonCommand,
    HandlerZwCmdFunctor_t funcTor
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
ZwCtrller::ProcessHandler(
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
ZwCtrller::RegisterProc(
    ZwMessage::Command zwCommand,
    ProcZwCmdFunctor_t funcTor
) {
    m_mapProcFunctor[zwCommand] = funcTor;
}

/**
 * @func
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
    MapProcFunctor::const_iterator_t it = m_mapProcFunctor.find(zwCommand);
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
    void_p pInBuffer
) {
    if (evAction == EvAction::Set) {
        m_evWaitMsgSignal.Set();
    } else if (evAction == EvAction::Reset) {
        m_evWaitMsgSignal.Reset();
    } else if (evAction == EvAction::Pushback) {
        ZwMessage_p pZwMsg = (ZwMessage_p) pInBuffer;
        m_pZwCtrllerLocker->Lock();
        if (pZwMsg != NULL) { m_queSendZwMsg.push(pZwMsg); }
        m_pZwCtrllerLocker->UnLock();

        return;
    }

    if ((m_pICtrllerFunctor != NULL) && (pInBuffer != NULL)) {
        m_pICtrllerFunctor->operator ()((JsonCommand_p) pInBuffer);
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
ZwCtrller::HandlerZwCmdAdd(
    JsonCommand_p pJsonCommand
) {
    m_pZwCtrllerLocker->Lock();
    if (!m_boDelNodeCmdSend) {
        m_pZwCtrllerLocker->UnLock();

        JsonMessagePtr<JsonZwAdd> jsonZwAdd = m_pJsonRecvZwaveSession->GetJsonMapping<JsonZwAdd>();

        jsonZwAdd->ParseJsonCommand(pJsonCommand);

        if ((jsonZwAdd->Act() == 0) || (jsonZwAdd->Act() == 1)) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boAddNodeCmdSend) {
                m_pZwCtrllerLocker->UnLock();

                ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand, ZwMessage::Command::AddDevice);
                m_evWaitMsgSignal.Reset();

                m_pZwCtrllerLocker->Lock();
                m_queSendZwMsg.push(pZwMessage);
            }

            delete pJsonCommand;
            pJsonCommand = NULL;
            m_pZwCtrllerLocker->UnLock();
        } else if (jsonZwAdd->Act() == 2) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boAddNodeResReceived) {
                m_pZwCtrllerLocker->UnLock();

                m_evWaitMsgSignal.Set();

                m_pZwCtrllerLocker->Lock();
            }

            ZwMessage_p pZwMessage = new ZwMessage( pJsonCommand, ZwMessage::Command::StopAdd);
            m_queSendZwMsg.push(pZwMessage);

            delete pJsonCommand;
            pJsonCommand = NULL;
            m_pZwCtrllerLocker->UnLock();
        }
    } else {
        delete pJsonCommand;
        pJsonCommand = NULL;
        m_pZwCtrllerLocker->UnLock();
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwCmdDel(
    JsonCommand_p pJsonCommand
) {
    m_pZwCtrllerLocker->Lock();
    if (!m_boAddNodeCmdSend) {
        m_pZwCtrllerLocker->UnLock();

        JsonMessagePtr<JsonZwDel> jsonZwDel = m_pJsonRecvZwaveSession->GetJsonMapping<JsonZwDel>();

        if (!jsonZwDel->ParseJsonCommand(pJsonCommand)) { return; }

        if ((jsonZwDel->Act() == 0) || (jsonZwDel->Act() == 1)) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boDelNodeCmdSend) {
                m_pZwCtrllerLocker->UnLock();
                ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand, ZwMessage::Command::RmvDevice);

                m_evWaitMsgSignal.Reset();

                m_pZwCtrllerLocker->Lock();
                m_queSendZwMsg.push(pZwMessage);
            }

            delete pJsonCommand;
            pJsonCommand = NULL;
            m_pZwCtrllerLocker->UnLock();
        } else if (jsonZwDel->Act() == 2) {
            m_pZwCtrllerLocker->Lock();
            if (!m_boDelNodeResReceived) {
                m_pZwCtrllerLocker->UnLock();

                m_evWaitMsgSignal.Set();

                m_pZwCtrllerLocker->Lock();
            }

            ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand, ZwMessage::Command::StopRmv);
            m_queSendZwMsg.push(pZwMessage);

            delete pJsonCommand;
            pJsonCommand = NULL;
            m_pZwCtrllerLocker->UnLock();
        }
    } else {
        delete pJsonCommand;
        pJsonCommand = NULL;
        m_pZwCtrllerLocker->UnLock();
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwCmdReset(
    JsonCommand_p pJsonCommand
) {
    ZwMessage_p pZwMessage = new ZwMessage(pJsonCommand);
    m_pZwCtrllerLocker->Lock();
    if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
    m_pZwCtrllerLocker->UnLock();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerZwCmdRestart(
    JsonCommand_p pJsonCommand
) {

}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerDevCmdSet(
    JsonCommand_p pJsonCommand
) {
    Json::Value root = pJsonCommand->GetJsonOjbect();

    if ((m_pZwDriver == NULL) || !root.isMember("dev")) { return; }

    Json::Value array = root["dev"];

    for (uint_t i = 0; i < root.size(); i++) {
        Json::Value dev = array[i];
        u16_t byDevID       = std::stoi(dev["devID"].asString());
        u8_t  byNodeID      = LO_UINT16(byDevID);
        u8_t  byEndPID      = HI_UINT16(byDevID);

        ZwMessage_p pZwMessage = NULL;
        ZwNode_p pZwNode = m_pZwDriver->GetZwNode(byNodeID);
        if (pZwNode == NULL) { return; }

        if (byEndPID == 0) {
            u8_t byDeviceType = pZwNode->GetDeviceType();
            if (byDeviceType == DEVICE_TYPE_CONTACT) {
                u8_t  byValue = std::stoi(dev["val"].asString());
                pZwMessage = pZwNode->SetNode((byValue != 0) ? TRUE : FALSE);
            } else if (byDeviceType == DEVICE_TYPE_DIMMER) {
                u8_t  byValue = std::stoi(dev["val"].asString());
                pZwMessage = pZwNode->SetLevel(byValue);
            }
        } else if (pZwNode->IsSupportMultiChannel()) {

        }

        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandlerDevCmdGet(
    JsonCommand_p pJsonCommand
) {
    Json::Value root = pJsonCommand->GetJsonOjbect();

    if ((m_pZwDriver == NULL) || !root.isMember("dev")) { return; }

    Json::Value array = root["dev"];

    for (uint_t i = 0; i < root.size(); i++) {
        Json::Value dev = array[i];
        u8_t byDevID = std::stoi(dev["devID"].asString());
        ZwMessage_p pZwMessage = m_pZwDriver->GetStateZwNode(byDevID);

        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::HandleSendCmdGetCapability(
    u8_t byNodeID,
    u8_t byNbrEndpoints
) {
    if (m_pZwDriver == NULL) { return; }

    ZwNode_p pZwNode = m_pZwDriver->GetZwNode(byNodeID);

    if (pZwNode == NULL) { return; }

    for (u8_t i = 0; i < byNbrEndpoints; i++) {
        MultiChannelCmdClass_p multiChannelCmdClass =
                (MultiChannelCmdClass_p) pZwNode->GetZwCmdClass(MultiChannelCmdClass::GetZwCmdClassID());
        ZwMessage_p pZwMessage = multiChannelCmdClass->SendCapabilitiyGet(i + 1);
        pZwMessage->SetZwCommand(ZwMessage::Command::GetCapability);

        m_pZwCtrllerLocker->Lock();
        if (pZwMessage != NULL) { m_queSendZwMsg.push(pZwMessage); }
        m_pZwCtrllerLocker->UnLock();
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdAddNode(
    ZwMessage_p pZwMessage
) {
    if (pZwMessage == NULL) { return; }

    if (pZwMessage->GetBuffer() == NULL) { return; }

    if (pZwMessage->GetZwCommad() == ZwMessage::Command::AddDevice) {
        m_evWaitMsgSignal.Reset();
        m_pZwCtrllerLocker->Lock();
        m_boAddNodeCmdSend      = TRUE;
        m_boAddNodeResReceived  = FALSE;
        m_pZwCtrllerLocker->UnLock();
    }
    m_pZwDriver->ProcSendMessage(pZwMessage);

    m_pZwCtrllerLocker->Lock();
    if (m_boAddNodeCmdSend) {
        DEBUG1("wait add: ++");
        m_pZwCtrllerLocker->UnLock();

        m_evWaitMsgSignal.Wait(WAIT_INFINITE);

        m_pZwCtrllerLocker->Lock();
        DEBUG1("wait add: --");
        m_boAddNodeCmdSend      = FALSE;
        m_boAddNodeResReceived  = TRUE;
    }
    m_pZwCtrllerLocker->UnLock();
    m_evWaitMsgSignal.Reset();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdDelNode(
    ZwMessage_p pZwMessage
) {
    if (pZwMessage == NULL) { return; }

    if (pZwMessage->GetBuffer() == NULL) { return; }

    if (pZwMessage->GetZwCommad() == ZwMessage::Command::RmvDevice) {
        m_evWaitMsgSignal.Reset();
        m_pZwCtrllerLocker->Lock();
        m_boDelNodeCmdSend      = TRUE;
        m_boDelNodeResReceived  = FALSE;
        m_pZwCtrllerLocker->UnLock();
    }
    m_pZwDriver->ProcSendMessage(pZwMessage);

    m_pZwCtrllerLocker->Lock();
    if (m_boDelNodeCmdSend) {
        DEBUG1("wait del: ++");
        m_pZwCtrllerLocker->UnLock();

        m_evWaitMsgSignal.Wait(WAIT_INFINITE);

        m_pZwCtrllerLocker->Lock();
        DEBUG1("wait del: --");
        m_boDelNodeCmdSend      = FALSE;
        m_boDelNodeResReceived  = TRUE;
    }
    m_pZwCtrllerLocker->UnLock();
    m_evWaitMsgSignal.Reset();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdRestart(
    ZwMessage_p pZwMessage
) { }

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdGetEndpoints(
    ZwMessage_p pZwMessage
) {
    DEBUG1("MULTICHANNEL ENDPOINTS");

    if ((pZwMessage == NULL) || (m_pZwDriver == NULL)) { return; }

    u8_t byNodeID = pZwMessage->GetTargetNodeID();
    Packet_p pOutPacket = m_pZwDriver->ProcSendMessage(pZwMessage, WAIT_PACKET_RESPONSE, TRUE);

    if (pOutPacket == NULL) { return; }

    ZwNode_p pZwNode = m_pZwDriver->GetZwNode(byNodeID);
    if (pZwNode == NULL) { return; }

    u8_t byNbrOfIndividualEndpoints = pOutPacket->GetBuffer()[3];
    pZwNode->SetNbrOfEndpoints(byNbrOfIndividualEndpoints);

    delete pOutPacket;
    pOutPacket = NULL;

    HandleSendCmdGetCapability(byNodeID, byNbrOfIndividualEndpoints);
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdGetCapability(
    ZwMessage_p pZwMessage
) {
    DEBUG1("MULTICHANNEL CAPABILITY");

    if ((pZwMessage == NULL) || (m_pZwDriver == NULL)) {
        return;
    }

    u8_t byNodeID = pZwMessage->GetTargetNodeID();
    Packet_p pOutPacket = m_pZwDriver->ProcSendMessage(pZwMessage, WAIT_PACKET_RESPONSE, TRUE);

    if (pOutPacket == NULL) { return; }

    ZwNode_p pZwNode = m_pZwDriver->GetZwNode(byNodeID);

    if (pZwNode == NULL) { return; }

    u8_t byEndpointID = pOutPacket->GetBuffer()[2];
    u8_t byGeneric    = pOutPacket->GetBuffer()[3];
    u8_t bySpecific   = pOutPacket->GetBuffer()[4];
    ZwEndpoint_p pZwEndpoint = pZwNode->GetZwEndPoint(byEndpointID);
    pZwEndpoint->SetGeneric(byGeneric);
    pZwEndpoint->SetSpecific(bySpecific);

    for (u8_t i = 0; i < pOutPacket->Count() - 5; i++) {
        pZwNode->AddZwCmdClass(pOutPacket->GetBuffer()[5 + i]);
    }

    delete pOutPacket;
    pOutPacket = NULL;

    u8_t byEndpointType = pZwEndpoint->GetTypeEndpoint();
    JsonCommand_p pJsonCommand = new JsonCommand("zw", "lstAdd");

    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zwave);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);

    Json::Value jsonValue;
    Json::Value jsonDevice;
    jsonDevice["devID"] = std::to_string(MERGE(byEndpointID, byNodeID));
    jsonDevice["type"]  = std::to_string(byEndpointType);
    jsonDevice["ord"]   = std::to_string(byEndpointID);
    jsonValue["dev"].append(jsonDevice);

    pJsonCommand->SetJsonObject(jsonValue);

    PushJsonCommand(EvAction::None, pJsonCommand);
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::ProcZwCmdSendData(
    ZwMessage_p pZwMessage
) {
    if (m_pZwDriver != NULL) { m_pZwDriver->ProcSendMessage(pZwMessage); }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCtrller::IniZwCtrller() {
    if (m_pZwDriver != NULL) { m_pZwDriver->InitDriver(); }
}
