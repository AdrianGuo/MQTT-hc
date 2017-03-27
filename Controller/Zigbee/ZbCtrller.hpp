/*
 * ZbCtrller.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBCTRLLER_HPP_
#define ZBCTRLLER_HPP_

#include "Typedefs.h"
#include "ICtrller.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "IZbDriver.hpp"
#include "ZbDriver.hpp"
#include "Event.hpp"

class ZbCtrller : public ICtrller {
private:
    ZbDriver_p          	m_pZbDriver;
    Queue<JsonCommand_p>& 	m_queSerRecvMsg;
    LThread_p           	m_pZbCtrllerThread;
    Locker_p            	m_pZbCtrllerLocker;
    CtrllerFunctor_p    	m_pCtrllerFunctor;

    threadFunctor_t     	m_ZbCtrllerThreadFunctor;
    ZbCtrllerFunctor_t  	m_ZbCtrllerFunctor;
    Event_t             	m_evWaitMsgSignal;

public:
    ZbCtrller(const_char_p chPortname);
    virtual ~ZbCtrller();

    void_t CtrllerSendFunctor();
    void_t CtrllerRecvFunctor(CtrllerFunctor_p pRecvFunctor);

    void_t Connect();
    void_t Start();

    void_t ProcessHandler(JsonCommand_p pJsonCommand);

    void_t PushJsonCommand(EvAct evAction, void_p pBuffer);
    void_p ZbCtrlllerThreadProc(void_p pInBuffer);

    void_t Init();
};

typedef ZbCtrller  ZbCtrller_t;
typedef ZbCtrller* ZbCtrller_p;

#endif /* ZBCTRLLER_HPP_ */
