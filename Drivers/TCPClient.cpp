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
 ******************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <netdb.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "LogPlus.hpp"
#include "TCPClient.hpp"

#ifndef INVALID_SOCKET
#define INVALID_SOCKET                      SOCKET_ERROR
#endif /* INVALID_SOCKET */

#define CONNECTION_TIMEOUT_SEC              (5)

#define BUFFER_SOCKET_SIZE                  (1024)

/**
 * @func   ClientSock
 * @brief  None
 * @param  None
 * @retval None
 */
TCPClient::TCPClient(
    const_char_p pChAddress,
    int_t idwPort
) {
    int idwResult = SOCKET_ERROR;

    struct sockaddr_in* pAddress = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    struct addrinfo* pResult = NULL;
    struct addrinfo hints = { 0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL };

    if ((idwResult = getaddrinfo(pChAddress, NULL, &hints, &pResult)) == 0) {
        struct addrinfo* res = pResult;
        while (res != NULL) {
            if (res->ai_family == AF_INET) {
                pResult = res;
                break;
            }
            res = res->ai_next;
        }

        if (pResult == NULL) {
            idwResult = SOCKET_ERROR;
        } else {
            if (pResult->ai_family == AF_INET) {
                pAddress->sin_port = htons(idwPort);
                pAddress->sin_family = AF_INET;
                pAddress->sin_addr = ((struct sockaddr_in*)(pResult->ai_addr))->sin_addr;
            } else {
                idwResult = SOCKET_ERROR;
            }
            freeaddrinfo(pResult);
        }
    }

    m_idwSockfd = 0;
    m_idwPort = idwPort;

    m_pbBuffer = new u8_t[BUFFER_SOCKET_SIZE];
    memset(m_pbBuffer, '\0', BUFFER_SOCKET_SIZE);

    m_boIsConnected = FALSE;
    m_boIsClosing = FALSE;
    m_boIsStarted = FALSE;
    m_boIsBlocked = TRUE;

    m_pSockAddr = pAddress;
    m_pSClientRecvFunctor = NULL;
    m_pClientSockThread = new LThread();
    m_ClientSockThreadFunctor =
    makeFunctor((threadFunctor_p) NULL, *this, &TCPClient::ClientSockThreadProc);
    m_pClientSockThread->RegThreadFunctor(&m_ClientSockThreadFunctor);
    m_pClientSockLocker = new Locker();
}


/**
 * @func   ~ClientSock
 * @brief  None
 * @param  None
 * @retval None
 */
TCPClient::~TCPClient() {
    if (m_pSockAddr != NULL) {
        delete(m_pSockAddr);
    }

    if (m_pbBuffer != NULL) {
        delete[] m_pbBuffer;
        m_pbBuffer = NULL;
    }

    if (m_pSockAddr != NULL) {
        delete m_pSockAddr;
        m_pSockAddr = NULL;
    }

    if (m_pClientSockThread != NULL) {
        delete m_pClientSockThread;
        m_pClientSockThread = NULL;
    }

    if (m_pClientSockLocker != NULL) {
        delete m_pClientSockLocker;
        m_pClientSockLocker = NULL;
    }
}

