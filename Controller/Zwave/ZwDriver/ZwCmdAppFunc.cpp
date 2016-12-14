#include "LogPlus.hpp"
#include "HelperHc.hpp"

#include "String.hpp"

#include "HcDevice.hpp"

#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "ZwDefs.hpp"
#include "ZwPacket.hpp"

#include "JsonCommand.hpp"
#include "JsonMessagePtr.hpp"

#include "JsonDevLstAdd.hpp"
#include "JsonDevLstDel.hpp"
#include "JsonDevStt.hpp"

#include "MultiChannelCmdClass.hpp"
#include "ZwNode.hpp"
#include "ValuePtr.hpp"
#include "HcDevice.hpp"
#include "ZwCmdAppFunc.hpp"

/**
 * @func   ZwCmdAppFunc
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdAppFunc::ZwCmdAppFunc(
) : IZwDriver(),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwDriver (ValueRef<ValueZwDriver>::GetInstance()),
    m_ValueZwAppFunc (ValueRef<ValueZwAppFunc>::GetInstance()),
    m_pJsonZwaveSession (JsonSendZwaveSession::CreateSession()),
    m_pZwDbModel (ZwDbModel::CreateModel("zwave.db")),
    m_pHandlerRequest (HandlerRequest::GetInstance()),
    m_pHandlerResponse (HandlerResponse::GetInstance()) {
    RegisterHandlers();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwCmdAppFunc::~ZwCmdAppFunc() { }

/**
 * @func   ProcResetLocally
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ProcResetLocally(
    u8_t byNodeId
) {
    if (m_ValueLstNode[byNodeId - 1] != NULL) {
        delete m_ValueLstNode[byNodeId - 1];
        m_ValueLstNode[byNodeId - 1] = NULL;
    }

    // Database
    ZwDbDevice devicefind =
    m_pZwDbModel->Find<ZwDbDeviceItem>().Where("NodeId = ?").Bind(byNodeId);
    if (devicefind.get() != NULL) {
        devicefind.Remove();
        m_pZwDbModel->UpdateChanges();
    }

    // Send JsonCommand
    Vector<JsonDevLstDel::Device_t> lstDelZwNode;
    JsonDevLstDel::Device_t delZwNode;

    delZwNode.devid = (i32_t) byNodeId;
    delZwNode.netwk = ZWAVE_NETW;
    delZwNode.order = ROOT_ORDER;

    lstDelZwNode.push_back(delZwNode);

    JsonMessagePtr<JsonDevLstDel> jsonZwLstDel =
    m_pJsonZwaveSession->GetJsonMapping<JsonDevLstDel>();

    JsonCommand_p pJsonCommand = jsonZwLstDel->CreateJsonCommand(lstDelZwNode);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Coord);

    ProcessFunctor(EvAction::None, pJsonCommand);
}

/**
 * @func   ProcMultiChannel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ProcMultiChannel(
    u8_p pbCommand,
    u8_t byLength
) {
    u8_t byCommandRes = pbCommand[1];
    switch (byCommandRes) {
    case MULTI_CHANNEL_END_POINT_REPORT_V4:
    case MULTI_CHANNEL_END_POINT_FIND_REPORT_V4:
    case MULTI_CHANNEL_AGGREGATED_MEMBERS_REPORT_V4:
        m_ValueZwDriver.expectedCbakId = 0;
        m_ValueZwDriver.expectedNodeId = 0;
        m_ValueZwDriver.expectedFuncId = 0;
        m_ValueZwDriver.expectedCmdCId = 0;
        m_ValueZwDriver.expectedEndPId = 0;
        m_ValueZwDriver.packetSignal->Set();
        break;

    case MULTI_CHANNEL_CAPABILITY_REPORT_V4:
        {
            u8_t byEndpointId = pbCommand[2];
            if (byEndpointId == m_ValueZwDriver.expectedEndPId) {
                m_ValueZwDriver.expectedCbakId = 0;
                m_ValueZwDriver.expectedNodeId = 0;
                m_ValueZwDriver.expectedFuncId = 0;
                m_ValueZwDriver.expectedCmdCId = 0;
                m_ValueZwDriver.expectedEndPId = 0;
                m_ValueZwDriver.packetSignal->Set();
            }
        }
        break;
    }
}

/**
 * @func   ProcApplicationCommandHandler
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::ProcApplicationCommandHandler(
    u8_t byNodeId,
    u8_p pbCommand,
    u8_t byLength
) {
    ZwNode_p pZwRootNode = m_ValueLstNode[byNodeId - 1];

    ValueDevice_p pValueDevice =
    pZwRootNode->ApplicationCommandHandler(pbCommand, byLength);

    u8_t byOrder =
    (pbCommand[0] == MultiChannelCmdClass::GetZwCmdClassId()) ? pbCommand[2] : 0;

    if ((pValueDevice != NULL) && (*pZwRootNode)[byOrder] != NULL) {
        u8_t byDeviceType = (*pZwRootNode)[byOrder]->GetDevType();

        if (byDeviceType == DEVICE_TYPE_UNKNOW) {
            delete pValueDevice;
            pValueDevice = NULL;
            return;
        }

        Vector<JsonDevStt::Device_t> vecLstZwNode;
        JsonDevStt::Device_t node;

        node.devid = (i32_t) byNodeId;
        node.netwk = ZWAVE_NETW;
        node.order = byOrder;
        node.type  = byDeviceType;
        node.value = pValueDevice->CreateJson();

        vecLstZwNode.push_back(node);

        JsonMessagePtr<JsonDevStt> jsonZwStt =
        m_pJsonZwaveSession->GetJsonMapping<JsonDevStt>();
        JsonCommand_p pJsonCommand = jsonZwStt->CreateJsonCommand(vecLstZwNode);

        pJsonCommand->SetDesFlag(JsonCommand::Flag::Coord);

        delete pValueDevice;
        pValueDevice = NULL;

        ProcessFunctor(EvAction::None, pJsonCommand);
    }
}

/**
 * @func   HandleApplicationCommandHandlerRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t 
ZwCmdAppFunc::HandleApplicationCommandHandlerRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }
    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_t byNodeId    =  pZwPacket->GetBuffer()[1];
    u8_t byLength    =  pZwPacket->GetBuffer()[2];
    u8_t byCmdClass  =  pZwPacket->GetBuffer()[3];
    u8_p pbCommand   = &pZwPacket->GetBuffer()[3];

    if (byCmdClass == COMMAND_CLASS_DEVICE_RESET_LOCALLY) {
        ProcResetLocally(byNodeId);
        return;
    }

    if (m_ValueLstNode[byNodeId - 1] == NULL) {
        m_ValueLstNode[byNodeId - 1] =
        new ZwNode(m_ValueZwAppFunc.homeId, byNodeId);
    }
    m_ValueLstNode[byNodeId - 1]->SetNodeAlive(TRUE);

    if (FUNC_ID_APPLICATION_COMMAND_HANDLER ==
        m_ValueZwDriver.expectedFuncId &&
        m_ValueZwDriver.expectedNodeId == byNodeId &&
        m_ValueZwDriver.expectedCmdCId == byCmdClass) {
        if (byCmdClass == MultiChannelCmdClass::GetZwCmdClassId()) {
            ProcMultiChannel(pbCommand, byLength - 3);
        } else {
            m_ValueZwDriver.expectedCbakId = 0;
            m_ValueZwDriver.expectedNodeId = 0;
            m_ValueZwDriver.expectedFuncId = 0;
            m_ValueZwDriver.expectedCmdCId = 0;
            m_ValueZwDriver.packetSignal->Set();
        }
    } else {
        m_ValueLstNode[byNodeId - 1]->IncUnsolicitedCount();
    }

    ProcApplicationCommandHandler(byNodeId, pbCommand, byLength - 3);
}

/**
 * @func   HandleApplicationNodeInformationRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t 
ZwCmdAppFunc::HandleApplicationNodeInformationRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }
}

/**
 * @func   HandleApplicationSlaveUpdateRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::HandleApplicationSlaveUpdateRequest(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    u8_p pBuffer = pZwPacket->GetBuffer();
    u8_t byNodeId   = pBuffer[1]; // bSource
    u8_t byLength   = pBuffer[2];
    u8_t byBasic    = pBuffer[3];
    u8_t byGeneric  = pBuffer[4];
    u8_t bySpecific = pBuffer[5];

    if ((byNodeId == 0) || (byNodeId == 0xFF)) { return; }

    LOG_DEBUG("node %d update", byNodeId);

    ZwDbDevice devicefind = m_pZwDbModel->Find<ZwDbDeviceItem>().
    Where("NodeId = ?").Bind(byNodeId).Where("Position = ?").Bind(ROOT_ORDER);

    if (m_ValueLstNode[byNodeId - 1] != NULL) {
        m_ValueLstNode[byNodeId - 1]->SetBasic(byBasic);
        m_ValueLstNode[byNodeId - 1]->SetGeneric(byGeneric);
        m_ValueLstNode[byNodeId - 1]->SetSpecific(bySpecific);

        if (devicefind.get() != NULL) {
            ZwDbCommandClasses cmdclassfind = m_pZwDbModel->Find<ZwDbCmdClassItem>().
            Where("DevId = ?").Bind(devicefind->NodeId);

        }

        for (u8_t i = 0; i < byLength - 3; i++) {
            m_ValueLstNode[byNodeId - 1]->AddZwCmdClass(pBuffer[6 + i]);
//            ZwDbCommandClass cmdclass =
//            m_pZwDbModel->Add(new ZwDbCmdClassItem());
//            cmdclass.Modify()->CmdId  = pBuffer[6 + i];
//            cmdclass.Modify()->CmdHex = GetHex(pBuffer[6 + i]);
//            cmdclass.Modify()->Device = devicefind;
        }

        ZwMessage_p pZwMessage = m_ValueLstNode[byNodeId - 1]->GetZwPlusInfo();
        ProcessFunctor(EvAction::Pushback, pZwMessage);
    }
    m_ValueZwDriver.packetSignal->Set();
}

/**
 * @func   HandleApplicationControllerUpdateRequest
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::HandleApplicationControllerUpdateRequest(
    ZwPacket_p pZwPacket
) {
}

/**
 * @func   RegisterHandlers
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwCmdAppFunc::RegisterHandlers() {
    m_pHandlerRequest->RegisterHandler(FUNC_ID_APPLICATION_COMMAND_HANDLER,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdAppFunc::HandleApplicationCommandHandlerRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_SERIAL_API_APPL_NODE_INFORMATION,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdAppFunc::HandleApplicationNodeInformationRequest));
    m_pHandlerRequest->RegisterHandler(FUNC_ID_ZW_APPLICATION_UPDATE,
    makeFunctor((HandlerRequestFunctor_p) NULL, *this,
    &ZwCmdAppFunc::HandleApplicationSlaveUpdateRequest));
}
