#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <netdb.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.hpp>
#include <LogPlus.hpp>

#include <ServerSock.hpp>

#define CONNECT_TIMEOUT_SEC                     (5)

/**
 * @func   ServerSock
 * @brief  None
 * @param  None
 * @retval None
 */
ServerSock::ServerSock(
    int_t idwPort
) {
    m_idwSockfd = SOCKET_ERROR;
    m_idwPort = idwPort;

    FD_ZERO(&m_ReadFds);
    FD_ZERO(&m_WriteFds);
    m_idwNfds = 0;

    m_boIsServed = FALSE;
    m_boIsStarted = FALSE;


    m_pSServerFunctor = NULL;
    m_pListenThread = new LThread();
    m_pMessageThread = new LThread();
    m_pServerSockLocker = new Locker();

    m_ListenThreadFunctor =
    makeFunctor((threadFunctor_p) NULL, *this, &ServerSock::ListenThread);
    m_pListenThread->RegThreadFunctor(&m_ListenThreadFunctor);

    m_MessageThreadFunctor =
    makeFunctor((threadFunctor_p) NULL, *this, &ServerSock::MessageThread);
    m_pMessageThread->RegThreadFunctor(&m_MessageThreadFunctor);

    m_pSockAddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    m_pSockAddr->sin_family = AF_INET;
    m_pSockAddr->sin_port = htons(m_idwPort);
    m_pSockAddr->sin_addr.s_addr = INADDR_ANY;

    m_pServerSockTimer = RTimer::getTimerInstance();
    m_keepaliveTimerFunctor =
    makeFunctor((timerFunctor_p) NULL, *this, &ServerSock::HandleKeepAliveProcess);
    m_iKeepAliveTimerHandle = m_pServerSockTimer->StartTimer(
    RTimer::Repeat::Forever, CONNECT_TIMEOUT_SEC, &m_keepaliveTimerFunctor, NULL);
}

/**
 * @func   ~ServerSock
 * @brief  None
 * @param  None
 * @retval None
 */
ServerSock::~ServerSock() {

}

