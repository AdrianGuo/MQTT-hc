/*
 * ZbDriver.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBDRIVER_HPP_
#define ZBDRIVER_HPP_

#include <Map.hpp>
#include <ZbBasicCmd.hpp>
#include <ZbSocketCmd.hpp>
#include <IZbDriver.hpp>
#include <SZbSerial.hpp>
#include <typedefs.h>
#include <Vector.hpp>
#include <Event.hpp>
#include <ZbModelDb.hpp>
#include <ZbZclCmd.hpp>
#include <ZbZclGlobalCmd.hpp>
#include <ZbZdoCmd.hpp>
#include <Locker.hpp>
#include <JsonZigbeeSession.hpp>
#include "RTimer.hpp"

#define WAIT_PACKET_RESPONSE             4000

typedef Functor1_t(JsonCommand_p)   ProcZbCmdFunctor_t;
typedef ProcZbCmdFunctor_t*         ProcZbCmdFunctor_p;

class ZbDriver {
private:
    DriverFunctor_t     m_SerialRecvFunctor;
    ZbCtrllerFunctor_p  m_pZbCtrllerFunctor;
    SZbSerial_t 		m_SZbSerial;
    Locker_p			m_pLocker;

    ZbBasicCmd_p     	m_pZbBasicCmd;
    ZbZdoCmd_p       	m_pZbZdoCmd;
    ZbZclCmd_p       	m_pZbZclCmd;
    ZbZclGlobalCmd_p 	m_pZbZclGlobalCmd;

    Queue<JsonCommand_p>  	m_queRecvMsg;
    JsonRecvZigbeeSession_p m_pJsonRecvSession;

    typedef Map<String, ProcZbCmdFunctor_t> MapProcFunctor;
    MapProcFunctor      m_mapProcFunctor;

    ZbDriver(const_char_p chPortname);

    RTimer_p m_pTimer;
    int_t m_iRequest;
    TimerFunctor_t m_RequestFunctor;

    void_t HandleRequest(void_p);
    int_t m_idwCheckTime;

public:
    virtual ~ZbDriver();

    static ZbDriver* s_pInstance;
    static ZbModelDb_p s_pZbModel;
    static ZbDriver* GetInstance(const_char_p chPortname = NULL);
    Queue<JsonCommand_p>& GetSerRecvMsgQueue();

    void_t  RegisterProcess(String, ProcZbCmdFunctor_t);
    void_t  ProcSerRecvMsg(JsonCommand_p);
    void_t  ProcDevRecvMsg(void_p);

    void_t  SendDriverFunctor();
    void_t  ZbDriverRecvFunctor(ZbCtrllerFunctor_p);

    void_t  Init(bool stateReq = true);
    bool_t  Start();
    bool_t  Connect();
    bool_t  Close();
    void_t  SendZbPacket(ZbPacket_p);
    void_t  SendJsonMessage(EvAct, void_p);

    void_t ProcCmdAdd(JsonCommand_p);
    void_t ProcCmdDel(JsonCommand_p);
    void_t ProcCmdSet(JsonCommand_p);
    void_t ProcCmdGet(JsonCommand_p);
    void_t ProcCmdReset(JsonCommand_p pJsonCommand = NULL);
    void_t ProcCmdRestart(JsonCommand_p);
    void_t ProcCmdInfo(JsonCommand_p);

};

typedef ZbDriver ZbDriver_t;
typedef ZbDriver* ZbDriver_p;

#endif /* ZBDRIVER_HPP_ */
