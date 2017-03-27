#ifndef ZWCMD_BASIC_HPP_
#define ZWCMD_BASIC_HPP_

#include "Typedefs.h"
#include "ValueString.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"


class ValueZwCmdBasic {
public:
    ValueString& libraryVersion;

    ValueZwCmdBasic(
        ValueString& strLibraryVersion
    ) : libraryVersion (strLibraryVersion) {
    }

    virtual ~ValueZwCmdBasic() {}
};

typedef ValueZwCmdBasic  ValueZwCmdBasic_t;
typedef ValueZwCmdBasic* ValueZwCmdBasic_p;

class ZwCmdBasic : public IZwDriver {
private:
    ValueLstNode_t& m_ValueLstNode;
    ValueZwDriver_t& m_ValueZwDriver;
    ValueZwCmdBasic_t& m_ValueZwCmdBasic;

    JsonSendZwaveSession_p m_pJsonZwaveSession;

    ZwDbModel_p m_pZwDbModel;
    HandlerRequest_p m_pHandlerRequest;
    HandlerResponse_p m_pHandlerResponse;

    void_t HandleExploreRequestInclusionResponse(ZwPacket_p pZwPacket);

    void_t HandleExploreRequestExclusionResponse(ZwPacket_p pZwPacket);

    void_t HandleGetBackgroundRssiResponse(ZwPacket_p pZwPacket);

    void_t HandleGetProtocolStatusResponse(ZwPacket_p pZwPacket);

    void_t HandleGetRandomWordResponse(ZwPacket_p pZwPacket);

    void_t HandleRandomResponse(ZwPacket_p pZwPacket);

    void_t HandleRfPowerLevelSetResponse(ZwPacket_p pZwPacket);

    void_t HandleRfPowerLevelGetResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNetWorkUpdateRequest (ZwPacket_p pZwPacket);
    void_t HandleRequestNetWorkUpdateResponse(ZwPacket_p pZwPacket);

    void_t HandleSendNodeInformationRequest (ZwPacket_p pZwPacket);
    void_t HandleSendNodeInformationResponse(ZwPacket_p pZwPacket);

    void_t HandleSendTestFrameRequest (ZwPacket_p pZwPacket);
    void_t HandleSendTestFrameResponse(ZwPacket_p pZwPacket);

    void_t HandleSetRfReceiveModeRequest (ZwPacket_p pZwPacket);
    void_t HandleSetRfReceiveModeResponse(ZwPacket_p pZwPacket);

    void_t HandleTypeLibraryResponse(ZwPacket_p pZwPacket);

    void_t HandleVersionResponse(ZwPacket_p pZwPacket);

    void_t HandleGetTxTimerResponse(ZwPacket_p pZwPacket);

    void_t HandleGetNetworkStatsResponse(ZwPacket_p pZwPacket);

    void_t HandleClearNetworkStatsResponse(ZwPacket_p pZwPacket);
public:
    ZwCmdBasic();
    virtual ~ZwCmdBasic();

    virtual void_t RegisterHandlers();
};

typedef ZwCmdBasic  ZwCmdBasic_t;
typedef ZwCmdBasic* ZwCmdBasic_p;

#endif // !ZWCMD_BASIC_HPP_
