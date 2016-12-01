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
#include "JsonCommand.hpp"
#include "ICtrller.hpp"
#include "Locker.hpp"
#include "SClient.hpp"
#include "SServer.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

class HCCtrller {
private:
    Queue<JsonCommand_p> m_queJsonCommand;
    Queue<JsonCommand_p> m_quePedingJsonCommand;

    Locker_p m_pHCCtrllerLocker;
    SClient_p m_pSClient;
    SServer_p m_pSServer;

    Vector<ICtrller_p> m_vecCtrller;
    HCCtrllerFunctor_t m_HCCtrllerFunctor;
    ICtrllerFunctor_t  m_ICtrllerFunctor;

public:
    HCCtrller(SClient_p pSClient = NULL, SServer_p pSServer = NULL);
    virtual ~HCCtrller();

    void_t SendFunctor();
    void_t AddCtrller(ICtrller_p pCtrller);
    bool_t RecvCommandFromSession(JsonCommand_p pJsonCommand);
    bool_t RecvCommandFromModules(JsonCommand_p pJsonCommand);
    void_t Process();
};

typedef HCCtrller  HCCtrller_t;
typedef HCCtrller* HCCtrller_p;

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
