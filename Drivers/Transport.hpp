/*
 * Transport.h
 *
 *  Created on: Feb 21, 2017
 *      Author: hoang
 */

#ifndef DRIVERS_TRANSPORT_HPP_
#define DRIVERS_TRANSPORT_HPP_

#include <sys/socket.h>
#include <netinet/in.h>

#include "typedefs.h"
#include "Functor.hpp"
#include "Queue.hpp"
#include "LThread.hpp"
#include "Packet.hpp"
#include "Locker.hpp"

#ifndef SOCKET_ERROR
#define SOCKET_ERROR                        (-1)
#endif /* SOCKET_ERROR */

#ifndef INVALID_SOCKET
#define INVALID_SOCKET                      SOCKET_ERROR
#endif /* INVALID_SOCKET */

#define SOCKET_COMPLETE                     (0)
#define SOCKET_INTERRUPTED                  (-2)

typedef struct sockaddr_in                  sockaddr_t;
typedef sockaddr_t*                         sockaddr_p;

typedef struct sockaddr_in6                 sockaddr6_t;
typedef sockaddr6_t*                        sockaddr6_p;

typedef Funtor0_t                           SMQTTFunctor_t;
typedef SMQTTFunctor_t*                     SMQTTFunctor_p;

class Transport {
private:
    sockaddr_t m_SockAddr;
    sockaddr_in6 m_SockAddr6;
    sa_family_t m_family;

    int_t m_boIsConnected;
    int_t m_boIsClosing;
    bool_t m_boIsStarted;
    int_t m_boIsBlocked;

    u8_p m_pbyBuffer;
    u32_t m_idwBufferReadPos;
    u32_t m_idwBufferWritePos;

    SMQTTFunctor_p m_pSMQTTRecvFunctor;

    Locker_p m_pTransportLocker;

    Queue<Packet_p> m_queTransportPacket;

    threadFunctor_t m_TransportThreadFunctor;
    LThread_p m_pTransportThread;

    threadFunctor_t m_TransportReadSocketThreadFunctor;
    LThread_p m_pTransportReadSocketThread;

    bool_t IsWritable(u32_t);
    bool_t IsReadable(u32_t);

    Transport(const_char_p, int_t);
    static Transport* s_pInstance;
public:
    int_t m_idwSockfd;
    int_t m_idwPort;

    static Transport* getInstant(const_char_p, int_t);
    virtual ~Transport();

    void_p TransportThreadProc(void_p);
    void_p TransportReadSocketThreadProc(void_p);

    bool_t RecvFunctor(SMQTTFunctor_p);

    bool_t Connect();
    bool_t Close();
    bool_t IsConnected();
    bool_t IsBlocked();

    bool_t Start();

    bool_t SetNonBlocking();
    bool_t SetBlocking();

    int_t GetBuffer(u8_p, u32_t);
    void PushBuffer(u8_p, u32_t);

    int_t DiSend(u8_p, u32_t);
    int_t DiGet(u8_p, u32_t);
};

typedef Transport Transport_t;
typedef Transport* Transport_p;

#endif /* DRIVERS_TRANSPORT_HPP_ */
