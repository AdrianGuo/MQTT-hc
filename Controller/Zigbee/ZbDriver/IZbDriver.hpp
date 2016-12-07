/*
 * IZbDriver.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_ZBDRIVER_IZBDRIVER_HPP_
#define CONTROLLER_ZIGBEE_ZBDRIVER_IZBDRIVER_HPP_

#ifndef IZBDRIVER_HPP_
#define IZBDRIVER_HPP_

#include "typedefs.h"

#include "Functor.hpp"

#include "Value.hpp"
#include "ValueByte.hpp"
#include "ValueBool.hpp"

typedef enum {
    EA_None = 0,
    EA_Set,
    EA_Reset,
    EA_Wait,
    EA_Pushback,
    EA_Count
} EvAct;



typedef enum ZbCtrllerState {
    ZBCTRLLER_STATE_NORMAL = 0,
    ZBCTRLLER_STATE_STARTING,
    ZBCTRLLER_STATE_CANCEL,
    ZBCTRLLER_STATE_WAITING,
    ZBCTRLLER_STATE_SLEEPING,
    ZBCTRLLER_STATE_INPROGRESS,
    ZBCTRLLER_STATE_COMPLETED,
    ZBCTRLLER_STATE_FAILED,
    ZBCTRLLER_STATE_NODEOK,
    ZBCTRLLER_STATE_NODEFAILED,
    ZBCTRLLER_STATE_COUNT
} ZbCtrllerState_t;

typedef Functor2_t(EvAct, void_p)  ZbCtrllerFunctor_t;
typedef ZbCtrllerFunctor_t*             ZbCtrllerFunctor_p;

class IZbDriver {
public:
    IZbDriver() {}
    virtual ~IZbDriver() {}

    virtual bool_t ProcRecvMessage(void_p pInBuffer) { return FALSE; }
//    virtual void_t ShareCommonObject(void_p pObject) {}
//    virtual void_t ShareSpecificObject(void_p pObject) {}
//    virtual void_t ShareNodeObject(void_p pObject) {}
//    virtual void_t ShareCallbackFunctor(void_p pCtrllerCallbackFunctor) {}
};

typedef IZbDriver  IZbDriver_t;
typedef IZbDriver* IZbDriver_p;

#endif /* !IZBDRIVER_HPP_ */


#endif /* CONTROLLER_ZIGBEE_ZBDRIVER_IZBDRIVER_HPP_ */
