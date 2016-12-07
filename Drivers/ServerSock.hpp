#ifndef SERVERSOCK_HPP_
#define SERVERSOCK_HPP_

#include <Map.hpp>
#include <RTimer.hpp>
#include <Functor.hpp>
#include <LThread.hpp>
#include <Packet.hpp>
#include <Client.hpp>

#ifndef SOCKET_ERROR
#define SOCKET_ERROR                        (-1)
#endif /* SOCKET_ERROR */

#ifndef INVALID_SOCKET
#define INVALID_SOCKET                      SOCKET_ERROR
#endif /* INVALID_SOCKET */

#define SOCKET_COMPLETE                     (0)
#define SOCKET_INTERRUPTED                  (-2)

#define MAX_CLIENTS                         (10)
#define BUFFER_SOCKET_SIZE                  (1024)

typedef struct timeval                      timeval_t;
typedef timeval_t*                          timeval_p;

typedef Functor3_t(u8_p, u32_t, int_t)      SServerFunctor_t;
typedef SServerFunctor_t*                   SServerFunctor_p;

typedef Map<int_t,Client_t>                 MapClients_t;
typedef Map<int_t,Client_t>*                MapClients_p;

class ServerSock {
private:
    int_t m_idwSockfd;
    int_t m_idwPort;

    fd_set m_ReadFds;
    fd_set m_WriteFds;
    int_t m_idwNfds;

    bool_t m_boIsServed;
    bool_t m_boIsStarted;

    MapClients_t m_mapClients;
    sockaddr_p m_pSockAddr;

    SServerFunctor_p m_pSServerFunctor;
    Locker_p m_pServerSockLocker;

    threadFunctor_t m_ListenThreadFunctor;
    LThread_p m_pListenThread;

    threadFunctor_t m_MessageThreadFunctor;
    LThread_p m_pMessageThread;

    RTimer_p m_pServerSockTimer;
    timerFunctor_t m_keepaliveTimerFunctor;
    int_t m_iKeepAliveTimerHandle;

public:
    ServerSock(int_t idwPort);
    ~ServerSock();

    void_p ListenThread(void_p pBuffer);
    void_p MessageThread(void_p pBuffer);
    bool_t RecvFunctor(SServerFunctor_p pRecvFunctor);

    bool_t Serve();
    bool_t Close();
    bool_t IsServed();
    bool_t Start();

    MapClients_p GetClients();
    void_t RemoveClient(Client_t);

    void_t PushPacket(Client_p pClient, Packet_p pOutgoing);

    void_t HandleKeepAliveProcess(void_p pBuffer);
};

typedef ServerSock ServerSock_t;
typedef ServerSock* ServerSock_p;

#endif /* !SERVERSOCK_HPP_ */
