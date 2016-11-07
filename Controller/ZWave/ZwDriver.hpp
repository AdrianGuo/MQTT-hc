#ifndef ZWDRIVER_HPP_
#define ZWDRIVER_HPP_

#include <ZwMessage.hpp>
#include "typedefs.h"
#include "ZwNode.hpp"

#include "String.hpp"

#include "IZwDriver.hpp"
#include "ZwCmdAppFunc.hpp"
#include "ZwCmdBasic.hpp"
#include "ZwCmdCtrller.hpp"
#include "ZwCmdFirmware.hpp"
#include "ZwCmdPower.hpp"
#include "ZwCmdTransport.hpp"
#include "ZwCmdNvm.hpp"

#include "SZwSerial.hpp"

#include "Vector.hpp"
#include "Event.hpp"

#include "Value.hpp"
#include "ValueByte.hpp"
#include "ValueBool.hpp"
#include "ValueWord.hpp"
#include "ValueDword.hpp"
#include "ValueString.hpp"
#include "ValueEvent.hpp"
#include "ValueZwNode.hpp"

#include "ZwCmdClassMan.hpp"
#include "ZwTransportAPI.hpp"

#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"

#define WAIT_PACKET_RESPONSE             4000

typedef enum ZwCtrllerError {
    ZWCTRLLER_ERROR_COUNT
} ZwCtrllerError_t;

class ZwDriver {
    ValueBool_t m_boWaitResponse;               // Value ZwDriver
    ValueByte_t m_byExpectedCallbackID;         // Value ZwDriver
    ValueByte_t m_byExpectedNodeID;             // Value ZwDriver
    ValueByte_t m_byExpectedFunctionID;         // Value ZwDriver
    ValueByte_t m_byExpectedCmdClassID;         // Value ZwDriver

    ValueByte_t  m_byNodeID;                    // Value ZwCmdNvm, ZwCmdCtrller
    ValueDword_t m_dwHomeID;                    // Value ZwCmdNvm, ZwCmdCtrller

    ValueString_t m_strLibraryVersion;          // Value ZwCmdBasic

    ValueByte_t m_pbyApiMask[32];
    ValueByte_t m_pbySerialApiVersion[2];

    ValueWord_t m_wManuFacturerID;
    ValueWord_t m_wProductID;
    ValueWord_t m_wProductType;
    ValueByte_t m_byVersion;
    ValueByte_t m_byCapabilities;
    ValueByte_t m_byChipType;
    ValueByte_t m_byChipVersion;
    ValueEvent_t m_evPacketSignal;

    ValueZwNode_p m_ppValueZwNode[ZW_MAX_NODES];

    ValueZwDriver_p     m_pValueZwDriver;
    ValueZwCmdBasic_p   m_pValueZwCmdBasic;
    ValueZwCmdNvm_p     m_pValueZwCmdNvm;
    ValueZwCmdCtrller_p m_pValueZwCmdCtrller;

    ZwCmdClassMan_p m_pZwCmdClassManager;

    SZwSerial_p m_pSZwSerial;

    ZwDriverRecvMsgFunctor_t    m_ZwDriverRecvMsgFunctor;
    ZwCtrllerFunctor_p          m_pZwCtrllerRecvFunctor;

    String m_strPathZwDriver;

    ZwCmdBasic_p        m_pZwCmdBasic;
    ZwCmdAppFunc_p      m_pZwCmdAppFunc;
    ZwCmdCtrller_p      m_pZwCmdCtrller;
    ZwCmdFirmware_p     m_pZwCmdFirmware;
    ZwCmdNvm_p          m_pZwCmdNvm;
    ZwCmdPower_p        m_pZwCmdPower;
    ZwCmdTransport_p    m_pZwCmdTransport;
    Vector<IZwDriver_p> m_vecZwCommand;

    HandlerRequest_p    m_pHandlerRequest;
    HandlerResponse_p   m_pHandlerResponse;

    Locker_p m_pZwDriverLocker;
    Packet_p m_pbyOutPacket;

    void_t RegisterHandlers();
public:
    ZwDriver(const_char_p cPathZwDriver);
    ZwDriver(SZwSerial_p pSZwSerial = NULL);
    virtual ~ZwDriver();

    Packet_p ProcSendMessage(void_p pInBuffer,
                             u32_t  dwMsecTimeout = WAIT_PACKET_RESPONSE,
                             bool_t boReturned = FALSE);
    void_t   ProcRecvMessage(void_p pInBuffer);

    void_t ZwDriverSendFunctor();
    bool_t ZwDriverRecvFunctor(ZwCtrllerFunctor_p pZwCrllerFunctor);

    ZwNode_p GetZwNode(u8_t byNodeID);
    void_t InitZwNode(u8_t byNodeID);

    u8_t GetZwNodeVersion(u8_t byNodeID) const;
    u8_t GetZwNodeBasic(u8_t byNodeID) const;
    u8_t GetZwNodeGeneric(u8_t byNodeID) const;
    u8_t GetZwNodeSpecific(u8_t byNodeID) const;
    u8_t GetZwNodeType(u8_t byNodeID) const;

    u8_t GetZwNodeManufactureName(u8_t byNodeID) const;
    u8_t GetZwNodeProductName(u8_t byNodeID) const;
    u8_t GetZwNodeName(u8_t byNodeID) const;
    u8_t GetZwNodeRole(u8_t byNodeID) const;
    u8_t GetZwNodePlusType(u8_t byNodeID) const;

    u8_t GetZwNodeManufactureID(u8_t byNodeID) const;
    u8_t GetZwNodeProductType(u8_t byNodeID) const;
    u8_t GetZwNodeProductID(u8_t byNodeID) const;
    u32_t GetZwHomeID() const;

    String GetPathZwDriver();

    ValueZwDriver_p GetValueZwDriver();

    bool_t IsApiCallSupported() const;

    void_t InitDriver();
    bool_t Start();
    bool_t Connect();
    bool_t Close();

    ZwMessage_p GetStateZwNode(u8_t byNodeID);
    ZwMessage_p SetLevelZwNode(u8_t byNodeID, u8_t   byLevel);
    ZwMessage_p SetStateZwNode(u8_t byNodeID, bool_t boState);
    ZwMessage_p SetZwNodeOn(u8_t byNodeID);
    ZwMessage_p SetZwNodeOff(u8_t byNodeID);

    void_t HandleSerialApiGetInitDataResponse(ZwPacket_p pZwPacket);
    void_t HandleSerialApiGetCapabilitiesResponse(ZwPacket_p pZwPacket);
    void_t HandleReturnedPacket(void_p pInBuffer);
};

typedef ZwDriver  ZwDriver_t;
typedef ZwDriver* ZwDriver_p;

#endif /* !ZWDRIVER_HPP_ */
