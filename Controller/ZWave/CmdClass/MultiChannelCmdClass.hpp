#ifndef MULTICHANNEL_CMDCLASS_HPP_
#define MULTICHANNEL_CMDCLASS_HPP_

#include "ZwCmdClass.hpp"

class MultiChannelCmdClass : public ZwCmdClass {
private:
    bool_t  m_boDynamicNumber;
    bool_t  m_boEpIsIdentical;
    u8_t    m_byNumberOfEndpoints;
    u8_t    m_byNumberOfIndividualEndpoints;
    u8_t    m_byNumberOfAggregatedEndpoints;
    u8_t    m_byTransmitOptions;
    MultiChannelCmdClass(u32_t dwHomeID, u8_t byNodeID);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeID, u8_t byNodeID);
    virtual ~MultiChannelCmdClass();

    static const u8_t GetZwCmdClassID();

    virtual u8_t GetMaxVersion() const;
    virtual u8_t GetNumberOfEndpoints() const;
    virtual u8_t GetNumberOfIndividualEndpoints() const;
    virtual u8_t GetNumberOfAggregatedEndpoints() const;

    virtual Value_p HandleMessage(u8_p pInBuffer, u8_t byLength);

    virtual ZwMessage_p SendEndpointGet();
    virtual ZwMessage_p SendCapabilitiyGet(u8_t byEndPoint);
    virtual ZwMessage_p SendEndpointFind(u8_t byGeneric, u8_t bySpecific);

    virtual void_t HandleMultiChannelEndpointReport(u8_p pInBuffer, u8_t byLength);
    virtual void_t HandleMultiChannelCapabilityReport(u8_p pInBuffer, u8_t byLength);
    virtual void_t HandleMultiChannelEnpointFindReport(u8_p pInBuffer, u8_t byLength);
    virtual void_t HandleMultiChannelAggregatedMemberReport(u8_p pInBuffer, u8_t byLength);

    virtual void_t HandleMultiChannelEncap(u8_p pBuffer, u8_t byLength);
};

typedef MultiChannelCmdClass  MultiChannelCmdClass_t;
typedef MultiChannelCmdClass* MultiChannelCmdClass_p;

#endif /* !MULTICHANNEL_CMDCLASS_HPP_ */
