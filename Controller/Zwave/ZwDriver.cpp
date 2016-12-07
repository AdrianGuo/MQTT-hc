#include <unistd.h>
#include "LogPlus.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "Functor.hpp"
#include "BasicCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "ValuePtr.hpp"
#include "ZwDriver.hpp"

#define NUM_NODE_BITFIELD_BYTES          29

/**
 * @func   ZwDriver
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::ZwDriver(
    const_char_p cPathZwDriver
) : m_dwCountCommand (0),
    m_SessionZwSerial (cPathZwDriver),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwCmdCtrller(
            ValueRef<ValueZwCmdCtrller>::GetInstance(
                    m_dwHomeId,
                    m_byNodeId)),
    m_ValueZwDriver (
            ValueRef<ValueZwDriver>::GetInstance(
                    m_byExpectedCallbackId,
                    m_byExpectedNodeId,
                    m_byExpectedFunctionId,
                    m_byExpectedCmdClassId,
                    m_byExpectedEndpointId,
                    m_evPacketSignal)),
    m_ValueZwAppFunc (
            ValueRef<ValueZwAppFunc>::GetInstance(
                    m_dwHomeId)) {
    m_pValueZwCmdNvm = ValuePtr<ValueZwCmdNvm>::GetInstance(
                    m_dwHomeId,
                    m_byNodeId);
    m_pValueZwCmdBasic = ValuePtr<ValueZwCmdBasic>::GetInstance(
                                m_strLibraryVersion);

    m_pZwDbModel = ZwDbModel::CreateModel("zwave.db");

    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();

    m_pZwCmdClassManager = ZwCmdClassMan::GetInstance();

    m_pZwCtrllerRecvFunctor = NULL;

    m_ZwDriverRecvMsgFunctor =
    makeFunctor((ZwDriverRecvMsgFunctor_p)NULL, *this, &ZwDriver::ProcRecvMessage);
    m_SessionZwSerial.SZwSerialRecvFunctor(&m_ZwDriverRecvMsgFunctor);

    m_pZwCmdAppFunc     = new ZwCmdAppFunc();
    m_pZwCmdBasic       = new ZwCmdBasic();
    m_pZwCmdCtrller     = new ZwCmdCtrller();
    m_pZwCmdFirmware    = new ZwCmdFirmware();
    m_pZwCmdNvm         = new ZwCmdNvm();
    m_pZwCmdPower       = new ZwCmdPower();
    m_pZwCmdTransport   = new ZwCmdTransport();

    m_vecZwCommand.push_back(m_pZwCmdAppFunc);
    m_vecZwCommand.push_back(m_pZwCmdBasic);
    m_vecZwCommand.push_back(m_pZwCmdCtrller);
    m_vecZwCommand.push_back(m_pZwCmdFirmware);
    m_vecZwCommand.push_back(m_pZwCmdNvm);
    m_vecZwCommand.push_back(m_pZwCmdPower);
    m_vecZwCommand.push_back(m_pZwCmdTransport);

    m_pZwDriverLocker = new Locker();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::~ZwDriver() {
}

/**
 * @func   RegisterHandlers
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::RegisterHandlers() {
    m_pHandlerResponse->RegisterHandler(FUNC_ID_SERIAL_API_GET_INIT_DATA,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwDriver::HandleSerialApiGetInitDataResponse));

    m_pHandlerResponse->RegisterHandler(FUNC_ID_SERIAL_API_GET_CAPABILITIES,
    makeFunctor((HandlerResponseFunctor_p) NULL, *this,
    &ZwDriver::HandleSerialApiGetCapabilitiesResponse));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ZwDriverRecvFunctor(
    ZwCtrllerFunctor_p pCtrllerFunctor
) {
    if (pCtrllerFunctor != NULL) {
        m_pZwCtrllerRecvFunctor = pCtrllerFunctor;
        for (u32_t i = 0; i < m_vecZwCommand.size(); i++) {
            m_vecZwCommand[i]->SetCallbackFunctor(m_pZwCtrllerRecvFunctor);
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
ZwDriver::InitZwaveNode(
    u8_t byNodeId
) {
    m_ValueLstNode[byNodeId - 1] = new ZwNode(m_dwHomeId, byNodeId);
    m_ValueLstNode[byNodeId - 1]->SetCallbackFunctor(m_pZwCtrllerRecvFunctor);
}

/**
 * @func   GetZwaveNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwDriver::GetZwaveNode(
    u8_t byNodeId
) {
    if (byNodeId > ZW_MAX_NODES) { return NULL; }
    return m_ValueLstNode[byNodeId - 1];
}

/**
 * @func   GetZwNodeVersion
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
ZwDriver::GetZwNodeVersion(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetVersion();
}

/**
 * @func   GetZwNodeBasic
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeBasic(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetBasic();
}

/**
 * @func   GetZwNodeGeneric
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeGeneric(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetGeneric();
}

/**
 * @func   GetZwNodeSpecific
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeSpecific(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetSpecific();
}

/**
 * @func   GetZwNodeType
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeType(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetNodeType();
}

/**
 * @func   GetZwNodeManufactureName
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDriver::GetZwNodeManufactureName(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return String();
    }
    return m_ValueLstNode[byNodeId - 1]->GetManufactureName();
}

/**
 * @func   GetZwNodeProductName
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDriver::GetZwNodeProductName(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return String();
    }
    return m_ValueLstNode[byNodeId - 1]->GetProductName();
}

/**
 * @func   GetZwNodeName
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDriver::GetZwNodeName(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return String();
    }
    return m_ValueLstNode[byNodeId - 1]->GetNodeName();
}

/**
 * @func   GetZwNodeRole
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeRole(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetRoleType();
}

/**
 * @func   GetZwNodePlusType
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodePlusType(
     u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetNodeType();
}

/**
 * @func   GetZwNodeManufactureID
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
ZwDriver::GetZwNodeManufactureId(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetManufactureId();
}

/**
 * @func   GetZwNodeProductType
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
ZwDriver::GetZwNodeProductType(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetProductType();
}

/**
 * @func   GetZwNodeProductId
 * @brief  None
 * @param  None
 * @retval None
 */
