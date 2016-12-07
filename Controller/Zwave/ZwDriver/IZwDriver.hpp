#ifndef IZWDRIVER_HPP_
#define IZWDRIVER_HPP_

#include "typedefs.h"
#include "Functor.hpp"

#include "Value.hpp"
#include "ValueByte.hpp"
#include "ValueBool.hpp"
#include "ValueEvent.hpp"

class ValueZwDriver {
public:
    ValueByte&  expectedCbakId;
    ValueByte&  expectedNodeId;
    ValueByte&  expectedFuncId;
    ValueByte&  expectedCClaId;
    ValueByte&  expectedEndpId;
    ValueEvent& packetSignal;

    ValueZwDriver(
        ValueByte&  byExpectedCallbackId,
        ValueByte&  byExpectedNodeId,
        ValueByte&  byExpectedFunctionId,
        ValueByte&  byExpectedCmdClassId,
        ValueByte&  byExpectedEndpointId,
        ValueEvent& evPacketSignal
        ) : expectedCbakId (byExpectedCallbackId),
            expectedNodeId (byExpectedNodeId),
            expectedFuncId (byExpectedFunctionId),
            expectedCClaId (byExpectedCmdClassId),
            expectedEndpId (byExpectedEndpointId),
            packetSignal   (evPacketSignal) {
    }

    virtual ~ValueZwDriver() {}
};

typedef ValueZwDriver  ValueZwDriver_t;
typedef ValueZwDriver* ValueZwDriver_p;

typedef enum {
    None = 0,
    SSet,
    Reset,
    Wait,
    Pushback,
    Count
} EvAction;

typedef Functor2_t(EvAction, void_p)    ZwCtrllerFunctor_t;
typedef ZwCtrllerFunctor_t*             ZwCtrllerFunctor_p;

class IZwDriver {
protected:
    ZwCtrllerFunctor_p m_pCallbackFunctor;
public:
    IZwDriver(
        ZwCtrllerFunctor_p pCallbackFunctor = NULL
    ) : m_pCallbackFunctor (pCallbackFunctor) {}
    virtual ~IZwDriver() {}

    virtual void_t SetCallbackFunctor(ZwCtrllerFunctor_p pCallbackFunctor) {
        m_pCallbackFunctor = pCallbackFunctor;
    }

    virtual void_t ProcessFunctor(EvAction event, void_p pBuffer) {
        if (m_pCallbackFunctor != NULL) {
            (*m_pCallbackFunctor) (event, pBuffer);
        }
    }
};

typedef IZwDriver  IZwDriver_t;
typedef IZwDriver* IZwDriver_p;

#endif /* !IZWDRIVER_HPP_ */
