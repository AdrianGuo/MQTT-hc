#ifndef CLIENTSOCK_HPP_
#define CLIENTSOCK_HPP_

#include "typedefs.h"
#include "Functor.hpp"
#include "Queue.hpp"
#include "LThread.hpp"
#include "Packet.hpp"
#include "Locker.hpp"

#ifndef SOCKET_ERROR
#define SOCKET_ERROR                        (-1)
#endif /* SOCKET_ERROR */

#define SOCKET_COMPLETE                     (0)
#define SOCKET_INTERRUPTED                  (-2)

typedef struct sockaddr_in                  sockaddr_t;
typedef sockaddr_t*                         sockaddr_p;

typedef Functor2_t(u8_p, u32_t)             SClientFunctor_t;
typedef SClientFunctor_t*                   SClientFunctor_p;

class ClientSock {
private:
    int_t m_idwSockfd;
    int_t m_idwPort;
    u8_p m_pByBuffer;

    bool_t m_boIsConnected;
    bool_t m_boIsClosing;
    bool_t m_boIsStarted;
    bool_t m_boIsBlocked;

    sockaddr_p m_pSockAddr;
    SClientFunctor_p m_pSClientRecvFunctor;

    Locker_p m_pClientSockLocker;

    Queue<Packet_p> m_queClientSockPacket;

    threadFunctor_t m_ClientSockThreadFunctor;
    LThread_p m_pClientSockThread;

    bool_t IsWritable(u32_t dwMsecTimeout);
    bool_t IsReadable(u32_t dwMsecTimeout);
public:
    ClientSock(const_char_p pChostname, int_t idwPort);
    virtual ~ClientSock();
    
    void_p ClientSockThreadProc(void_p pBuffer);

    bool_t RecvFunctor(SClientFunctor_p pRecvFunctor);

    bool_t Connect();
    bool_t Close();
    bool_t IsConnected();
    bool_t IsBlocked();

    bool_t Start();
    bool_t Ping();

    bool_t SetNonBlocking();
    bool_t SetBlocking();

    int_t GetBuffer(void_p pBuffer);

    void_t PushData(u8_t byData);
    void_t PushBuffer(u8_p pByBuffer, u32_t dwLength);
    void_t PushPacket(Packet_p pOutgoing);
};

typedef ClientSock  ClientSock_t;
typedef ClientSock* ClientSock_p;

#endif /* CLIENTSOCK_HPP_ */
