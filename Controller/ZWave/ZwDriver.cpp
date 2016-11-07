#include <unistd.h>
#include "debug.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwCtrllerAPI.hpp"
#include "Functor.hpp"
#include "BasicCmdClass.hpp"
#include "MultilevelSwitchCmdClass.hpp"
#include "ValueShare.hpp"

#include "ZwDriver.hpp"

#define NUM_NODE_BITFIELD_BYTES          29

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::ZwDriver(
    const_char_p cPathZwDriver
) : m_strPathZwDriver (cPathZwDriver) {

    m_pValueZwCmdCtrller = ValueShare<ValueZwCmdCtrller>::GetInstance(
                                m_dwHomeID,
                                m_byNodeID);

    m_pValueZwDriver = ValueShare<ValueZwDriver>::GetInstance(
                                m_byExpectedCallbackID,
                                m_byExpectedNodeID,
                                m_byExpectedFunctionID,
                                m_byExpectedCmdClassID,
                                m_evPacketSignal);

    m_pValueZwCmdNvm = ValueShare<ValueZwCmdNvm>::GetInstance(
                                m_dwHomeID,
                                m_byNodeID);

    m_pValueZwCmdBasic = ValueShare<ValueZwCmdBasic>::GetInstance(
                                m_strLibraryVersion);

    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();

    m_pZwCmdClassManager = ZwCmdClassMan::GetInstance();

    m_pSZwSerial = new SZwSerial(cPathZwDriver);

    m_pZwCtrllerRecvFunctor = NULL;

    m_ZwDriverRecvMsgFunctor =
            makeFunctor((ZwDriverRecvMsgFunctor_p)NULL, *this, &ZwDriver::ProcRecvMessage);
    ZwDriverSendFunctor();

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

    for (u32_t i = 0; i < m_vecZwCommand.size(); i++) {
        m_vecZwCommand[i]->ShareNodeObject(m_ppValueZwNode);
    }

    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        m_ppValueZwNode[i] = NULL;
    }

    m_pZwDriverLocker = new Locker();
    m_pbyOutPacket = NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::ZwDriver(
    SZwSerial_p pSZwSerial
) : m_pSZwSerial (pSZwSerial) {
    m_pValueZwCmdCtrller = ValueShare<ValueZwCmdCtrller>::GetInstance(
                                    m_dwHomeID,
                                    m_byNodeID);

    m_pValueZwDriver = ValueShare<ValueZwDriver>::GetInstance(
                                    m_byExpectedCallbackID,
                                    m_byExpectedNodeID,
                                    m_byExpectedFunctionID,
                                    m_byExpectedCmdClassID,
                                    m_evPacketSignal);

    m_pValueZwCmdNvm = ValueShare<ValueZwCmdNvm>::GetInstance(
                                    m_dwHomeID,
                                    m_byNodeID);

    m_pValueZwCmdBasic = ValueShare<ValueZwCmdBasic>::GetInstance(
                                    m_strLibraryVersion);

    m_strPathZwDriver = m_pSZwSerial->GetNamePort();

    m_pHandlerRequest   = HandlerRequest::GetInstance();
    m_pHandlerResponse  = HandlerResponse::GetInstance();
    RegisterHandlers();

    m_pZwCmdClassManager = ZwCmdClassMan::GetInstance();

    m_pZwCtrllerRecvFunctor = NULL;

    m_ZwDriverRecvMsgFunctor =
            makeFunctor((ZwDriverRecvMsgFunctor_p)NULL, *this, &ZwDriver::ProcRecvMessage);
    ZwDriverSendFunctor();

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

    for (u32_t i = 0; i < m_vecZwCommand.size(); i++) {
        m_vecZwCommand[i]->ShareNodeObject(m_ppValueZwNode);
    }

    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        m_ppValueZwNode[i] = NULL;
    }

    m_pZwDriverLocker = new Locker();
    m_pbyOutPacket = NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDriver::~ZwDriver() {
    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        if (m_ppValueZwNode[i] != NULL) {
            delete m_ppValueZwNode[i];
            m_ppValueZwNode[i] = NULL;
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
ZwDriver::RegisterHandlers() {
    m_pHandlerResponse->RegisterHandler(FUNC_ID_APPLICATION_COMMAND_HANDLER,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwDriver::HandleSerialApiGetInitDataResponse));
    m_pHandlerResponse->RegisterHandler(FUNC_ID_APPLICATION_COMMAND_HANDLER,
            makeFunctor((HandlerResponseFunctor_p) NULL, *this, &ZwDriver::HandleSerialApiGetCapabilitiesResponse));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ZwDriverSendFunctor() {
    if (m_pSZwSerial != NULL) {
        m_pSZwSerial->SZwSerialRecvFunctor(&m_ZwDriverRecvMsgFunctor);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::ZwDriverRecvFunctor(
    ZwCtrllerFunctor_p pZwCtrllerFunctor
) {
    if (pZwCtrllerFunctor != NULL) {
        m_pZwCtrllerRecvFunctor = pZwCtrllerFunctor;
        for (u32_t i = 0; i < m_vecZwCommand.size(); i++) {
            m_vecZwCommand[i]->ShareCallbackFunctor(m_pZwCtrllerRecvFunctor);
        }
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
ZwDriver::InitZwNode(
    u8_t byNodeID
) {
    if (m_ppValueZwNode[byNodeID - 1] != NULL) {
        delete m_ppValueZwNode[byNodeID - 1];
        m_ppValueZwNode[byNodeID - 1] = NULL;
    }
    m_ppValueZwNode[byNodeID - 1] = new ValueZwNode(new ZwNode(m_dwHomeID.GetValue(), byNodeID));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ZwDriver::GetZwNode(
    u8_t byNodeID
) {
    if ((byNodeID > ZW_MAX_NODES) || (m_ppValueZwNode[byNodeID - 1] == NULL)) {
        return NULL;
    }

    return m_ppValueZwNode[byNodeID - 1]->GetValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeVersion(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeBasic(
    u8_t byNodeID
) const {
    if ((byNodeID > ZW_MAX_NODES) || (m_ppValueZwNode[byNodeID - 1] == NULL)) {
        return 0;
    }
    return m_ppValueZwNode[byNodeID - 1]->GetValue()->GetBasic();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeGeneric(
    u8_t byNodeID
) const {
    if ((byNodeID > ZW_MAX_NODES) || (m_ppValueZwNode[byNodeID - 1] == NULL)) {
        return 0;
    }
    return m_ppValueZwNode[byNodeID - 1]->GetValue()->GetGeneric();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeSpecific(
    u8_t byNodeID
) const {
    if ((byNodeID > ZW_MAX_NODES) || (m_ppValueZwNode[byNodeID - 1] == NULL)) {
        return 0;
    }
    return m_ppValueZwNode[byNodeID - 1]->GetValue()->GetSpecific();
}

/**
 * @func   GetZwNodeType
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeType(
    u8_t byNodeID
) const {
    if ((byNodeID > ZW_MAX_NODES) || (m_ppValueZwNode[byNodeID - 1] == NULL)) {
        return 0;
    }
    return m_ppValueZwNode[byNodeID - 1]->GetValue()->GetNodeType();
}

/**
 * @func   GetZwNodeManufactureName
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeManufactureName(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeProductName
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeProductName(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeName
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeName(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeRole
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeRole(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodePlusType
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodePlusType(
     u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeManufactureID
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeManufactureID(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeProductType
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeProductType(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwNodeProductID
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwDriver::GetZwNodeProductID(
    u8_t byNodeID
) const {
    return 0;
}

/**
 * @func   GetZwHomeID
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
ZwDriver::GetZwHomeID() const {
    return m_dwHomeID.GetValue();
}

/**
 * @func   GetZwNodeProductID
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDriver::GetPathZwDriver() {
    return m_strPathZwDriver;
}

/**
 * @func   GetValueZwDriver
 * @brief  None
 * @param  None
 * @retval None
 */
ValueZwDriver_p
ZwDriver::GetValueZwDriver() {
    return m_pValueZwDriver;
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Start() {
    if (m_pSZwSerial != NULL) {
        return m_pSZwSerial->Start();
    }
    return FALSE;
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Connect() {
    if (m_pSZwSerial != NULL) {
        return m_pSZwSerial->Connect();
    }
    return FALSE;
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZwDriver::Close() {
    if (m_pSZwSerial != NULL) {
        return m_pSZwSerial->Close();
    }
    return FALSE;
}

/**
 * @func   SetLevelZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::GetStateZwNode(
    u8_t byNodeID
) {
    ZwCmdClass_p pZwCmdClass = m_pZwCmdClassManager->CreateCmdClass(
            BasicCmdClass::GetZwCmdClassID(), m_dwHomeID.GetValue(), byNodeID);
    ZwMessage_p pZwMsg = pZwCmdClass->GetValue();

    return pZwMsg;
}

/**
 * @func   SetLevelZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::SetLevelZwNode(
    u8_t byNodeID,
    u8_t byLevel
) {
    if (m_ppValueZwNode[byNodeID - 1] == NULL) { return NULL; }
    ZwNode_p pZwNode = m_ppValueZwNode[byNodeID - 1]->GetValue();
    if (pZwNode != NULL) { return pZwNode->SetLevel(byLevel); }
    return NULL;
}

/**
 * @func   SetStateZwNode
 * @brief  None
 * @param  None
 * @retval None
 */
ZwMessage_p
ZwDriver::SetStateZwNode(
    u8_t byNodeID,
    bool_t boState
) {
    ZwNode_p pZwNode = m_ppValueZwNode[byNodeID - 1]->GetValue();
    if (pZwNode != NULL) {
        return pZwNode->SetNode(boState);
    }
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
    u8_t byNodeID
) {
    ZwNode_p pZwNode = m_ppValueZwNode[byNodeID - 1]->GetValue();
    if (pZwNode != NULL) {
        return pZwNode->SetNodeOn();
    }
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
    u8_t byNodeID
) {
    ZwNode_p pZwNode = m_ppValueZwNode[byNodeID - 1]->GetValue();
    if (pZwNode != NULL) {
        return pZwNode->SetNodeOff();
    }
    return NULL;
}

/**
 * @func   ProcSendMsg
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p
ZwDriver::ProcSendMessage(
    void_p pInBuffer,
    u32_t  dwMsecTimeout,
    bool_t boReturned
) {
    bool_t boStopNetwrk = FALSE;
    ZwMessage_p pCurrentZwMsg = (ZwMessage_p) pInBuffer;

    m_pZwDriverLocker->Lock();
    if (pCurrentZwMsg == NULL) {
        m_boWaitResponse = FALSE;
        m_byExpectedCallbackID = 0;
        m_byExpectedNodeID = 0 ;
        m_byExpectedFunctionID = 0;
        m_byExpectedCmdClassID = 0;
        m_pZwDriverLocker->UnLock();
        return NULL;
    }
    m_pZwDriverLocker->UnLock();

    if ((pCurrentZwMsg->GetZwCommad() == ZwMessage::Command::StopAdd) ||
        (pCurrentZwMsg->GetZwCommad() == ZwMessage::Command::StopRmv)) {
        DEBUG1("  STOP ADD/RMV");
        boStopNetwrk = TRUE;
    }

    if (!boStopNetwrk) {
        m_pZwDriverLocker->Lock();
        m_boWaitResponse = TRUE;
        m_byExpectedCallbackID = pCurrentZwMsg->GetCallbackID();
        m_byExpectedNodeID = pCurrentZwMsg->GetTargetNodeID();
        m_byExpectedFunctionID = pCurrentZwMsg->GetExpectedFunctionID();
        m_byExpectedCmdClassID = pCurrentZwMsg->GetExpectedCmdClassID();
        m_pZwDriverLocker->UnLock();
    }
    ZwPacket_p pZwPacket = new ZwPacket(pCurrentZwMsg->GetTypeMessage(),
            pCurrentZwMsg->GetFunctionID(), pCurrentZwMsg->Length());

    DEBUG2("    send: Cb:%3d Func:0x%02x Node:%3d",
            pCurrentZwMsg->GetCallbackID(),
            pCurrentZwMsg->GetFunctionID(),
            pCurrentZwMsg->GetTargetNodeID());

    m_pZwDriverLocker->Lock();
    DEBUG2("expected: Cb:%3d Func:0x%02x Node:%3d CmdClass:0x%02x",
            m_byExpectedCallbackID.GetValue(),
            m_byExpectedFunctionID.GetValue(),
            m_byExpectedNodeID.GetValue(),
            m_byExpectedCmdClassID.GetValue());
    m_pZwDriverLocker->UnLock();

    if (pCurrentZwMsg->Length() > 0) {
        pZwPacket->Push(pCurrentZwMsg->GetBuffer(), pCurrentZwMsg->Length());
    }

    m_evPacketSignal.GetValue()->Reset();
    m_pSZwSerial->PushZwPacket(pZwPacket);

    if (!boStopNetwrk) {
        if (!m_evPacketSignal.GetValue()->Wait(dwMsecTimeout)) {
            DEBUG1("Wait Timeout");
            m_byExpectedCallbackID  = 0;
            m_byExpectedNodeID      = 0;
            m_byExpectedFunctionID  = 0;
            m_byExpectedCmdClassID  = 0;
        }
    }
    m_boWaitResponse = FALSE;

    if (pCurrentZwMsg != NULL) {
        delete pCurrentZwMsg;
        pCurrentZwMsg = NULL;
    }

    m_pZwDriverLocker->Lock();

    if ((boReturned) && (m_pbyOutPacket != NULL)) {
        Packet_p pOutPacket = new Packet(m_pbyOutPacket->Count());
        pOutPacket->Push(m_pbyOutPacket->GetBuffer(), m_pbyOutPacket->Count());

        delete m_pbyOutPacket;
        m_pbyOutPacket = NULL;
        m_pZwDriverLocker->UnLock();
        return pOutPacket;
    }

    if (m_pbyOutPacket != NULL) {
        delete m_pbyOutPacket;
        m_pbyOutPacket = NULL;
    }

    DEBUG1("  SEND DONE");
    m_pZwDriverLocker->UnLock();

    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::ProcRecvMessage(
    void_p pInBuffer
) {
    ZwPacket_p pZwPacket = (ZwPacket_p) pInBuffer;

    if ((pZwPacket->GetTypeOfFrame() == REQUEST) &&
        (pZwPacket->GetFunctionID() == FUNC_ID_APPLICATION_COMMAND_HANDLER)) {
        HandleReturnedPacket(pZwPacket);
    }

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
 * @func
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

    if (m_byExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u8_t byCount = 0;
        u8_p pbyBuffer = pZwPacket->GetBuffer();
        m_byVersion = pbyBuffer[byCount++];
        m_byCapabilities = pbyBuffer[byCount++];
        u8_t byNodeBitField = pbyBuffer[byCount++];

        if (NUM_NODE_BITFIELD_BYTES == byNodeBitField) {
            for (u8_t i = 0; i < NUM_NODE_BITFIELD_BYTES; i++) {
                for (u32_t j = 0; j < 8; j++) {
                    if ((pbyBuffer[byCount] & BV(j)) != 0) {
                        u8_t byNodeID = (8 *i) + j + 1;
                        ZwNode_p pZwNode = GetZwNode(byNodeID);
                        if (pZwNode == NULL) {
                            InitZwNode(byNodeID);
                        }
                    }
                }
                byCount++;
            }
        }
        m_byChipType    = pZwPacket->GetBuffer()[byCount++];
        m_byChipVersion = pZwPacket->GetBuffer()[byCount];

        m_pZwDriverLocker->Lock();
        m_byExpectedCallbackID  = 0;
        m_byExpectedNodeID      = 0;
        m_byExpectedFunctionID  = 0;
        m_byExpectedCmdClassID  = 0;
        m_pZwDriverLocker->UnLock();
    }
    m_evPacketSignal.GetValue()->Set();
}

/**
 * @func
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

    if (m_byExpectedFunctionID == pZwPacket->GetFunctionID()) {
        u8_p pbyBuffer = pZwPacket->GetBuffer();
        m_pbySerialApiVersion[0]    = pbyBuffer[0];
        m_pbySerialApiVersion[1]    = pbyBuffer[1];
        m_wManuFacturerID           = MERGE(pbyBuffer[2], pbyBuffer[3]);
        m_wProductType              = MERGE(pbyBuffer[4], pbyBuffer[5]);
        m_wProductID                = MERGE(pbyBuffer[6], pbyBuffer[7]);

        for (u8_t i = 0; i < 32; i++) {
            m_pbyApiMask[i] = pbyBuffer[8 + i];
        }

        m_pZwDriverLocker->Lock();
        m_byExpectedCallbackID  = 0;
        m_byExpectedNodeID      = 0;
        m_byExpectedFunctionID  = 0;
        m_byExpectedCmdClassID  = 0;
        m_pZwDriverLocker->UnLock();
    }
    m_evPacketSignal.GetValue()->Set();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::HandleReturnedPacket(
    void_p pInBuffer
) {
    ZwPacket_p pZwPacket = (ZwPacket_p) pInBuffer;
    u8_p pBuffer    = pZwPacket->GetBuffer();
    u8_t byNodeID   = pBuffer[1];
    u8_t byLength   = pBuffer[2];
    u8_t byCmdClass = pBuffer[3];

    if ((FUNC_ID_APPLICATION_COMMAND_HANDLER == m_byExpectedFunctionID) &&
        (byCmdClass != COMMAND_CLASS_DEVICE_RESET_LOCALLY) && (m_byExpectedNodeID == byNodeID)) {
        m_pZwDriverLocker->Lock();
        m_pbyOutPacket = new Packet(byLength);
        m_pbyOutPacket->Push(&pBuffer[3], byLength);
        m_pZwDriverLocker->UnLock();
    }
}

/**
 * @func   InitDriver
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwDriver::InitDriver() {
    DEBUG1("START INIT");
    DEBUG1("GET VERSION");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_VERSION, TRUE));

    DEBUG1("MEMORY");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_MEMORY_GET_ID, TRUE));

    DEBUG1("NODE LIST");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_SERIAL_API_GET_INIT_DATA, TRUE));

    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        if (m_ppValueZwNode[i] != NULL) {
            DEBUG2("NODE PROTOCOL INFO %d", m_ppValueZwNode[i]->GetValue()->GetNodeID());
            ZwMessage_p pZwMsg = new ZwMessage(m_ppValueZwNode[i]->GetValue()->GetNodeID(),
                    REQUEST, FUNC_ID_ZW_GET_NODE_PROTOCOL_INFO, TRUE);
            pZwMsg->ResetPacket(1);
            pZwMsg->Push(m_ppValueZwNode[i]->GetValue()->GetNodeID());
            ProcSendMessage(pZwMsg);
        }
    }

    for (u32_t i = 1; i < ZW_MAX_NODES; i++) {
        if (m_ppValueZwNode[i] != NULL) {
            DEBUG2("NODE INFO %d", m_ppValueZwNode[i]->GetValue()->GetNodeID());
            ZwMessage_p pZwMsg = new ZwMessage(m_ppValueZwNode[i]->GetValue()->GetNodeID(),
                    REQUEST, FUNC_ID_ZW_REQUEST_NODE_INFO, TRUE);
            pZwMsg->ResetPacket(1);
            pZwMsg->Push(m_ppValueZwNode[i]->GetValue()->GetNodeID());
            ProcSendMessage(pZwMsg);
        }
    }

    DEBUG1("SERIAL CAPABILITIES");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_SERIAL_API_GET_CAPABILITIES, TRUE));

    DEBUG1("CONTROLLER CAPABILITIES");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES, TRUE));

    DEBUG1("NODE INFORMATION");
    ProcSendMessage(new ZwMessage(0xFF, REQUEST, FUNC_ID_ZW_GET_CONTROLLER_CAPABILITIES, TRUE));

    DEBUG1("INIT DONE");
}
