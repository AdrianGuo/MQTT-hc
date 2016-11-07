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
#ifndef ICTRLLER_HPP_
#define ICTRLLER_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "JsonCommand.hpp"
#include "Functor.hpp"

/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
typedef Functor1Ret_t(JsonCommand_p, bool_t)         ICtrllerFunctor_t;
typedef ICtrllerFunctor_t*                           ICtrllerFunctor_p;

class ICtrller {
public:
    ICtrller() {}
    virtual ~ICtrller() { }

    virtual void_t Process() = 0;
    virtual void_t RegisterJsonMessageInform() = 0;
    virtual bool_t LoadCmdClass(JsonCommand_p pJsonCommand) = 0;
    virtual bool_t ICtrllerRecvFunctor(ICtrllerFunctor_p pRecvFunctor) = 0;
};

typedef ICtrller ICtrller_t;
typedef ICtrller* ICtrller_p;

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



#endif /* ICTRLLER_HPP_ */
