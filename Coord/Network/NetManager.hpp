/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
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
 ******************************************************************************/
#ifndef NETCTRLLER_HPP_
#define NETCTRLLER_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "ICtrller.hpp"
#include "RTimer.hpp"
#include "JsonCommand.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Vector.hpp"
#include "Queue.hpp"
#include "SClient.hpp"
#include "JsonNetSession.hpp"

typedef Functor1_t(JsonCommand_p)   HandlerNetCmdFunctor_t;
typedef HandlerNetCmdFunctor_t*     HandlerNetCmdFunctor_p;

class NetManager {
private:
    const static u32_t ConnectTimeout = 2;
    const static u32_t AuthenTimeout  = 5;
    const static u32_t KaliveTimeout  = 60;
    const static u32_t AuthenMax = 10;

    String m_strMacId;
    SClient_p m_pSessionClient;

    RTimer_p m_pNetCtrllerTimer;
    CtrllerFunctor_p m_pICtrllerFunctor;

    threadFunctor_t m_NetCtrllerThreadFunctor;

    TimerFunctor_t m_checkConectionTimerFunctor;
    TimerFunctor_t m_authenRequestTimerFunctor;
    TimerFunctor_t m_keepaliveTimerFunctor;

    u32_t m_dwAuthenRequestCount;
    u32_t m_dwKaliveRequestCount;

    int_t m_iKeepAliveTimerHandle;
    int_t m_iAuthenRequestTimerHandle;
    int_t m_iConnectionTimerHandle;

    bool_t m_boIsAlived;
    bool_t m_boAuthenticated;
    bool_t m_boIsConnected;

    JsonNetSession_p m_pJsonNetSession;

    Queue<JsonCommand_p> m_queNetCtrllerJsonCommand;

    typedef Map<String, HandlerNetCmdFunctor_t> MapHandleFunc;
    MapHandleFunc m_mapHandleFunc;

    void_t RegisterNetSession();

    void_t ProcessAuthenReq();
    void_t ProcessKaliveReq();

    void_t HandleAuthenProcess    (void_p pBuffer);
    void_t HandleKeepAliveProcess (void_p pBuffer);
    void_t HandleConnectProcess(void_p pBuffer);

    void_t StartHandleConnect();
    void_t StopHandleConnect();

    void_t StartHandleAuthen();
    void_t StopHandleAuthen();

    void_t StartHandleKeepAlive();
    void_t StopHandleKeepAlive();
public:
    NetManager(const_char_p cMacId = "");
    virtual ~NetManager();

    virtual void_t Process();
    virtual bool_t SetFunctor(CtrllerFunctor_p pRecvFunctor);
    void_t SetSessionClient(SClient_p pSClient);

    void_t Connect();
    bool_t IsAuthenticated() const;

    void_t HandlerNetCmdAuthRes(JsonCommand_p pJsonCommand);
    void_t HandlerNetCmdKaliveRes(JsonCommand_p pJsonCommand);
    void_t HandlerNetCmdCommon(JsonCommand_p pJsonCommand);
};

typedef NetManager  NetManager_t;
typedef NetManager* NetManager_p;

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
