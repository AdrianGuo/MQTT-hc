#ifndef ZWCMD_NVM_HPP_
#define ZWCMD_NVM_HPP_

#include "typedefs.h"
#include "ValueByte.hpp"
#include "ValueDword.hpp"
#include "HandlerRequest.hpp"
#include "HandlerResponse.hpp"
#include "JsonZwaveSession.hpp"
#include "ZwDbModel.hpp"
#include "IZwDriver.hpp"
#include "ValueLstNode.hpp"

class ValueZwCmdNvm {
public:
    ValueDword& homeId;
    ValueByte&  nodeId;

    ValueZwCmdNvm(
        ValueDword& dwHomeId,
        ValueByte&  byNodeId
    ) : homeId (dwHomeId),
        nodeId (byNodeId) {
    }

    virtual ~ValueZwCmdNvm() { }
 };

typedef ValueZwCmdNvm  ValueZwCmdNvm_t;
typedef ValueZwCmdNvm* ValueZwCmdNvm_p;

class ZwCmdNvm : public IZwDriver {
private:
    ValueLstNode_t&         m_ValueLstNode;
    ValueZwDriver_t&        m_ValueZwDriver;
    ValueZwCmdNvm_p         m_pValueZwCmdNvm;

    JsonSendZwaveSession_p  m_pJsonZwaveSession;

    ZwDbModel_p                 m_pZwDbModel;
    HandlerRequest_p            m_pHandlerRequest;
    HandlerResponse_p           m_pHandlerResponse;

    void_t HandleMemoryGetIdResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryGetByteResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryPutByteResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryGetBufferResponse(ZwPacket_p pZwPacket);

    void_t HandleMemoryPutBufferRequest (ZwPacket_p pZwPacket);
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
};

typedef ZwCmdNvm  ZwCmdNvm_t;
typedef ZwCmdNvm* ZwCmdNvm_p;

#endif /* !ZWCMD_NVM_HPP_ */
