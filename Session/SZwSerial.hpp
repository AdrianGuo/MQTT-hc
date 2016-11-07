#ifndef SZWSERIAL_HPP_
#define SZWSERIAL_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "Serial.hpp"
#include "Functor.hpp"
#include "ZwPacket.hpp"
#include "Packet.hpp"
#include "Event.hpp"

typedef Functor1_t(void_p)          ZwDriverRecvMsgFunctor_t;
typedef ZwDriverRecvMsgFunctor_t*   ZwDriverRecvMsgFunctor_p;

typedef enum {
    FRS_SOF_HUNT = 0,
    FRS_LENGTH,
    FRS_TYPE,
    FRS_COMMAND,
    FRS_PAYLOAD,
    FRS_CHECKSUM,
    FRS_RX_TIMEOUT
} RecvState_t;

class SZwSerial {
private:
    Serial_p m_pSerial;
    Event_t m_ACKSignal;
    ZwPacket_p  m_pZwavePacket;
    RecvState_t m_enuRecvState;

    SerialFunctor_t m_SerialSendFunctor;
    ZwDriverRecvMsgFunctor_p m_pDriverRecvFunctor;

    bool_t m_boIsACKNeeded;
    bool_t m_boIsReceivedCAN;
    u8_t m_bySendAttempts;

    String m_strNamePort;

    void_t ParseData(u8_t byData);
    void_t SendZwPacket(ZwPacket_p pZwPacket);
public:
    SZwSerial(const_char_p cPortName);
    SZwSerial(Serial_p pSerial = NULL);
    ~SZwSerial();

    String GetNamePort();

    void_t SZwSerialSendFunctor();
    bool_t SZwSerialRecvFunctor(ZwDriverRecvMsgFunctor_p pDriverFunctor);

    bool_t Start();
    bool_t Connect();
    bool_t Close();

    void_t BufferToZwPacket(u8_p pByBuffer, u32_t dwLen);
    void_t PushZwPacket(ZwPacket_p pZwPacket);
};

typedef SZwSerial  SZwSerial_t;
typedef SZwSerial* SZwSerial_p;

#endif /* !SZWSERIAL_HPP_ */