/**
 * @func   RecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::RecvFunctor(
    SClientFunctor_p pSClientRecvFunctor
) {
    if (pSClientRecvFunctor != NULL) {
        m_pSClientRecvFunctor = pSClientRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::Connect() {
    int idwSockfd = SOCKET_ERROR;

    LOG_DEBUG("connecting...");

    /* Set socket fd */
    if ((idwSockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        LOG_ERROR("socket fail");
        m_boIsConnected = FALSE;
        return FALSE;
    }

    m_boIsClosing = FALSE;
    m_idwSockfd = idwSockfd;

    if (m_pSockAddr != NULL) {
        int_t idwResult =
        connect(m_idwSockfd, (struct sockaddr*) m_pSockAddr, sizeof(*m_pSockAddr));

        if (m_boIsBlocked) { /* If is blocking */
            if (idwResult == SOCKET_ERROR) {
                LOG_ERROR("connect fail"); /* Debug */
                m_boIsConnected = FALSE;
                return FALSE;
            } else {
                m_boIsConnected = TRUE;
            }
        } else { /* If is non-blocking */
            fd_set rset, wset;
            struct timeval tval;
            int_t idwError = 0;

            if (idwResult == SOCKET_ERROR) {
                LOG_WARN("wait connect");
                if (errno != EINPROGRESS) {
                    LOG_ERROR("connect fail");
                    close(m_idwSockfd);
                    m_boIsConnected = FALSE;
                    return FALSE;
                }
            }

            if (idwResult == 0) {
                LOG_INFO("connect local");
                goto done; /* connect completed immediately */
            }

            FD_ZERO(&rset);
            FD_SET(m_idwSockfd, &rset);
            FD_SET(m_idwSockfd, &wset);
            tval.tv_sec = CONNECTION_TIMEOUT_SEC;
            tval.tv_usec = 0;
            /* Waiting for the socket to be ready for either reading and writing */
            if ((idwResult == select(m_idwSockfd + 1, &rset, &wset, NULL, &tval)) == 0) {
                LOG_WARN("timeout");
                m_boIsConnected = FALSE;
                close(m_idwSockfd);
                idwError = ETIMEDOUT;
                return FALSE;
            }

            if (FD_ISSET(m_idwSockfd, &rset) || FD_ISSET(m_idwSockfd, &wset)) {
                socklen_t dwLen = sizeof(idwError);
                if (getsockopt(m_idwSockfd, SOL_SOCKET, SO_ERROR, &idwError, &dwLen) < 0) {
                    /* Solaris pending error */
                    close(m_idwSockfd);
                    m_boIsConnected = FALSE;
                    return FALSE;
                } else {
                    LOG_INFO("connected");
                    m_boIsConnected = TRUE;
                }
            } else {
                close(m_idwSockfd);
                m_boIsConnected = FALSE;
                return FALSE;
            }
            done:
            m_boIsConnected = TRUE;
            if (idwError > 0) {
                errno = idwError;
                close(m_idwSockfd);
                m_boIsConnected = FALSE;
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::Close() {
    int_t idwResult;

    LOG_INFO("close sockfd %d", m_idwSockfd);

    m_pClientSockLocker->Lock();
    if ((idwResult = shutdown(m_idwSockfd, SHUT_RDWR)) == SOCKET_ERROR) {
        m_pClientSockLocker->UnLock();
        LOG_ERROR("shutdown fail");
        return FALSE;
    }
    m_pClientSockLocker->UnLock();

    m_pClientSockLocker->Lock();
    if ((idwResult = close(m_idwSockfd)) == SOCKET_ERROR) {
        m_pClientSockLocker->UnLock();
        LOG_ERROR("close socket %d fail", m_idwSockfd);
        return FALSE;
    }
    m_pClientSockLocker->UnLock();

    LOG_INFO("after close sockfd %d", m_idwSockfd);

    m_pClientSockLocker->Lock();
    m_boIsConnected = FALSE;
    m_boIsClosing = TRUE;
    m_pClientSockLocker->UnLock();

    return TRUE;
}

/**
 * @func   IsConnected
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::IsConnected() {
    return m_boIsConnected;
}

/**
 * @func   IsBlocked
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::IsBlocked() {
    return m_boIsBlocked;
}

/**
 * @func   IsWritable
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::IsWritable(
    u32_t dwMsecTimeout
) {
    fd_set Writefds;
    struct timeval timeout;
    bool_t boRetVal = FALSE;
    int_t idwResult;

    FD_ZERO(&Writefds);
    FD_SET(m_idwSockfd, &Writefds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (dwMsecTimeout > 1000) {
        dwMsecTimeout -= 1000;
        timeout.tv_sec++;
    }
    timeout.tv_usec = dwMsecTimeout * 1000;

    idwResult = select(m_idwSockfd + 1, NULL, &Writefds, NULL, &timeout);

    if (idwResult == 0) {
    } else if (idwResult == -1) {
        LOG_ERROR("writable error"); /* error */
    } else {
        if (FD_ISSET(m_idwSockfd, &Writefds)) {
            boRetVal = TRUE;
        }
    }
    return boRetVal;
}

/**
 * @func   IsReadable
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::IsReadable(
    u32_t dwMsecTimeout
) {
    fd_set Readfds;
    struct timeval timeout;
    bool_t boRetVal = FALSE;
    int_t idwResult;

    FD_ZERO(&Readfds);
    FD_SET(m_idwSockfd, &Readfds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    while (dwMsecTimeout > 1000) {
        dwMsecTimeout -= 1000;
        timeout.tv_sec++;
    }
    timeout.tv_usec = dwMsecTimeout * 1000;

    idwResult = select(m_idwSockfd + 1, &Readfds, NULL, NULL, &timeout);

    if (idwResult == 0) {
    } else if (idwResult == -1) {
        LOG_ERROR("readable error"); /* error */
    } else {
        if (FD_ISSET(m_idwSockfd, &Readfds)) {
            boRetVal = TRUE;
        }
    }
    return boRetVal;
}

