/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DevManager.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 09:46:00
 *
 ******************************************************************************/
#ifndef DEVMANAGER_HPP_
#define DEVMANAGER_HPP_

#include "typedefs.h"
#include "DbModel.hpp"
#include "Functor.hpp"
#include "ICtrller.hpp"
#include "JsonCommand.hpp"
#include "JsonDevSession.hpp"

typedef Functor1_t(JsonCommand_p)   HandlerDevCmdFunctor_t;
typedef HandlerDevCmdFunctor_t*     HandlerDevCmdFunctor_p;

class DevManager {
public:
    DevManager();
    ~DevManager();

    bool_t SetFunctor(CtrllerFunctor_p pRecvFunctor);
    void_t PushJsonCommand(JsonCommand_p pJsonCommand);
private:
    DbModel_p m_pDbModelDb;
    JsonRecvDevSession_p m_pJsonRecvDevSession;
    JsonSendDevSession_p m_pJsonSendDevSession;
    CtrllerFunctor_p m_pCtrllerFunctor;

    void_t RegisterDevSession();
public:
    void_t HandlerDevCmdSet(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdGet(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdStt(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdAdd(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdDel(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdLstAdd(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdLstDel(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdReset(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdResetRes(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdRestart(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdInfo(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdLstRes(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdLstAddRes(JsonCommand_p pJsonCommand);
    void_t HandlerDevCmdLstDelRes(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdSyncRes(JsonCommand_p pJsonCommand);

    void_t HandlerDevCmdAuthReq(JsonCommand_p pJsonCommand);
};

typedef DevManager  DevManager_t;
typedef DevManager* DevManager_p;

#endif /* !DEVMANAGER_HPP_ */
