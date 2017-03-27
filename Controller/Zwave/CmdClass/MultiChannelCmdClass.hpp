#ifndef MULTICHANNEL_CMDCLASS_HPP_
#define MULTICHANNEL_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwNode.hpp"
#include "ZwDbModel.hpp"
#include "ZwCmdClass.hpp"
#include "ZwCmdClassMap.hpp"

class MultiChannelCmdClass : public ZwCmdClass {
private:
    bool_t  m_boDynamicNumber;
    bool_t  m_boEpIsIdentical;

    u8_t m_byNumberOfEndpoints;
    u8_t m_byNumberOfIndividualEndpoints;
    u8_t m_byNumberOfAggregatedEndpoints;
    u8_t m_byTransmitOptions;

    ZwDbModel_p m_pDbModel;
    ZwCmdClassMap_t& m_ZwCmdClassMap;

    MultiChannelCmdClass(u32_t dwHomeId, u8_t byNodeId);

    void_t ProcGetCapability(ZwNode_p pZwNode, u8_t byEndpoint);

    ValueDevice_p
    HandleMultiChannelEndpointReport(u8_p pbCommand, u8_t byLength);
    ValueDevice_p
    HandleMultiChannelCapabilityReport(u8_p pbCommand, u8_t byLength);
    ValueDevice_p
    HandleMultiChannelEnpointFindReport(u8_p pbCommand, u8_t byLength);
    ValueDevice_p
    HandleMultiChannelAggregatedMemberReport(u8_p pbCommand, u8_t byLength);
    ValueDevice_p
    HandleMultiChannelEncap(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~MultiChannelCmdClass();

    virtual u8_t GetMaxVersion() const { return MULTI_CHANNEL_VERSION_V4; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_MULTI_CHANNEL_V4; }
    static const String GetZwCmdClassName() { return "MULTI_CHANNEL"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    u8_t GetNumberOfEndpoints() const;
    u8_t GetNumberOfIndividualEndpoints() const;
    u8_t GetNumberOfAggregatedEndpoints() const;

    ZwMessage_p Encapsulate(u8_p pData, u8_t bLen, u8_t bSrc, u8_t bDes);
    ZwMessage_p GetEndpoint();
    ZwMessage_p GetCapabilitiy(u8_t byEndPoint);
    ZwMessage_p EndpointFind(u8_t byGeneric, u8_t bySpecific);
};

typedef MultiChannelCmdClass  MultiChannelCmdClass_t;
typedef MultiChannelCmdClass* MultiChannelCmdClass_p;

#endif /* !MULTICHANNEL_CMDCLASS_HPP_ */