/**
 * @func   Ping
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::Ping() {
    return TRUE;
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::Start() {
    bool_t boRetVal = FALSE;

    m_pClientSockLocker->Lock();
    LOG_INFO("start");
    if (!m_boIsStarted) {
        if (m_pClientSockThread->Start()) {
            m_boIsStarted = TRUE;
            boRetVal = TRUE;
        } else {
            LOG_ERROR("thread fail");
        }
    }
    m_pClientSockLocker->UnLock();

    return boRetVal;
}

/**
 * @func   ClientSockThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
TCPClient::ClientSockThreadProc(
    void_p pBuffer
) {
    while (TRUE) {
        m_pClientSockLocker->Lock();
        if (m_boIsClosing) {
            break;
        }
        m_pClientSockLocker->UnLock();

        if (IsConnected() && IsReadable(100)) {
            int_t iLength = recv(m_idwSockfd, m_pbBuffer, BUFFER_SOCKET_SIZE, 0);
            if ((m_pSClientRecvFunctor != NULL) && (iLength > 0)) {
                (*m_pSClientRecvFunctor)((u8_p)m_pbBuffer, iLength);
            }
            memset(m_pbBuffer, '\0', BUFFER_SOCKET_SIZE);
        }

        if (IsConnected() && IsWritable(0)) {
            Packet_p pPacket = NULL;
            m_pClientSockLocker->Lock();
            if (!m_queClientSockPacket.empty()) {
                pPacket = m_queClientSockPacket.front();
                if (pPacket != NULL) {
                    send(m_idwSockfd, pPacket->GetBuffer(), pPacket->Length(), 0);
                    m_queClientSockPacket.pop();
                }
            }
            m_pClientSockLocker->UnLock();
        }
        usleep(500);
    }
    m_pClientSockLocker->UnLock();

    m_pClientSockLocker->Lock();
    m_boIsStarted = FALSE;
    m_boIsClosing = FALSE;
    m_pClientSockLocker->UnLock();

    LOG_INFO("thread exit");
    pthread_exit(NULL);

    return NULL;
}

/**
 * @func   SetNonBlocking
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::SetNonBlocking() {
    int_t idwFlags;
    int_t idwResult = SOCKET_ERROR;

    LOG_INFO("set non-blocking");

    if ((idwFlags = fcntl(m_idwSockfd, F_GETFL, 0)) < 0) {
        LOG_ERROR("get error"); /* Debug */
        return FALSE;
    }

    if ((idwResult = fcntl(m_idwSockfd, F_SETFL, idwFlags | O_NONBLOCK)) < 0) {
        LOG_ERROR("set error"); /* Debug */
        fcntl(m_idwSockfd, F_SETFL, idwFlags); /* Restore if error */
        return FALSE;
    }

    m_boIsBlocked = FALSE;
    return TRUE;
}

/**
 * @func   SetBlocking
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
TCPClient::SetBlocking() {
    int_t idwFlags;
    int_t idwResult = SOCKET_ERROR;

    LOG_INFO("set blocking");

    if ((idwFlags = fcntl(m_idwSockfd, F_GETFL, 0)) < 0) {
        LOG_ERROR("get error");/* Debug */
        return FALSE;
    }

    if ((idwResult = fcntl(m_idwSockfd, F_SETFL, idwFlags & (~O_NONBLOCK))) < 0) {
        LOG_ERROR("set error"); /* Debug */
        fcntl(m_idwSockfd, F_SETFL, idwFlags); /* Restore if error */
        return FALSE;
    }

    m_boIsBlocked = TRUE;
    return TRUE;
}

/**
 * @func   PushData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
TCPClient::PushData(
    u8_t byData
) {
    Packet_p pPacket = new Packet(1);
    pPacket->Push(byData);
    m_queClientSockPacket.push(pPacket);
}

/**
 * @func   PushBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
TCPClient::PushBuffer(
    u8_p pByBuffer,
    u32_t dwLength
) {
    m_pClientSockLocker->Lock();
    Packet_p pPacket = new Packet(dwLength);
    pPacket->Push(pByBuffer, dwLength);
    m_queClientSockPacket.push(pPacket);
    m_pClientSockLocker->UnLock();
}

/**
 * @func   PushPacket
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
TCPClient::PushPacket(
    Packet_p pPacket
) {
    m_pClientSockLocker->Lock();
    m_queClientSockPacket.push(pPacket);
    m_pClientSockLocker->UnLock();
}
