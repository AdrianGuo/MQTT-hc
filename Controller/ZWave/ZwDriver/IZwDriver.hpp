#ifndef IZWDRIVER_HPP_
#define IZWDRIVER_HPP_

#include "typedefs.h"
#include "ZwNode.hpp"

#include "Functor.hpp"

#include "Value.hpp"
#include "ValueByte.hpp"
#include "ValueBool.hpp"
#include "ValueEvent.hpp"
#include "ValueZwNode.hpp"

class ValueZwDriver {
public:
    ValueByte& ByExpectedCallbackID;
    ValueByte& ByExpectedNodeID;
    ValueByte& ByExpectedFunctionID;
    ValueByte& ByExpectedCmdClassID;
    ValueEvent& EvPacketSignal;

    ValueZwDriver(
        ValueByte& byExpectedCallbackID,
        ValueByte& byExpectedNodeID,
        ValueByte& byExpectedFunctionID,
        ValueByte& byExpectedCmdClassID,
        ValueEvent& EvPacketSignal
        ) : ByExpectedCallbackID (byExpectedCallbackID),
            ByExpectedNodeID (byExpectedNodeID),
            ByExpectedFunctionID (byExpectedFunctionID),
            ByExpectedCmdClassID (byExpectedCmdClassID),
            EvPacketSignal (EvPacketSignal) {
    }

    virtual ~ValueZwDriver() {}
};

typedef ValueZwDriver  ValueZwDriver_t;
typedef ValueZwDriver* ValueZwDriver_p;

typedef enum {
    None = 0,
    Set,
    Reset,
    Wait,
    Pushback,
    Count
} EvAction;


typedef enum ZwCtrllerState {
    ZWCTRLLER_STATE_NORMAL = 0,
    ZWCTRLLER_STATE_STARTING,
    ZWCTRLLER_STATE_CANCEL,
    ZWCTRLLER_STATE_WAITING,
    ZWCTRLLER_STATE_SLEEPING,
    ZWCTRLLER_STATE_INPROGRESS,
    ZWCTRLLER_STATE_COMPLETED,
    ZWCTRLLER_STATE_FAILED,
    ZWCTRLLER_STATE_NODEOK,
    ZWCTRLLER_STATE_NODEFAILED,
    ZWCTRLLER_STATE_COUNT
} ZwCtrllerState_t;

typedef Functor2_t(EvAction, void_p)    ZwCtrllerFunctor_t;
typedef ZwCtrllerFunctor_t*             ZwCtrllerFunctor_p;

class IZwDriver {
public:
    IZwDriver() {}
    virtual ~IZwDriver() {}

    virtual bool_t ProcRecvMessage(void_p pInBuffer) { return FALSE; }
    virtual void_t ShareCommonObject(void_p pObject) {}
    virtual void_t ShareSpecificObject(void_p pObject) {}
    virtual void_t ShareNodeObject(void_p pObject) {}
    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor) {}
};

typedef IZwDriver  IZwDriver_t;
typedef IZwDriver* IZwDriver_p;

#endif /* !IZWDRIVER_HPP_ */
