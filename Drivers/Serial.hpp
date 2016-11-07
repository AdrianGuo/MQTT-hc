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
#ifndef SERIAL_HPP_
#define SERIAL_HPP_

/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "typedefs.h"
#include "Packet.hpp"
#include "String.hpp"
#include "Queue.hpp"
#include "LThread.hpp"
#include "Locker.hpp"
#include "Functor.hpp"

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/
#define BAUD96                              96
#define BAUD192                             192
#define BAUD384                             384
#define BAUD576                             576
#define BAUD1152                            1152

typedef Functor2_t(u8_p, u32_t)             SerialFunctor_t;
typedef SerialFunctor_t*                    SerialFunctor_p;

class Serial {
    u16_t m_wBaudRate;
    i32_t m_idwPortfd;
    u8_p m_pByBuffer;

    bool_t m_boIsConnected;
    bool_t m_boIsClosing;
    bool_t m_boIsStarted;

    String m_strNamePort;
    Queue<Packet_p> m_queSerialPacket;
    Locker_p m_pSerialLocker;
    LThread_p m_pSerialThread;
    threadFunctor_t m_SerialthreadFunctor;
    SerialFunctor_p m_pSerialFunctor;
public:
    Serial(const_char_p portName, u16_t wBaudRate = BAUD1152);
    ~Serial();

    void_p SerialThreadProc(void_p pBuffer);

    bool_t SerialRecvFunctor(SerialFunctor_p pRecvFunctor);

    String GetNamePort();
    void_t SetNamePort(String strNamePort);

    u16_t GetBaudRate();
    void_t SetBaudRate(u16_t wBaudRate);

    bool_t Connect();
    bool_t Close();

    bool_t IsConnected();
    bool_t IsClosing();

    bool_t IsWriteable(u32_t dwMsecTimeout);
    bool_t IsReadable (u32_t dwMsecTimeout);

    bool_t Start();

    void_t PushData(u8_t byData);
    void_t PushBuffer(u8_p pByBuffer, u32_t dwLength);
    void_t PushPacket(Packet_p pPacket);
};

typedef Serial  Serial_t;
typedef Serial* Serial_p;
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


#endif /* SERIAL_HPP_ */
