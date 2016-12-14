/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File name:        ZwCtrller.hpp
 *
 * Description:      Include file for application
 *
 * Author:           TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         Revision: 1.0
 * Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
 *
 * Note:
 *
 ******************************************************************************/
#ifndef ZWCTRLLER_HPP_
#define ZWCTRLLER_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/

#include "typedefs.h"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "Event.hpp"
#include "ICtrller.hpp"
#include "JsonCommand.hpp"

#include "JsonZwaveSession.hpp"
#include "ZwDeviceValueMan.hpp"
#include "ZwMessage.hpp"
#include "ZwDriver.hpp"
#include "RTimer.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
typedef Functor1_t(JsonCommand_p)   HandlerZwCmdFunctor_t;
typedef HandlerZwCmdFunctor_t*      HandlerZwCmdFunctor_p;

typedef Functor1_t(ZwMessage_p)     ProcZwCmdFunctor_t;
typedef ProcZwCmdFunctor_t*         ProcZwCmdFunctor_p;

class ZwCtrller : public ICtrller {
private:
    const static i32_t AddNodeTimeout = 40000; // ms
    const static i32_t RmvNodeTimeout = 40000;

    Queue<ZwMessage_p> m_queSendZwMsg;

    ZwDriver_t m_ZwDriver;

    ZwDeviceValueMan_p m_pZwDeviceValueMan;

    LThread_p   m_pZwCtrllerThread;
    Locker_p    m_pZwCtrllerLocker;

    CtrllerFunctor_p m_pCtrllerFunctor;
    threadFunctor_t m_ZwCtrllerThreadFunctor;

    bool_t m_boAddNodeSend;
    bool_t m_boDelNodeCmdSend;
    bool_t m_boAddNodeRecv;
    bool_t m_boDelNodeResReceived;

    JsonRecvZwaveSession_p m_pJsonRecvZwaveSession;
    JsonSendZwaveSession_p m_pJsonSendZwaveSession;
    JsonSelfZwaveSession_p m_pJsonSelfZwaveSession;

    Event_t m_evWaitMsgSignal;
    ZwCtrllerFunctor_t m_ZwCtrllerFunctor;

    typedef Map<String, HandlerZwCmdFunctor_t> MapHandlerFunctor;
    MapHandlerFunctor m_mapHandlerFunctor;

    typedef Map<ZwMessage::Command, ProcZwCmdFunctor_t> MapProcFunctor;
    MapProcFunctor m_mapProcFunctor;

    void_t RegisterHandler(String strJsonCommand, HandlerZwCmdFunctor_t funcTor);
    void_t RegisterProc(ZwMessage::Command zwCommand, ProcZwCmdFunctor_t funcTor);
    void_t ProcessProc(ZwMessage_p pZwMessage);

    void_t HandlerZwaveCmdAdd(JsonCommand_p pJsonCommand);
    void_t HandlerZwaveCmdDel(JsonCommand_p pJsonCommand);
    void_t HandlerZwaveCmdRst(JsonCommand_p pJsonCommand);
    void_t HandlerZwaveCmdRsa(JsonCommand_p pJsonCommand);
    void_t HandlerZwaveCmdSet(JsonCommand_p pJsonCommand);
    void_t HandlerZwaveCmdGet(JsonCommand_p pJsonCommand);

    void_t ProcZwaveCmdAddNode(ZwMessage_p pZwMessage);
    void_t ProcZwaveCmdDelNode(ZwMessage_p pZwMessage);
    void_t ProcZwaveCmdRestart(ZwMessage_p pZwMessage);
    void_t ProcZwCmdSendData  (ZwMessage_p pZwMessage);

    void_t RegisterZwSession();
    void_t RegisterHandler();
    void_t RegisterProcess();

    void_t ProcSelfStopAddNode();
    void_t ProcSelfStopRmvNode();

public:
    static void_t MapValueDevice();

    ZwCtrller(const_char_p portname);
    virtual ~ZwCtrller();

    virtual void_t Process();

    void_t CtrllerSendFunctor();
    void_t CtrllerRecvFunctor(CtrllerFunctor_p pRecvFunctor);

    void_t Debug();
    void_t Start();
    void_t Connect();
    void_t ProcessHandler(JsonCommand_p pJsonCommand);
    void_t PushJsonCommand(EvAction evAction, void_p pBuffer);

    void_p ZwCtrlllerThreadProc(void_p pInBuffer);

    void_t Init();
};

typedef ZwCtrller  ZwCtrller_t;
typedef ZwCtrller* ZwCtrller_p;

#endif /* !ZWCTRLLER_HPP_ */
