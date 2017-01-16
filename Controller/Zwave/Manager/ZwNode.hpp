#ifndef NODE_HPP_
#define NODE_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "String.hpp"
#include "Functor.hpp"
#include "ZwDefs.hpp"
#include "ValueDevice.hpp"
#include "ZwCmdClass.hpp"
#include "ZwCmdClassMan.hpp"
#include "IZwDriver.hpp"

class ZwNode {
    /* Contruction */
private:
public:
    ZwNode(u32_t dwHomeId = 0, u8_t byNodeId = 0);
    virtual ~ZwNode();

    /* Initialization */
private:
    bool_t m_boInfoReceived;
    bool_t m_boPlusInfoReceived;
    bool_t m_boNodeAlive;
public:
    bool_t IsNodeZWavePlus() const { return m_boPlusInfoReceived; }
    bool_t IsNodeAlive() const { return m_boNodeAlive; }
    void_t SetNodeAlive(bool_t boNodeLive) { m_boNodeAlive = boNodeLive; }

    /* Capabilities */
private:
    bool_t m_boAddingNode;
    bool_t m_boListening;               /* TRUE:  AOE - Always On Slave  */
                                        /* FALSE: Battery - Powerd Device */
    bool_t m_boFreqListening;           /* FLIRS: Frequent Listening Slave */
                                        /* TRUE:  LSS - Listening Sleeping Slave */
                                        /* FALSE: RSS - Reporting Sleeping Slave */
    bool_t m_boSecurity;
    bool_t m_boIsController;

    u8_t  m_byNodeId;
    u32_t m_dwHomeId;
    u8_t  m_byCapability;
    u8_t  m_byBasic;                  /* 01: Controller 02: Static Controller 03: Slave 04: Routing Slave */
    u8_t  m_byGeneric;
    u8_t  m_bySpecific;
    u8_t  m_byDevType;
    u8_t  m_byVersion;

public:
    u8_t   GetBasic()      const { return m_byBasic; }
    u32_t  GetHomeId()     const { return m_dwHomeId; }
    u8_t   GetNodeId()     const { return m_byNodeId; }
    u8_t   GetDevType()    const { return m_byDevType; }
    bool_t GetSecurity()   const { return m_boSecurity; }
    u8_t   GetCapability() const { return m_byCapability; }
    u8_t   GetGeneric()    const { return m_byGeneric; }
    u8_t   GetSpecific()   const { return m_bySpecific; }
    u8_t   GetVersion()    const { return m_byVersion; }

    void_t SetSecurity   (bool_t boSercurity) { m_boSecurity = boSercurity; }
    void_t SetNodeId     (u8_t byNodeId)      { m_byNodeId = byNodeId; }
    void_t SetCapability (u8_t byCapability)  { m_byCapability = byCapability; }
    void_t SetBasic      (u8_t byBasic)       { m_byBasic = byBasic; }
    void_t SetGeneric    (u8_t byGeneric)     { m_byGeneric = byGeneric; }
    void_t SetSpecific   (u8_t bySpecific)    { m_bySpecific = bySpecific; }
    void_t SetVersion    (u8_t byVersion)     { m_byVersion = byVersion; }
    void_t SetHomeId     (u32_t dwHomeId)     { m_dwHomeId = dwHomeId; }
    void_t SetDeviceType (u8_t byDeviceType)  { m_byDevType = byDeviceType; }
    void_t SetDeviceType ();

    bool_t IsController() const {
        return (((m_byBasic == BASIC_TYPE_CONTROLLER) || (m_byBasic == BASIC_TYPE_STATIC_CONTROLLER)) && \
                ((m_bySpecific == GENERIC_TYPE_GENERIC_CONTROLLER) || (m_bySpecific == GENERIC_TYPE_STATIC_CONTROLLER)));
    }

    bool_t IsAddingNode() const { return m_boAddingNode; }
    void_t SetAddingNode(bool_t boAddingNode) { m_boAddingNode = boAddingNode; }

    bool_t IsListeningNode() const { return m_boListening; }

    bool_t IsSecurityNode() const { return m_boSecured; }


    /* Device Name */
private:
    String m_strManufacName;
    String m_strProductName;
    String m_strNodeName;

    u16_t m_wManufacId;
    u16_t m_wProductTy;             /* 01: Gateway 02: ZWave 03: ZWave Plus */
    u16_t m_wProductId;

