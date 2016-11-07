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
#include <JsonZwaveSession.hpp>
#include "typedefs.h"
#include "JsonCommand.hpp"
#include "ZwMessage.hpp"
#include "ICtrller.hpp"
#include "ValueJsonChecker.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "ZwDriver.hpp"
#include "Event.hpp"

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
    Queue<ZwMessage_p> m_queSendZwMsg;
    ValueJsonChecker m_valueJsonChecker;

    ZwDriver_p m_pZwDriver;
    LThread_p m_pZwCtrllerThread;
    Locker_p m_pZwCtrllerLocker;
    ICtrllerFunctor_p m_pICtrllerFunctor;

    JsonRecvZwaveSession_p m_pJsonRecvZwaveSession;
    JsonSendZwaveSession_p m_pJsonSendZwaveSession;

    threadFunctor_t m_ZwCtrllerThreadFunctor;

    bool_t m_boAddNodeCmdSend;
    bool_t m_boDelNodeCmdSend;
    bool_t m_boAddNodeResReceived;
    bool_t m_boDelNodeResReceived;

    Event_t m_evWaitMsgSignal;
    ZwCtrllerFunctor_t m_ZwCtrllerFunctor;

    typedef Map<String, HandlerZwCmdFunctor_t> MapHandlerFunctor;
    MapHandlerFunctor m_mapHandlerFunctor;

    typedef Map<ZwMessage::Command, ProcZwCmdFunctor_t> MapProcFunctor;
    MapProcFunctor m_mapProcFunctor;

    void_t RegisterHandler(String strJsonCommand, HandlerZwCmdFunctor_t funcTor);
    void_t ProcessHandler(JsonCommand_p pJsonCommand);
    void_t RegisterProc(ZwMessage::Command zwCommand, ProcZwCmdFunctor_t funcTor);
    void_t ProcessProc(ZwMessage_p pZwMessage);

    void_t HandlerZwCmdAdd(JsonCommand_p pJsonCommand);
    void_t HandlerZwCmdDel(JsonCommand_p pJsonCommand);
    void_t HandlerZwCmdReset(JsonCommand_p pJsonCommand);
    void_t HandlerZwCmdRestart(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdSet(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdGet(JsonCommand_p pJsonCommand);

    void_t ProcZwCmdAddNode(ZwMessage_p pZwMessage);
    void_t ProcZwCmdDelNode(ZwMessage_p pZwMessage);
    void_t ProcZwCmdRestart(ZwMessage_p pZwMessage);
    void_t ProcZwCmdGetEndpoints (ZwMessage_p pZwMessage);
    void_t ProcZwCmdGetCapability(ZwMessage_p pZwMessage);
    void_t ProcZwCmdSendData(ZwMessage_p pZwMessage);

    void_t RegisterJsonMessageInform();
public:
    ZwCtrller(ZwDriver_p pZwDriver = NULL);
    virtual ~ZwCtrller();

    virtual void_t Process();

    void_t ZwCtrllerSendFunctor();
    bool_t ICtrllerRecvFunctor(ICtrllerFunctor_p pRecvFunctor);

    void_t Start();

    bool_t LoadCmdClass(JsonCommand_p pJsonCommand);

    void_t PushJsonCommand(EvAction evAction, void_p pBuffer);
    void_p ZwCtrlllerThreadProc(void_p pInBuffer);

    void_t HandleSendCmdGetCapability(u8_t byNodeID, u8_t byNumberOfEndpoints);

    void_t IniZwCtrller();
};

typedef ZwCtrller  ZwCtrller_t;
typedef ZwCtrller* ZwCtrller_p;


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ProcZwCmd {

};

#endif /* !ZWCTRLLER_HPP_ */
