#ifndef NODE_HPP_
#define NODE_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "String.hpp"
#include "ZwDefs.hpp"
#include "ZwEndpoint.hpp"
#include "ZwCmdClass.hpp"
#include "ZwCmdClassMan.hpp"

#define DEVICE_TYPE_CONTACT                     1
#define DEVICE_TYPE_DIMMER                      2
#define DEVICE_TYPE_FAN                         3
#define DEVICE_TYPE_CURTAIN                     4
#define DEVICE_TYPE_RGB                         5
#define DEVICE_TYPE_INFRARED_REMOTE             6

#define DEVICE_TYPE_PIR_SENSOR                  256
#define DEVICE_TYPE_DOOR_SENSOR                 257
#define DEVICE_TYPE_TEMPERATURE_SENSOR          258
#define DEVICE_TYPE_HUMINITY_SENSOR             259
#define DEVICE_TYPE_LIGHT_SENSOR                260

class ZwNode {
    /* Contruction */
private:
public:
    ZwNode(u32_t dwHomeID = 0, u8_t byNodeID = 0);
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

    u8_t m_byNodeID;
    u8_t m_byCapability;
    u8_t m_byBasic;                  /* 01: Controller 02: Static Controller 03: Slave 04: Routing Slave */
    u8_t m_byGeneric;
    u8_t m_bySpecific;

    u16_t m_wVersion;
    u32_t m_dwHomeID;
public:
    u8_t   GetDeviceType();
    bool_t GetSecurity() const { return m_boSecurity; }
    u8_t   GetNodeID() const { return m_byNodeID; }
    u8_t   GetCapability() const { return m_byCapability; }
    u8_t   GetBasic() const { return m_byBasic; }
    u8_t   GetGeneric() const { return m_byGeneric; }
    u8_t   GetSpecific() const { return m_bySpecific; }
    u16_t  GetVersion() const { return m_wVersion; }
    u32_t  GetHomeID() const { return m_dwHomeID; }

    void_t SetSecurity(bool_t boSercurity) { m_boSecurity = boSercurity; }
    void_t SetNodeID(u8_t byNodeID) { m_byNodeID = byNodeID; }
    void_t SetCapability(u8_t byCapability) { m_byCapability = byCapability; }
    void_t SetBasic(u8_t byBasic) { m_byBasic = byBasic; }
    void_t SetGeneric(u8_t byGeneric) { m_byGeneric = byGeneric; }
    void_t SetSpecific(u8_t bySpecific) { m_bySpecific = bySpecific; }
    void_t SetVersion(u16_t wVersion) { m_wVersion = wVersion; }
    void_t SetHomeID(u32_t dwHomeID) { m_dwHomeID = dwHomeID; }

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
    String m_strManufactureName;
    String m_strProductName;
    String m_strNodeName;
    u16_t m_wManufactureID;
    u16_t m_wProductType;             /* 01: Gateway 02: ZWave 03: ZWave Plus */
    u16_t m_wProductID;
    u8_t m_byRoleType;
    u8_t m_byNodeType;
    u8_t m_byIconType;
public:
    String GetManufactureName() const { return m_strManufactureName; }
    void_t SetManuFactureName(String strManufactureName) { m_strManufactureName = strManufactureName; }

    String GetProductName() const { return m_strProductName; }
    void_t SetProductName(String strProductName) { m_strProductName = strProductName; }

    String GetNodeName() const { return m_strNodeName; }
    void_t SetNodeName(String strNodeName) { m_strNodeName = strNodeName; }

    u16_t GetManufactureID() const { return m_wManufactureID; }
    void_t GetManufactureID(u16_t wManufactureID) { m_wManufactureID = wManufactureID; }

    u16_t GetProductType() const { return m_wProductType; }
    void_t SetProductType(u16_t wProductType) { m_wProductType = wProductType; }

    u16_t GetProductID() const { return m_wProductID; }
    void_t SetProductID(u16_t wProductID) { m_wProductID = wProductID; }

    u8_t GetRoleType() const { return m_byRoleType; }
    void_t SetRoleType(u8_t byRoleType) { m_byRoleType = byRoleType; }

    u8_t GetNodeType() const { return m_byNodeType; }
    void_t SetNodeType(u8_t byNodeType) { m_byNodeType = byNodeType; }

    u8_t GetIconType() const { return m_byIconType; }
    void_t SetIconType(u8_t byIconType) { m_byIconType = byIconType; }

    /* Command Class */
private:
    ZwCmdClassMan_p m_pZwCmdClassManager;
    Map<u8_t, ZwCmdClass_p> m_mZwCmdClass;
    bool_t m_boSecured;
public:
    bool_t       IsSupportMultiChannel();
    Value_p      ApplicationCommandHandler(u8_p pbyInBuffer, u8_t byLength);
    ZwCmdClass_p GetZwCmdClass(const u8_t byZwCmdClassID);
    ZwCmdClass_p AddZwCmdClass(const u8_t byZwCmdClassID);
    ZwCmdClass_p RmvZwCmdClass(const u8_t byZwCmdClassID);

    /* Basic Command */
private:
public:
    ZwMessage_p SetLevel(const u8_t byLevel);
    ZwMessage_p SetNode(bool_t boState);
    ZwMessage_p SetNodeOn();
    ZwMessage_p SetNodeOff();

    /* Statistics */
private:
    u32_t m_dwReceivedUnsolicited;
public:
    void_t IncrementUnsolicitedCount() { m_dwReceivedUnsolicited++; }

    /* Endpoint */
private:
    u8_t m_byNbrOfEndpoints;
    ZwEndpoint_p m_pZwEndpoints;
public:
    void_t SetNbrOfEndpoints(u8_t byNbrOfEndpoints);
    u8_t   GetNbrOfEndpoints() const;
    ZwEndpoint_p GetZwEndPoint(u8_t byEndpointID);
    /* Encapsulate */
};

typedef ZwNode  ZwNode_t;
typedef ZwNode* ZwNode_p;

#endif /* !NODE_HPP */
