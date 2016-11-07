#ifndef ZWCMD_BASIC_HPP_
#define ZWCMD_BASIC_HPP_

#include "typedefs.h"
#include "ValueString.hpp"
#include "IZwDriver.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwCmdBasic {
public:
    ValueString& StrLibraryVersion;

    ValueZwCmdBasic(
        ValueString& strLibraryVersion
    ) : StrLibraryVersion (strLibraryVersion) {
    }

    virtual ~ValueZwCmdBasic() {}
};

typedef ValueZwCmdBasic  ValueZwCmdBasic_t;
typedef ValueZwCmdBasic* ValueZwCmdBasic_p;

class ZwCmdBasic : public IZwDriver {
private:
    ValueZwNode_pp              m_ppValueZwNode;
    ValueZwDriver_p             m_pValueZwDriver;
    ValueZwCmdBasic_p           m_pValueZwCmdBasic;

    ZwCtrllerFunctor_p          m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p          m_pJsonZwaveSession;

    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleExploreRequestInclusionResponse(ZwPacket_p pZwPacket);

    void_t HandleExploreRequestExclusionResponse(ZwPacket_p pZwPacket);

    void_t HandleGetBackgroundRssiResponse(ZwPacket_p pZwPacket);

    void_t HandleGetProtocolStatusResponse(ZwPacket_p pZwPacket);

    void_t HandleGetRandomWordResponse(ZwPacket_p pZwPacket);

    void_t HandleRandomResponse(ZwPacket_p pZwPacket);

    void_t HandleRFPowerLevelSetResponse(ZwPacket_p pZwPacket);

    void_t HandleRFPowerLevelGetResponse(ZwPacket_p pZwPacket);

    void_t HandleRequestNetWorkUpdateRequest(ZwPacket_p pZwPacket);
    void_t HandleRequestNetWorkUpdateResponse(ZwPacket_p pZwPacket);

    void_t HandleSendNodeInformationRequest(ZwPacket_p pZwPacket);
    void_t HandleSendNodeInformationResponse(ZwPacket_p pZwPacket);

    void_t HandleSendTestFrameRequest(ZwPacket_p pZwPacket);
    void_t HandleSendTestFrameResponse(ZwPacket_p pZwPacket);

    void_t HandleSetRFReceiveModeSend(ZwPacket_p pZwPacket);
    void_t HandleSetRFReceiveModeResponse(ZwPacket_p pZwPacket);

    void_t HandleTypeLibraryResponse(ZwPacket_p pZwPacket);

    void_t HandleVersionResponse(ZwPacket_p pZwPacket);

    void_t HandleGetTxTimerResponse(ZwPacket_p pZwPacket);

    void_t HandleGetNetworkStatsResponse(ZwPacket_p pZwPacket);

    void_t HandleClearNetworkStatsResponse(ZwPacket_p pZwPacket);
public:
    ZwCmdBasic();
    virtual ~ZwCmdBasic();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdBasic  ZwCmdBasic_t;
typedef ZwCmdBasic* ZwCmdBasic_p;

#endif // !ZWCMD_BASIC_HPP_
