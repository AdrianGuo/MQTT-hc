#ifndef ZWCMD_NVM_HPP_
#define ZWCMD_NVM_HPP_

#include "typedefs.h"
#include "IZwDriver.hpp"
#include "Value.hpp"
#include "ValueByte.hpp"
#include "ValueDword.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"

class ValueZwCmdNvm {
public:
    ValueDword& DwHomeID;
    ValueByte& ByNodeID;

    ValueZwCmdNvm(
        ValueDword& dwHomeID,
        ValueByte& byNodeID
    ) : DwHomeID (dwHomeID),
        ByNodeID (byNodeID) {
    }

    virtual ~ValueZwCmdNvm() { }
 };

typedef ValueZwCmdNvm  ValueZwCmdNvm_t;
typedef ValueZwCmdNvm* ValueZwCmdNvm_p;

class ZwCmdNvm : public IZwDriver {
private:
    ValueZwNode_pp              m_ppValueZwNode;
    ValueZwDriver_p             m_pValueZwDriver;
    ValueZwCmdNvm_p             m_pValueZwCmdNvm;

    ZwCtrllerFunctor_p          m_pZwCtrllerCallbackFunctor;
    JsonSendZwaveSession_p          m_pJsonZwaveSession;

    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleMemoryGetIdResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryGetByteResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryPutByteResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryGetBufferResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryPutBufferRequest(ZwPacket_p pZwPacket);
    void_t HandleMemoryPutBufferResponse(ZwPacket_p pZwPacket);

    void_t HandleGetIdResponse(ZwPacket_p pZwPacket);

    void_t HandleExtReadLongByteResponse(ZwPacket_p pZwPacket);

    void_t HandleExtWriteLongByteResponse(ZwPacket_p pZwPacket);

    void_t HandleExtReadLongBufferResponse(ZwPacket_p pZwPacket);

    void_t HandleExtWriteLongBufferResponse(ZwPacket_p pZwPacket);
public:
    ZwCmdNvm();
    virtual ~ZwCmdNvm();

    virtual void_t RegisterHandlers();

    virtual void_t ShareNodeObject(void_p pObject);
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor);
};

typedef ZwCmdNvm  ZwCmdNvm_t;
typedef ZwCmdNvm* ZwCmdNvm_p;

#endif /* !ZWCMD_NVM_HPP_ */
