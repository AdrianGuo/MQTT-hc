/*
 * ZbCtrller.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBCTRLLER_HPP_
#define ZBCTRLLER_HPP_

#include "typedefs.h"
#include "JsonCommand.hpp"
#include "ZbMessage.hpp"
#include "ICtrller.hpp"
#include "ValueJsonChecker.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "IZbDriver.hpp"
#include "ZbDriver.hpp"
#include "Event.hpp"
#include "JsonZigbeeSession.hpp"

typedef Functor1_t(JsonCommand_p)   HandlerZbCmdFunctor_t;
typedef HandlerZbCmdFunctor_t*      HandlerZbCmdFunctor_p;

typedef Functor1_t(ZbMessage_p)     ProcZbCmdFunctor_t;
typedef ProcZbCmdFunctor_t*         ProcZbCmdFunctor_p;

class ZbCtrller : public ICtrller {
private:
    Queue<ZbMessage_p>  m_queSendZbMsg;
    ValueJsonChecker    m_valueJsonChecker;

    ZbDriver_p          m_pZbDriver;
    LThread_p           m_pZbCtrllerThread;
    Locker_p            m_pZbCtrllerLocker;
    ICtrllerFunctor_p   m_pICtrllerFunctor;

    JsonSendZigbeeSession_p m_pJsonZigbeeSession;
    threadFunctor_t     m_ZbCtrllerThreadFunctor;
    ZbCtrllerFunctor_t  m_ZbCtrllerFunctor;
    Event_t             m_evWaitMsgSignal;

    typedef Map<String, HandlerZbCmdFunctor_t>          MapHandlerFunctor;
    MapHandlerFunctor   m_mapHandlerFunctor;

    typedef Map<ZbMessage::Command, ProcZbCmdFunctor_t> MapProcFunctor;
    MapProcFunctor      m_mapProcFunctor;

    void_t RegisterHandler(String strJsonCommand, HandlerZbCmdFunctor_t funcTor);
    void_t ProcessHandler(JsonCommand_p pJsonCommand);
    void_t RegisterProcess(ZbMessage::Command ZbCommand, ProcZbCmdFunctor_t funcTor);
    void_t ProcessProcess(ZbMessage_p pZbMessage);

    void_t HandlerCmdAdd(JsonCommand_p pJsonCommand);
    void_t HandlerCmdDel(JsonCommand_p pJsonCommand);
    void_t HandlerCmdSet(JsonCommand_p pJsonCommand);
    void_t HandlerCmdGet(JsonCommand_p pJsonCommand);
    void_t HandlerCmdInfo(JsonCommand_p pJsonCommand);
    void_t HandlerCmdReset(JsonCommand_p pJsonCommand);
    void_t HandlerCmdRestart(JsonCommand_p pJsonCommand);
    void_t HandlerCmdIrLearn(JsonCommand_p pJsonCommand);
    void_t HandlerCmdIrSet(JsonCommand_p pJsonCommand);
    void_t HandlerCmdIrDel(JsonCommand_p pJsonCommand);
    void_t HandlerCmdIrEna(JsonCommand_p pJsonCommand);

    void_t HandlerCmdManualRemove(JsonCommand_p pJsonCommand);

    void_t ProcCmdAdd(ZbMessage_p pZbMessage);
    void_t ProcCmdDel(ZbMessage_p pZbMessage);
    void_t ProcCmdSet(ZbMessage_p pZbMessage);
    void_t ProcCmdGet(ZbMessage_p pZbMessage);
    void_t ProcCmdReset(ZbMessage_p pZbMessage);
    void_t ProcCmdRestart(ZbMessage_p pZbMessage);
    void_t ProcCmdInfo(ZbMessage_p pZbMessage);
    void_t ProcCmdIrLearn(ZbMessage_p pZbMessage);
    void_t ProcCmdIrSet(ZbMessage_p pZbMessage);
    void_t ProcCmdIrDel(ZbMessage_p pZbMessage);
    void_t ProcCmdIrEna(ZbMessage_p pZbMessage);

    void_t ProcCmdManualRemove(ZbMessage_p pZbMessage);

    void_t RegisterJsonMessageInform();
public:
    ZbCtrller(ZbDriver_p pZbDriver = NULL);
    virtual ~ZbCtrller();

    virtual void_t Process();

    void_t ZbCtrllerSendFunctor();
    bool_t ICtrllerRecvFunctor(ICtrllerFunctor_p pRecvFunctor);

    void_t Start();

    bool_t LoadCmdClass(JsonCommand_p pJsonCommand);

    void_t PushJsonCommand(EvAction evAction, void_p pBuffer);
    void_p ZbCtrlllerThreadProc(void_p pInBuffer);

    void_t IniZbCtrller();
};

typedef ZbCtrller  ZbCtrller_t;
typedef ZbCtrller* ZbCtrller_p;

#endif /* ZBCTRLLER_HPP_ */