    u8_t m_byRoleType;
    u8_t m_byNodeType;

    u16_t m_wIconType;
public:
    String GetManufacName() const { return m_strManufacName; }
    String GetProductName() const { return m_strProductName; }
    String GetNodeName() const { return m_strNodeName; }

    u16_t GetManufacId() const { return m_wManufacId; }
    u16_t GetProductTy() const { return m_wProductTy; }
    u16_t GetProductId() const { return m_wProductId; }

    u8_t GetRoleType() const { return m_byRoleType; }
    u8_t GetNodeType() const { return m_byNodeType; }

    u16_t GetIconType() const { return m_wIconType; }

    void_t SetManuFacName (String strManufacName) { m_strManufacName = strManufacName; }
    void_t SetProductName (String strProductName) { m_strProductName = strProductName; }
    void_t SetNodeName    (String strNodeName) { m_strNodeName = strNodeName; }

    void_t SetManufacId (u16_t wManufacId) { m_wManufacId = wManufacId; }
    void_t SetProductTy (u16_t wProductTy) { m_wProductTy = wProductTy; }
    void_t SetProductId (u16_t wProductId) { m_wProductId = wProductId; }

    void_t SetRoleType (u8_t byRoleType) { m_byRoleType = byRoleType; }
    void_t SetNodeType (u8_t byNodeType) { m_byNodeType = byNodeType; }
    void_t SetIconType (u16_t wIconType) { m_wIconType  = wIconType; }

    /* Command Class */
private:
    ZwCmdClassMan_p m_pZwCmdClassManager;
    typedef Map<u8_t, ZwCmdClass_p> MapCmdClass;
    MapCmdClass m_mZwCmdClass;
    bool_t m_boSecured;
public:
    ZwCmdClass_p  GetZwCmdClass(const u8_t byZwCmdClassId);
    ZwCmdClass_p  AddZwCmdClass(const u8_t byZwCmdClassId);
    void_t        RmvZwCmdClass(const u8_t byZwCmdClassId);

    bool_t IsMultiChannel();

    ValueDevice_p ApplicationCommandHandler(u8_p pbyInBuffer, u8_t byLength);
    /* Basic Command */
private:
public:
    ZwMessage_p SetLevel(const u8_t byLevel);
    ZwMessage_p SetNodeOn();
    ZwMessage_p SetNodeOff();

    /* General Command */
private:
    static Map<Value::Type_t, u8_t> m_mapValueToCmdClass;
public:
    static void_t MapValueToCmdClass();
    ZwMessage_p SetNodeValue(ValueDevice_p pValueDevice);
    void_t SetNodeValue(ValueDevice_p pValueDevice, u8_p& ppBuffer, u8_p pLength);

    /* Statistics */
private:
    u32_t m_dwRecvUnsolicited;
public:
    void_t IncUnsolicitedCount() { m_dwRecvUnsolicited++; }

    /* Endpoint */
private:
    u8_t m_byNbrOfEndpoints;
    typedef Map<u8_t, ZwNode*> ZwEnpoint;
    ZwEnpoint m_ZwEndpoints;
public:
    void_t SetNbrOfEndpoints(u8_t byNbrOfEndpoints) { m_byNbrOfEndpoints = byNbrOfEndpoints; }
    u8_t   GetNbrOfEndpoints() const { return m_byNbrOfEndpoints; }

    ZwNode* GetEndpoint(const u8_t byEndpointId);
    ZwNode* AddEndpoint(u8_t byEndpointId);
    void_t  RmvEndpoint(u8_t byEndpointId);

    ZwNode* operator[] (u8_t byEndpointId);
    ZwMessage_p SetEnpointValue(u8_t bEndpointId, ValueDevice_p pValueDevice);
    /* Encapsulate */
private:
public:
    /* Functor */
private:
    ZwCtrllerFunctor_p m_pControllerFunctor;
public:
    void_t SetCallbackFunctor(ZwCtrllerFunctor_p pCtrllerFunctor);
    void_t ProcessFunctor(EvAction event, void_p pBuffer);
    /* Init */
public:
    ZwMessage_p GetNodeProtocolInfo();
    ZwMessage_p RequestNodeInfo();
    ZwMessage_p GetZwPlusInfo();
};

typedef ZwNode  ZwNode_t;
typedef ZwNode* ZwNode_p;

#endif /* !NODE_HPP */
