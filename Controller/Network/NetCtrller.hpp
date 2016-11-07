/*******************************************************************************
*
* Copyright (c) 2016
* Lumi, JSC.
* All Rights Reserved
*
*
* Description:      Include file for application
*
* Author:
*
* Last Changed By:  TrungTQ
* Revision:         Revision: 1.0
* Last Changed:     Date: 2016-08-08 10:00:00 (Mon, 08 Aug 2016)
*
* Note:             
*
*******************************************************************************/
#ifndef NETCTRLLER_HPP_
#define NETCTRLLER_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "ICtrller.hpp"
#include "RTimer.hpp"
#include "JsonCommand.hpp"
#include "ICtrller.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "SClient.hpp"
#include "ValueJsonChecker.hpp"
#include "JsonNetSession.hpp"

typedef Functor1_t(JsonCommand_p)   HandlerNetCmdFunctor_t;
typedef HandlerNetCmdFunctor_t*     HandlerNetCmdFunctor_p;

class NetCtrller : public ICtrller {
private:
    const static u32_t ConnectTimeout = 2;
    const static u32_t AuthenTimeout  = 5;
    const static u32_t KaliveTimeout  = 60;

    ValueJsonChecker m_valueJsonChecker;
    SClient_p m_pSessionClient;

    RTimer_p m_pNetCtrllerTimer;
    LThread_p m_pNetCtrllerThread;
    Locker_p m_pNetCtrllerLocker;
    ICtrllerFunctor_p m_pICtrllerFunctor;

    threadFunctor_t m_NetCtrllerThreadFunctor;

    timerFunctor_t m_checkConectionTimerFunctor;
    timerFunctor_t m_authenRequestTimerFunctor;
    timerFunctor_t m_keepaliveTimerFunctor;

    u8_t  m_byAuthenRequestCount;
    int_t m_iConnectionTimerHandle;
    int_t m_iAuthenRequestTimerHandle;
    int_t m_iKeepAliveTimerHandle;

    bool_t m_boIsAlived;
    bool_t m_boAuthenticated;
    bool_t m_boIsConnected;

    String m_strMacID;

    Queue<JsonCommand_p> m_queNetCtrllerJsonCommand;

    JsonNetSession_p m_pJsonNetSession;

    typedef Map<String, HandlerNetCmdFunctor_t> MapHandleFunc;
    MapHandleFunc m_mapHandleFunc;

    void_t RegisterHandler(String strJsonCommand, HandlerNetCmdFunctor_t funcTor);
    void_t ProcessHandler(JsonCommand_p pJsonCommand);
public:
    NetCtrller(SClient_p pSClient = NULL, const_char_p cMacID = "");
    virtual ~NetCtrller();

    void_p NetCtrlllerThreadProc(void_p pBuffer);

    virtual void_t Process();
    virtual bool_t ICtrllerRecvFunctor(ICtrllerFunctor_p pRecvFunctor);

    void_t InitProcess();
    void_t Start();
    void_t RegisterJsonMessageInform();
    bool_t LoadCmdClass(JsonCommand_p pJsonCommand);

    void_t HandlerNetCmdAuthRes(JsonCommand_p pJsonCommand);
    void_t HandlerNetCmdKaliveRes(JsonCommand_p pJsonCommand);
    void_t HandlerNetCmdCommon(JsonCommand_p pJsonCommand);

    void_t ProcessAuthenReq();
    void_t ProcessKaliveReq();

    void_t HandleAuthenProcess(void_p pBuffer);
    void_t HandleKeepAliveProcess(void_p pBuffer);
    void_t HandleConnectionProcess(void_p pBuffer);
};

typedef NetCtrller  NetCtrller_t;
typedef NetCtrller* NetCtrller_p;

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


#endif /* !NETCTRLLER_HPP_ */
