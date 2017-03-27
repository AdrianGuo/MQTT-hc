/*
 * SZbSerial.hpp
 *
 *  Created on: Sep 10, 2016
 *      Author: taho
 */

#ifndef SESSION_SZBSERIAL_HPP_
#define SESSION_SZBSERIAL_HPP_

#include <Typedefs.h>
#include <Serial.hpp>
#include <Functor.hpp>
#include <ZbPacket.hpp>
#include <Packet.hpp>
#include <Event.hpp>

typedef Functor1_t(void_p) DriverFunctor_t;
typedef DriverFunctor_t* DriverFunctor_p;

typedef enum {
    ZB_FRS_SOF_HUNT1 = 0,
    ZB_FRS_SOF_HUNT2,
    ZB_FRS_LENGTH,
    ZB_FRS_SEQ,
    ZB_FRS_COMMAND,
    ZB_FRS_PAYLOAD,
    ZB_FRS_CHECKSUM,
    ZB_FRS_RX_TIMEOUT
} ZbRecvState_t;

class SZbSerial {
private:
    Serial_t m_Serial;
    Event_t m_ACKSignal;
    ZbPacket_p  m_pZbPacket;
    ZbRecvState_t m_enuRecvState;

    SerialFunctor_t m_serialSendFunctor;
    DriverFunctor_p m_pDriverRecvFunctor;

    bool_t m_boIsACKNeeded;
    bool_t m_boIsReceivedCAN;
    u8_t m_bySendAttempts;

    void_t ParseData(u8_t byData);
    void_t SendZbPacket(ZbPacket_p pZbPacket);
public:
    SZbSerial(const_char_p chPortname);
    ~SZbSerial();

    bool_t RecvFunctor(DriverFunctor_p pDriverFunctor);

    bool_t Start();
    bool_t Connect();
    bool_t Close();

    void_t BufferToZbPacket(u8_p pByBuffer, u32_t dwLen);
    void_t PushZbPacket(ZbPacket_p pZbPacket);
};

typedef SZbSerial  SZbSerial_t;
typedef SZbSerial* SZbSerial_p;


#endif /* SESSION_SZBSERIAL_HPP_ */
