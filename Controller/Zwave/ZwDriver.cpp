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
    m_CmdClassMap (ZwCmdClassMap::GetInstance()),
    m_ValueLstNode (ValueLstNode::GetInstance()),
    m_ValueZwCmdCtrller(
            ValueRef<ValueZwCmdCtrller>::GetInstance(
                    m_dwHomeId,
                    m_byNodeId)),
    m_ValueZwDriver (
            ValueRef<ValueZwDriver>::GetInstance(
                    m_byExpectedCbakId,
                    m_byExpectedNodeId,
                    m_byExpectedFuncId,
                    m_byExpectedCmdCId,
                    m_byExpectedEndPId,
                    m_evPacketSignal)),
    m_ValueZwAppFunc (
            ValueRef<ValueZwAppFunc>::GetInstance(
                    m_dwHomeId)),
    m_ValueZwCmdNvm (
            ValueRef<ValueZwCmdNvm>::GetInstance(
                    m_dwHomeId,
                    m_byNodeId)),
    m_ValueZwCmdBasic (
            ValueRef<ValueZwCmdBasic>::GetInstance(
                    m_strLibraryVersion)) {

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
    LoadZwDatabase();
}

/**
 * @func   ~ZwDriver
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::~ZwDriver() {
    if (m_pZwCmdAppFunc != NULL) {
        delete m_pZwCmdAppFunc;
        m_pZwCmdAppFunc = NULL;
    }
    if (m_pZwCmdBasic != NULL) {
        delete m_pZwCmdBasic;
        m_pZwCmdBasic = NULL;
    }

    if (m_pZwCmdCtrller != NULL) {
        delete m_pZwCmdCtrller;
        m_pZwCmdCtrller = NULL;
    }

    if (m_pZwCmdFirmware != NULL) {
        delete m_pZwCmdFirmware;
        m_pZwCmdFirmware = NULL;
    }

    if (m_pZwCmdNvm != NULL) {
        delete m_pZwCmdNvm;
        m_pZwCmdNvm = NULL;
    }

    if (m_pZwCmdPower != NULL) {
        delete m_pZwCmdPower;
        m_pZwCmdPower = NULL;
    }

    if (m_pZwCmdTransport != NULL) {
        delete m_pZwCmdTransport;
        m_pZwCmdTransport = NULL;
    }
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
 * @func   ZwDriverRecvFunctor
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
 * @func   InitZwaveNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwDriver::InitZwaveNode(
    u8_t byNodeId
) {
    m_ValueLstNode[byNodeId - 1] = new ZwNode(m_dwHomeId, byNodeId);
    m_ValueLstNode[byNodeId - 1]->SetCallbackFunctor(m_pZwCtrllerRecvFunctor);
    return m_ValueLstNode[byNodeId - 1];
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
    return m_ValueLstNode[byNodeId - 1]->GetManufacName();
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
 * @func   GetZwNodeManufactureId
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
    return m_ValueLstNode[byNodeId - 1]->GetManufacId();
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
    return m_ValueLstNode[byNodeId - 1]->GetProductTy();
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
    ZwMessage_p pCurrZwMsg = (ZwMessage_p) pInBuffer;

    m_pZwDriverLocker->Lock();
    if (pCurrZwMsg == NULL) {
        m_byExpectedCbakId = 0;
        m_byExpectedNodeId = 0 ;
        m_byExpectedFuncId = 0;
        m_byExpectedCmdCId = 0;
        m_byExpectedEndPId = 0;
        m_pZwDriverLocker->UnLock();
        return;
    }
    m_pZwDriverLocker->UnLock();

    if ((pCurrZwMsg->GetZwCommad() == ZwMessage::Command::StopAdd) ||
        (pCurrZwMsg->GetZwCommad() == ZwMessage::Command::StopRmv)) {
        LOG_DEBUG("stop adding/removing");
        boStopNetwrk = TRUE;
    }

    if (!boStopNetwrk) {
        m_pZwDriverLocker->Lock();
        m_byExpectedCbakId = pCurrZwMsg->GetCallbackId();
        m_byExpectedNodeId = pCurrZwMsg->GetTargetNodeId();
        m_byExpectedFuncId = pCurrZwMsg->GetExpectedFunctionId();
        m_byExpectedCmdCId = pCurrZwMsg->GetExpectedCmdClassId();
        m_byExpectedEndPId = pCurrZwMsg->GetExpectedEndpointId();
        m_pZwDriverLocker->UnLock();
    }

    ZwPacket_p pZwPacket = new ZwPacket(
                                pCurrZwMsg->GetTypeMessage(),
                                pCurrZwMsg->GetFunctionId(),
                                pCurrZwMsg->Length());

    LOG_INFO("send command %d", ++m_dwCountCommand);

//    LOG_DEBUG("send: Func: %02x Node: %03d EndP: %03d Cb: %03d",
//                    pCurrZwMsg->GetFunctionId(),
//                    pCurrZwMsg->GetTargetNodeId(),
//                    pCurrZwMsg->GetExpectedEndpointId(),
//                    pCurrZwMsg->GetCallbackId());
//
//    LOG_DEBUG("expc: Func: %02x Node: %03d CmdC:  %02X Cb: %03d",
//                    m_byExpectedFuncId.GetValue(),
//                    m_byExpectedNodeId.GetValue(),
//                    m_byExpectedCmdCId.GetValue(),
//                    m_byExpectedCbakId.GetValue());

    if (pCurrZwMsg->Length() > 0) {
        pZwPacket->Push(pCurrZwMsg->GetBuffer(), pCurrZwMsg->Length());
    }

    LOG_INFO("data: %s", pZwPacket->PrintfPacket().c_str());

    m_evPacketSignal->Reset();
    m_SessionZwSerial.PushZwPacket(pZwPacket);

    if (!boStopNetwrk) {
        if (!m_evPacketSignal->Wait(wait_response)) {
            LOG_INFO("send command %2d timeout", m_dwCountCommand);
            m_pZwDriverLocker->Lock();
            m_byExpectedNodeId = 0;
            m_byExpectedCbakId = 0;
            m_byExpectedFuncId = 0;
            m_byExpectedCmdCId = 0;
            m_byExpectedEndPId = 0;
            m_pZwDriverLocker->UnLock();
            u8_t byCountResend = pCurrZwMsg->GetCountResend();
            if (byCountResend < pCurrZwMsg->ResendMax) {
                LOG_INFO("resend command %2d %2d", m_dwCountCommand, byCountResend);
                ProcessFunctor(EvAction::Pushback, pCurrZwMsg); // Resend
                pCurrZwMsg->IncCountResend();
                return;
            } else {
                LOG_INFO("resend command %2d fail", m_dwCountCommand);
            }
        } else {
            LOG_INFO("send command %2d done", m_dwCountCommand);
        }
    } else {
        LOG_INFO("send command %d done", m_dwCountCommand);
    }

    if (pCurrZwMsg != NULL) {
        delete pCurrZwMsg;
        pCurrZwMsg = NULL;
    }
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
    if (pZwPacket->GetTpeOfFrame() == RESPONSE) {
        m_pHandlerResponse->ProcessFunctor(pZwPacket);
    } else if (pZwPacket->GetTpeOfFrame() == REQUEST) {
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

    if (m_byExpectedFuncId == pZwPacket->GetFunctionId()) {
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

        m_byExpectedCbakId  = 0;
        m_byExpectedNodeId      = 0;
        m_byExpectedFuncId  = 0;
        m_byExpectedCmdCId  = 0;
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

    if (m_byExpectedFuncId == pZwPacket->GetFunctionId()) {
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

        m_byExpectedCbakId  = 0;
        m_byExpectedNodeId      = 0;
        m_byExpectedFuncId  = 0;
        m_byExpectedCmdCId  = 0;
        m_evPacketSignal->Set();
    }
}

/**
 * @func   LoadCmdClasses
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::LoadCmdClasses(
    u32_t dwKeyId,
    u8_t bNodeId,
    u8_t bEndPId
) {
    ZwDbCommandClasses cmdClasses = m_pZwDbModel->Find<ZwDbCmdClassItem>().
    Where("DevId = ?").Bind((int_t)dwKeyId);
    for (ZwDbCommandClasses::const_iterator it = cmdClasses.begin();
            it != cmdClasses.end(); it++) {
        ZwNode_p pZwNode = m_ValueLstNode[bNodeId - 1];
        u8_t byCmdClass = (u8_t) (*it)->CmdId;
        (*pZwNode)[bEndPId]->AddZwCmdClass(byCmdClass);
    }
}

/**
 * @func   LoadZwDevices
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::LoadZwDevices(
    u32_t dwHomeId
) {
    for (ZwDbDevices::const_iterator it = m_pZwDbModel->Devices.begin();
            it != m_pZwDbModel->Devices.end(); it++) {
        ZwNode_p pZwNode = NULL;
        u8_t byKeyId  = (u8_t) it->Id();
        u8_t byDevTy  = (u8_t) (*it)->DevType;
        u8_t byNodeId = (u8_t) (*it)->NodeId;
        u8_t byEndPId = (u8_t) (*it)->Position;

        if ((-1) == (*it)->ParId) {
            pZwNode = InitZwaveNode(byNodeId);

        } else {
            if (m_ValueLstNode[byNodeId - 1] != NULL) {
                pZwNode = m_ValueLstNode[byNodeId - 1]->AddEndpoint(byEndPId);
            }
        }

        if (pZwNode != NULL) {
            pZwNode->SetDeviceType(byDevTy);
            LoadCmdClasses(byKeyId, byNodeId, byEndPId);
        }
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
        u32_t dwHomeId = m_pZwDbModel->Controller->HomeId;
        if (-1 != (i32_t) dwHomeId) {
            m_dwHomeId = dwHomeId;
            LoadZwDevices(dwHomeId);
        }
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
