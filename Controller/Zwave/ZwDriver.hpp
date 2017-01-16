#ifndef ZWDRIVER_HPP_
#define ZWDRIVER_HPP_

#include "typedefs.h"
#include "String.hpp"

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
#include "ZwCmdClassMan.hpp"
#include "ZwTransportAPI.hpp"

#include "ZwCmdClassMap.hpp"

#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "ZwCmdClassMan.hpp"
#include "ZwMessage.hpp"
#include "ZwNode.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"
#include "ValueZwNode.hpp"
#include "ZwCmdAppFunc.hpp"
#include "ZwCmdBasic.hpp"
#include "ZwCmdCtrller.hpp"
#include "ZwCmdFirmware.hpp"
#include "ZwCmdNvm.hpp"
#include "ZwCmdPower.hpp"
#include "ZwCmdTransport.hpp"

class ZwDriver {
    const static i32_t wait_response = 6000;

    u32_t m_dwCountCommand;

    ValueByte_t m_byExpectedCbakId;
    ValueByte_t m_byExpectedNodeId;
    ValueByte_t m_byExpectedFuncId;
    ValueByte_t m_byExpectedCmdCId;
    ValueByte_t m_byExpectedEndPId;

    ValueByte_t  m_byNodeId;                    // Value ZwCmdNvm, ZwCmdCtrller
    ValueDword_t m_dwHomeId;                    // Value ZwCmdNvm, ZwCmdCtrller

    ValueString_t m_strLibraryVersion;          // Value ZwCmdBasic

    ValueByte_t m_pbyApiMask[32];
    ValueByte_t m_pbySerialApiVersion[2];

    ValueWord_t m_wManuFacturerId;
    ValueWord_t m_wProductId;
    ValueWord_t m_wProductType;
    ValueByte_t m_byVersion;
    ValueByte_t m_byCapabilities;
    ValueByte_t m_byChipType;
    ValueByte_t m_byChipVersion;
    ValueEvent_t m_evPacketSignal;

    SZwSerial_t m_SessionZwSerial;
    ZwCmdClassMap_t& m_CmdClassMap;
    ValueLstNode_t& m_ValueLstNode;

    ValueZwCmdCtrller_t& m_ValueZwCmdCtrller;
    ValueZwDriver_t& m_ValueZwDriver;
    ValueZwAppFunc_t& m_ValueZwAppFunc;
    ValueZwCmdNvm_t& m_ValueZwCmdNvm;
    ValueZwCmdBasic_t& m_ValueZwCmdBasic;

    ZwCmdClassMan_p m_pZwCmdClassManager;

    ZwDriverRecvMsgFunctor_t m_ZwDriverRecvMsgFunctor;
    ZwCtrllerFunctor_p m_pZwCtrllerRecvFunctor;

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

    ZwDbModel_p         m_pZwDbModel;
    Locker_p            m_pZwDriverLocker;

    void_t ConfigZwCtrller();

    void_t RegisterHandlers();

    void_t LoadZwDevices(u32_t dwHomeId);
    void_t LoadCmdClasses(u32_t dwKeyId, u8_t bNodeId, u8_t bEndPId);
    void_t LoadZwDatabase();
public:
    ZwDriver(const_char_p cPathZwDriver);
    virtual ~ZwDriver();

    void_t ProcSendMessage(void_p pInBuffer);
    void_t ProcRecvMessage(void_p pInBuffer);

    void_t ZwDriverRecvFunctor(ZwCtrllerFunctor_p pZwCrllerFunctor);

    ZwNode_p InitZwaveNode(u8_t byNodeId);
    ZwNode_p GetZwaveNode(u8_t byNodeId);

    u16_t GetZwNodeVersion (u8_t byNodeId) const;
    u8_t  GetZwNodeBasic   (u8_t byNodeId) const;
    u8_t  GetZwNodeGeneric (u8_t byNodeId) const;
    u8_t  GetZwNodeSpecific(u8_t byNodeId) const;
    u8_t  GetZwNodeType    (u8_t byNodeId) const;

    String GetZwNodeManufactureName(u8_t byNodeId) const;
    String GetZwNodeProductName    (u8_t byNodeId) const;
    String GetZwNodeName           (u8_t byNodeId) const;

    u8_t GetZwNodeRole(u8_t byNodeId) const;
    u8_t GetZwNodePlusType(u8_t byNodeId) const;

    u16_t GetZwNodeManufactureId(u8_t byNodeId) const;
    u16_t GetZwNodeProductType(u8_t byNodeId) const;
    u16_t GetZwNodeProductId(u8_t byNodeId) const;
    u32_t GetZwHomeId() const;

    bool_t IsApiCallSupported() const;

    void_t InitDriver();
    bool_t Start();
    bool_t Connect();
    bool_t Close();
    void_t Debug();

    void_t ProcessFunctor(EvAction event, void_p pBuffer);

    ZwMessage_p GetStateZwNode(u8_t byNodeId);

    ZwMessage_p SetLevelZwNode(u8_t byNodeId, u8_t byLevel);
    ZwMessage_p SetZwNodeOn(u8_t byNodeId);
    ZwMessage_p SetZwNodeOff(u8_t byNodeId);

    void_t HandleSerialApiGetInitDataResponse(ZwPacket_p pZwPacket);
    void_t HandleSerialApiGetCapabilitiesResponse(ZwPacket_p pZwPacket);
};

typedef ZwDriver  ZwDriver_t;
typedef ZwDriver* ZwDriver_p;

#endif /* !ZWDRIVER_HPP_ */
