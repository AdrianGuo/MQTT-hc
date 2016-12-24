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
#include <ZbMessage.hpp>
#include <SZbSerial.hpp>
#include <typedefs.h>
#include <Vector.hpp>
#include <Event.hpp>
#include <ZbModelDb.hpp>
#include <ZbZclCmd.hpp>
#include <ZbZclGlobalCmd.hpp>
#include <ZbZdoCmd.hpp>

#define WAIT_PACKET_RESPONSE             4000

typedef Functor1_t(ZbMessage_p)     ProcZbCmdFunctor_t;
typedef ProcZbCmdFunctor_t*         ProcZbCmdFunctor_p;

class ZbDriver {
private:
    DriverFunctor_t     m_driverSendFunctor;
    ZbCtrllerFunctor_p  m_pZbCtrllerFunctor;

    SZbSerial_t m_SZbSerial;

    ZbBasicCmd_p     m_pZbBasicCmd;
    ZbZdoCmd_p       m_pZbZdoCmd;
    ZbZclCmd_p       m_pZbZclCmd;
    ZbZclGlobalCmd_p m_pZbZclGlobalCmd;

    Queue<ZbMessage_p>  m_queSendZbMsg;

    typedef Map<ZbMessage::Command, ProcZbCmdFunctor_t> MapProcFunctor;
    MapProcFunctor      m_mapProcFunctor;

    ZbDriver(const_char_p chPortname);

public:
    virtual ~ZbDriver();

    static ZbDriver* s_pInstance;
    static ZbModelDb_p s_pZbModel;
    static ZbDriver* GetInstance(const_char_p chPortname = NULL);
    Queue<ZbMessage_p>& GetSendZbMsgQueue();

    void_t  RegisterProcess(ZbMessage::Command, ProcZbCmdFunctor_t);
    void_t  ProcSendMessage(ZbMessage_p);
    void_t  ProcRecvMessage(void_p);

    void_t  SendDriverFunctor();
    void_t  ZbDriverRecvFunctor(ZbCtrllerFunctor_p);

    void_t  Init(bool req = true);
    bool_t  Start();
    bool_t  Connect();
    bool_t  Close();
    void_t  SendZbPacket(ZbPacket_p);
    void_t  SendJsonMessage(EvAct, void_p);

    void_t ProcCmdAdd(ZbMessage_p);
    void_t ProcCmdDel(ZbMessage_p);
    void_t ProcCmdSet(ZbMessage_p);
    void_t ProcCmdGet(ZbMessage_p);
    void_t ProcCmdReset(ZbMessage_p);
    void_t ProcCmdRestart(ZbMessage_p);
    void_t ProcCmdInfo(ZbMessage_p);
    void_t ProcCmdAuth(ZbMessage_p);

};

typedef ZbDriver ZbDriver_t;
typedef ZbDriver* ZbDriver_p;

#endif /* ZBDRIVER_HPP_ */
