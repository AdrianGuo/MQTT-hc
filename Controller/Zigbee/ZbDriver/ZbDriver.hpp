/*
 * ZbDriver.hpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#ifndef ZBDRIVER_HPP_
#define ZBDRIVER_HPP_

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

class ZbDriver {
private:
    DriverFunctor_t     m_driverSendFunctor;
    ZbCtrllerFunctor_p  m_pZbCtrllerFunctor;

    SZbSerial_t m_SZbSerial;

    ZbBasicCmd_p     m_pZbBasicCmd;
    ZbZdoCmd_p       m_pZbZdoCmd;
    ZbZclCmd_p       m_pZbZclCmd;
    ZbZclGlobalCmd_p m_pZbZclGlobalCmd;

    ZbDriver(const_char_p chPortname);

public:
    virtual ~ZbDriver();

    static ZbDriver* s_pInstance;
    static ZbDriver* GetInstance(const_char_p chPortname = NULL);
    static ZbModelDb_p s_pZbModel;

    Packet_p    ProcSendMessage(void_p pInBuffer, u32_t dwMsecTimeout = WAIT_PACKET_RESPONSE, bool_t boReturned = FALSE);
    void_t      ProcRecvMessage(void_p);

    void_t      SendDriverFunctor();
    void_t      ZbDriverRecvFunctor(ZbCtrllerFunctor_p);

    void_t      Init();
    bool_t      Start();
    bool_t      Connect();
    bool_t      Close();
    void_t      SendZbPacket(ZbPacket_p);
    void_t      SendJsonMessage(EvAct, void_p);

    void_t      HandleZbAdd(ZbMessage_p);
    void_t      HandleDevSet(ZbMessage_p);
    void_t      HandleZbReset(ZbMessage_p);
    void_t      HandleZbDel(ZbMessage_p);

};

typedef ZbDriver ZbDriver_t;
typedef ZbDriver* ZbDriver_p;

#endif /* ZBDRIVER_HPP_ */