/**
 * @func   RecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ServerSock::RecvFunctor(
    SServerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pSServerFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   Serve
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ServerSock::Serve() {
    int_t idwSockfd = SOCKET_ERROR;
    int_t optval;

    if ((idwSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        m_boIsServed = FALSE;
        return FALSE;
    }

    m_idwSockfd = idwSockfd;
    optval = 1;
    if (setsockopt(m_idwSockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        m_boIsServed = FALSE;
        return FALSE;
    }

    if (bind(m_idwSockfd, (struct sockaddr *) m_pSockAddr, sizeof(*m_pSockAddr)) < 0) {
        m_boIsServed = FALSE;
        return FALSE;
    }

    if(listen(m_idwSockfd, MAX_CLIENTS) < 0) {
        m_boIsServed = FALSE;
        return FALSE;
    }

    m_boIsServed = TRUE;
    return TRUE;
}

/**
 * @func   IsServed
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t ServerSock::IsServed() {
    return m_boIsServed;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t ServerSock::Close() {
    if(close(m_idwSockfd) == 0)
        return TRUE;
    return FALSE;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
ServerSock::ListenThread(
    void_p pBuffer
) {
    fd_set readset;
    int_t ready, nfds = 0;
    timeval_t timeout; //Not used now, non-blocking

    while (IsServed() == TRUE) {
        FD_ZERO(&readset);
        FD_SET(m_idwSockfd, &readset);
        nfds = m_idwSockfd + 1;

        ready = select(nfds, &readset, NULL, NULL, &timeout);

        if ((ready == SOCKET_ERROR) || (ready == EINTR) || (ready == EBADF)) {
            LOG_ERROR("SOCKET_ERROR");
            continue;
        } else if (ready == 1) {
            if (FD_ISSET(m_idwSockfd, &readset)) {
                LOG_DEBUG("New connection.");
                int_t tempSock;
                sockaddr_p tempSockAddr = (struct sockaddr_in*) malloc(
                        sizeof(struct sockaddr_in));
                int_t tempSockAddrLen = sizeof(*tempSockAddr);

                tempSock = accept(m_idwSockfd, (struct sockaddr *) tempSockAddr,
                        (socklen_t*) &tempSockAddrLen);

                if (tempSock == INVALID_SOCKET) {
                    LOG_INFO("invalid socket");
                    delete tempSockAddr;
                    close(tempSock);
                    continue;
                } else {
                    LOG_INFO("new client from %s on socket %d.",
                            inet_ntoa(tempSockAddr->sin_addr), tempSock);

                    if(m_mapClients.find(tempSock) != m_mapClients.end())
                        m_mapClients.erase(tempSock);

                    Client_t client;
                    client.SetSocketFd(tempSock);
                    client.SetSocketAddress(tempSockAddr);
                    FD_SET(tempSock, &m_ReadFds);
                    FD_SET(tempSock, &m_WriteFds);
                    if(m_idwNfds <= tempSock)
                        m_idwNfds = tempSock + 1;
                    m_mapClients[tempSock] = client;

                    tempSockAddr = NULL;
                    delete tempSockAddr;
                }
            }
        }
    }
    m_pListenThread->Stop();
    return NULL;
}

/**
 * @func   MessageThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
ServerSock::MessageThread(
    void_p pBuffer
) {
    timeval_t timeout;
    timeout.tv_usec = 100000;
    int_t ready = INVALID_SOCKET;
    fd_set readfds, writefds;

    while (TRUE) {
        sleep(1);
        readfds = m_ReadFds;
        writefds = m_WriteFds;
        ready = select(m_idwNfds, &readfds, &writefds, NULL, &timeout);
        if ((ready != SOCKET_ERROR) && (ready != EINTR) && (ready != EBADF) && (ready != 0)) {
            for (int_t fd = 0; fd < m_idwNfds; fd++) {
                if (m_mapClients.find(fd) != m_mapClients.end()) {
                    // Receive messages
                    if (FD_ISSET(fd, &m_ReadFds)) {
                        m_mapClients[fd].SetCount();

                        u8_p m_pByBuffer = new u8_t[BUFFER_SOCKET_SIZE];
                        int_t iLength = recv(fd, m_pByBuffer, BUFFER_SOCKET_SIZE,0);

                        if (m_pSServerFunctor != NULL && iLength > 0) {
                            m_pSServerFunctor->operator ()((u8_p) m_pByBuffer, iLength, fd);
                        }
                        delete m_pByBuffer;
                    }
                    // Send messages
                    if (FD_ISSET(fd, &m_WriteFds)) {
                        while (m_mapClients[fd].PendingMessages() > 0) {
                            Packet_p pPacket = NULL;
                            pPacket = m_mapClients[fd].Front();
                            if (pPacket != NULL) {
                                send(fd, pPacket->GetBuffer(), pPacket->Length(), 0);
                            }
                            m_mapClients[fd].Pop();
                            pPacket = NULL;
                            delete pPacket;
                        }
                    }
                }
            }
        }
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
    }

    m_pMessageThread->Stop();
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ServerSock::Start() {
    bool_t boRetVal = FALSE;

    m_pServerSockLocker->Lock();
    LOG_DEBUG("start");
    if (!m_boIsStarted) {
        if (m_pListenThread->Start() && m_pMessageThread->Start()) {
            m_boIsStarted = TRUE;
            boRetVal = TRUE;
        } else {
            LOG_INFO("thread fail");
        }
    }
    m_pServerSockLocker->UnLock();

    return boRetVal;
}

/**
 * @func   GetClients
 * @brief  None
 * @param  None
 * @retval None
 */
MapClients_p
ServerSock::GetClients() {
    return &m_mapClients;
}

/**
 * @func   RemoveClient
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ServerSock::RemoveClient(
    Client_t client
) {
    close(client.GetSocketFd());
    FD_CLR(client.GetSocketFd(), &m_ReadFds);
    FD_CLR(client.GetSocketFd(), &m_WriteFds);
    m_mapClients.erase(client.GetSocketFd());
    m_pServerSockLocker->Lock();
    m_idwNfds = 0;
    for (MapClients_t::const_iterator_t it = m_mapClients.begin();
            it != m_mapClients.end(); it++) {
        if(m_idwNfds < (it->first + 1))
            m_idwNfds = it->first + 1;
    }
    m_pServerSockLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ServerSock::PushPacket(
    Client_p pClient,
    Packet_p pOutgoing
) {
    pClient->Push(pOutgoing);
}

/**
 * @func   HandleKeepAliveProcess
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ServerSock::HandleKeepAliveProcess(
    void_p pBuffer
) {
    LOG_DEBUG("Handle clients alive state.");
    for (MapClients_t::const_iterator_t it = m_mapClients.begin();
            it != m_mapClients.end(); it++) {
        if (m_mapClients[it->first].IsAlive()) {
            m_mapClients[it->first].SetAlive(FALSE);
        } else {
            RemoveClient(m_mapClients[it->first]);
        }

        if (m_mapClients.size() == 0) break;
    }
}
