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


/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#ifndef HCCTRLLER_HPP_
#define HCCTRLLER_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "Vector.hpp"
#include "Queue.hpp"
#include "Functor.hpp"
#include "JsonCommand.hpp"
#include "ICtrller.hpp"
#include "Locker.hpp"
#include "SClient.hpp"
#include "SServer.hpp"
#include "DbModel.hpp"
#include "DbManager.hpp"
#include "DevManager.hpp"
#include "NetManager.hpp"
#include "RuManager.hpp"
#include "ZwCtrller.hpp"
#include "ZbCtrller.hpp"
#include "RuleCtrller.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef Functor1_t(JsonCommand_p)   HandlerJsonCmdFunctor_t;
typedef HandlerJsonCmdFunctor_t*    HandlerJsonCmdFunctor_p;

class HcCtrller {
private:
    DbManager_t  m_DbManager;
    DevManager_t m_DevManager;
    NetManager_t m_NetManager;
    RuManager_t  m_RuManager;

    SClient_t    m_SessionClient;
    SServer_t    m_SessionServer;

    ZwCtrller_p  m_pZwCtrller;
    ZbCtrller_p  m_pZbCtrller;
    RuleCtrller_p m_pRuleCtrller;

    Locker_p     m_pHCCtrllerLocker;
    bool_t       m_boIsDebug;

    Queue<JsonCommand_p> m_queJsonCommand;

    typedef Map<String, HandlerJsonCmdFunctor_t> MapHandlerFunctor;
    MapHandlerFunctor m_mapHandlerFunctor;

    HCCtrllerFunctor_t m_HCCtrllerFunctor;
    CtrllerFunctor_t m_CtrllerFunctor;

    DbModel_p m_pDbModelDb;

    void_t RegisterHandler(String strJsonCommand, HandlerJsonCmdFunctor_t funcTor);
    void_t RegisterHandler();
    void_t ProcessHandler(JsonCommand_p pJsonCommand);
public:
    HcCtrller(const_char_p ipname = "",
              int_t ipport = -1,
              const_char_p cMacID = "",
              int_t openedport = 1235);
    virtual ~HcCtrller();

    void_t Debug();
    void_t Connect();
    void_t Serve();
    void_t AddZwCtrller(ZwCtrller_p pZwCtrller);
    void_t AddZbCtrller(ZbCtrller_p pZbCtrller);
    void_t AddRuleCtrller(RuleCtrller_p pRuleCtrller);

    bool_t RecvCommandFromSession(JsonCommand_p pJsonCommand);
    bool_t RecvCommandFromModules(JsonCommand_p pJsonCommand);
    void_t Process();
};

typedef HcCtrller  HcCtrller_t;
typedef HcCtrller* HcCtrller_p;

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

#endif /* HCCTRLLER_HPP_ */