u16_t
ZwDriver::GetZwNodeProductId(
    u8_t byNodeId
) const {
    if ((byNodeId > ZW_MAX_NODES) || (m_ValueLstNode[byNodeId - 1] == NULL)) {
        return 0;
    }
    return m_ValueLstNode[byNodeId - 1]->GetProductId();
}

/**
 * @func   GetZwHomeId
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
ZwDriver::GetZwHomeId() const {
    return m_dwHomeId;
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Start() {
    return m_SessionZwSerial.Start();
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Connect() {
    return m_SessionZwSerial.Connect();
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Close() {
    return m_SessionZwSerial.Close();
}

/**
 * @func   Debug
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::Debug() {
//    ZwDbDevice devicefind = m_pZwDbModel->Find<ZwDbDeviceItem>().
//    Where("NodeId = ?").Bind(25).Where("Position = ?").Bind(0);
//    ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();
}

/**
 * @func   SetLevelZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::GetStateZwNode(
    u8_t byNodeId
) {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
    BasicCmdClass::GetZwCmdClassId(), m_dwHomeId.GetValue(), byNodeId);
    ZwMessage_p pZwMessage = pZwCmdClass->GetValue();

    return pZwMessage;
}

/**
 * @func   SetLevelZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::SetLevelZwNode(
    u8_t byNodeId,
    u8_t byLevel
) {
    ZwNode_p pZwaveNode = m_ValueLstNode[byNodeId - 1];
    if (pZwaveNode != NULL) { return pZwaveNode->SetLevel(byLevel); }
    return NULL;
}

/**
 * @func   SetZwNodeOn
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::SetZwNodeOn(
    u8_t byNodeId
) {
    ZwNode_p pZwaveNode = m_ValueLstNode[byNodeId - 1];
    if (pZwaveNode != NULL) { return pZwaveNode->SetNodeOn(); }
    return NULL;
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::SetZwNodeOff(
    u8_t byNodeId
) {
    ZwNode_p pZwaveNode = m_ValueLstNode[byNodeId - 1];
    if (pZwaveNode != NULL) { return pZwaveNode->SetNodeOff(); }
    return NULL;
}

/**
 * @func   ProcSendMsg
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ProcSendMessage(
    void_p pInBuffer
) {
    bool_t boStopNetwrk = FALSE;
    ZwMessage_p pCurrentZwMsg = (ZwMessage_p) pInBuffer;

    m_pZwDriverLocker->Lock();
    if (pCurrentZwMsg == NULL) {
        m_byExpectedCallbackId = 0;
        m_byExpectedNodeId     = 0 ;
        m_byExpectedFunctionId = 0;
        m_byExpectedCmdClassId = 0;
        m_byExpectedEndpointId = 0;
        m_pZwDriverLocker->UnLock();
        return;
    }
    m_pZwDriverLocker->UnLock();

    if ((pCurrentZwMsg->GetZwCommad() == ZwMessage::Command::StopAdd) ||
        (pCurrentZwMsg->GetZwCommad() == ZwMessage::Command::StopRmv)) {
        LOG_INFO("    stop adding/removing");
        boStopNetwrk = TRUE;
    }

    if (!boStopNetwrk) {
        m_pZwDriverLocker->Lock();
        m_byExpectedCallbackId = pCurrentZwMsg->GetCallbackId();
        m_byExpectedNodeId     = pCurrentZwMsg->GetTargetNodeId();
        m_byExpectedFunctionId = pCurrentZwMsg->GetExpectedFunctionId();
        m_byExpectedCmdClassId = pCurrentZwMsg->GetExpectedCmdClassId();
        m_byExpectedEndpointId = pCurrentZwMsg->GetExpectedEndpointId();
        m_pZwDriverLocker->UnLock();
    }

    ZwPacket_p pZwPacket = new ZwPacket(pCurrentZwMsg->GetTypeMessage(),
            pCurrentZwMsg->GetFunctionId(), pCurrentZwMsg->Length());

    LOG_INFO("send command %d", ++m_dwCountCommand);

    LOG_INFO("    send: Cb:%3d Func:0x%02x Node:%3d EndP:%3d",
            pCurrentZwMsg->GetCallbackId(),
            pCurrentZwMsg->GetFunctionId(),
            pCurrentZwMsg->GetTargetNodeId(),
            pCurrentZwMsg->GetExpectedEndpointId());

    m_pZwDriverLocker->Lock();
    LOG_INFO("expected: Cb:%3d Func:0x%02x Node:%3d CmdClass:0x%02x",
            m_byExpectedCallbackId.GetValue(),
            m_byExpectedFunctionId.GetValue(),
            m_byExpectedNodeId.GetValue(),
            m_byExpectedCmdClassId.GetValue());
    m_pZwDriverLocker->UnLock();

    if (pCurrentZwMsg->Length() > 0) {
        pZwPacket->Push(pCurrentZwMsg->GetBuffer(), pCurrentZwMsg->Length());
    }

    m_evPacketSignal->Reset();
    m_SessionZwSerial.PushZwPacket(pZwPacket);

    if (!boStopNetwrk) {
        if (!m_evPacketSignal->Wait(wait_response)) {
            LOG_INFO("wait timeout");
            m_pZwDriverLocker->Lock();
            m_byExpectedCallbackId  = 0;
            m_byExpectedNodeId      = 0;
            m_byExpectedFunctionId  = 0;
            m_byExpectedCmdClassId  = 0;
            m_byExpectedEndpointId  = 0;
            m_pZwDriverLocker->UnLock();
        }
    }

    if (pCurrentZwMsg != NULL) {
        delete pCurrentZwMsg;
        pCurrentZwMsg = NULL;
    }
    LOG_INFO("send command %d done", m_dwCountCommand);
}

/**
 * @func   ProcRecvMessage
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ProcRecvMessage(
    void_p pInBuffer
) {
    ZwPacket_p pZwPacket = (ZwPacket_p) pInBuffer;

    m_pZwDriverLocker->Lock();
    if (pZwPacket->GetTypeOfFrame() == RESPONSE) {
        m_pHandlerResponse->ProcessFunctor(pZwPacket);
    } else if (pZwPacket->GetTypeOfFrame() == REQUEST) {
        m_pHandlerRequest->ProcessFunctor(pZwPacket);
    }
    m_pZwDriverLocker->UnLock();

    if (pZwPacket != NULL) { delete pZwPacket; }
}

/**
 * @func   HandleSerialApiGetInitDataResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::HandleSerialApiGetInitDataResponse(
    ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_byExpectedFunctionId == pZwPacket->GetFunctionId()) {
        u8_t byCount = 0;
        u8_p pbyBuffer = pZwPacket->GetBuffer();
        m_byVersion = pbyBuffer[byCount++];
        m_byCapabilities = pbyBuffer[byCount++];
        u8_t byNodeBitField = pbyBuffer[byCount++];

        if (NUM_NODE_BITFIELD_BYTES == byNodeBitField) {
            for (u8_t i = 0; i < NUM_NODE_BITFIELD_BYTES; i++) {
                for (u32_t j = 0; j < 8; j++) {
                    if ((pbyBuffer[byCount] & BV(j)) != 0) {
                        u8_t byNodeId = (8 *i) + j + 1;
                        if (GetZwaveNode(byNodeId) == NULL) { InitZwaveNode(byNodeId); }
                        ZwMessage_p pZwMsgProtocolInfo = m_ValueLstNode[byNodeId - 1]->GetNodeProtocolInfo();
                        ZwMessage_p pZwMsgReqNodeInfo  = m_ValueLstNode[byNodeId - 1]->RequestNodeInfo();
                        ProcessFunctor(EvAction::Pushback, pZwMsgProtocolInfo);
                        ProcessFunctor(EvAction::Pushback, pZwMsgReqNodeInfo);
                    }
                }
                byCount++;
            }
        }
        m_byChipType    = pZwPacket->GetBuffer()[byCount++];
        m_byChipVersion = pZwPacket->GetBuffer()[byCount];

        ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();
        if (controllerfind.get() != NULL) {
            controllerfind.Modify()->ChipType    = m_byChipType;
            controllerfind.Modify()->ChipVersion = m_byChipVersion;
            m_pZwDbModel->Add(controllerfind);
            m_pZwDbModel->UpdateChanges();
        }

        m_byExpectedCallbackId  = 0;
        m_byExpectedNodeId      = 0;
        m_byExpectedFunctionId  = 0;
        m_byExpectedCmdClassId  = 0;
        m_evPacketSignal->Set();
    }
}

/**
 * @func   HandleSerialApiGetCapabilitiesResponse
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::HandleSerialApiGetCapabilitiesResponse(
     ZwPacket_p pZwPacket
) {
    if (pZwPacket == NULL) { return; }

    if (pZwPacket->GetBuffer() == NULL) { return; }

    if (m_byExpectedFunctionId == pZwPacket->GetFunctionId()) {
        u8_p pbyBuffer = pZwPacket->GetBuffer();
        m_pbySerialApiVersion[0]    = pbyBuffer[0];
        m_pbySerialApiVersion[1]    = pbyBuffer[1];
        m_wManuFacturerId           = MERGE(pbyBuffer[2], pbyBuffer[3]);
        m_wProductType              = MERGE(pbyBuffer[4], pbyBuffer[5]);
        m_wProductId                = MERGE(pbyBuffer[6], pbyBuffer[7]);

        for (u8_t i = 0; i < 32; i++) {
            m_pbyApiMask[i] = pbyBuffer[8 + i];
        }

        ZwDbController controllerfind = m_pZwDbModel->Find<ZwDbCtrllerItem>();
        if (controllerfind.get() != NULL) {
            controllerfind.Modify()->ManuId = m_wManuFacturerId;
            controllerfind.Modify()->ProdId = m_wProductId;
            m_pZwDbModel->Add(controllerfind);
            m_pZwDbModel->UpdateChanges();
        }

        m_byExpectedCallbackId  = 0;
        m_byExpectedNodeId      = 0;
        m_byExpectedFunctionId  = 0;
        m_byExpectedCmdClassId  = 0;
        m_evPacketSignal->Set();
    }
}

/**
 * @func   LoadZwDatabase
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::LoadZwDatabase() {
    if (m_pZwDbModel->Controller.get() != NULL) {
    }
}

/**
 * @func   ConfigZwCtrller
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ConfigZwCtrller() {
    LOG_DEBUG("get memory");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_MEMORY_GET_ID, TRUE));

    LOG_DEBUG("get version");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_VERSION, TRUE));

    LOG_DEBUG("serial capabilities");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_SERIAL_API_GET_CAPABILITIES, TRUE));

    LOG_DEBUG("controller capabilities");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES, TRUE));
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::InitDriver() {
    ConfigZwCtrller();

    LOG_DEBUG("node list");
    ProcessFunctor(EvAction::Pushback,
    new ZwMessage(0xFF, REQUEST, FUNC_ID_SERIAL_API_GET_INIT_DATA, TRUE));
}

/**
 * @func   ProcessFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ProcessFunctor(
    EvAction event,
    void_p pBuffer
) {
    if (m_pZwCtrllerRecvFunctor != NULL) {
        (*m_pZwCtrllerRecvFunctor)(event, pBuffer);
    }

}
