/*
 * Transport.cpp
 *
 *  Created on: Feb 21, 2017
 *      Author: hoang
 */

#include <sys/param.h>
#include <sys/time.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "LogPlus.hpp"
#include "Transport.hpp"

#define BUFFER_SOCKET_SIZE  (2000)

Transport* Transport::s_pInstance = NULL;

/**
 * @func   Transport
 * @brief  None
 * @param  None
 * @retval None
 */
Transport::Transport(
    const_char_p pChostname,
    int_t idwPort
) {
    int_t idwResult = SOCKET_ERROR;
    struct addrinfo *result = NULL;
    struct addrinfo hints = {0, AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP, 0, NULL, NULL, NULL};

    if (pChostname[0] == '[')
      ++pChostname;

    if ((idwResult = getaddrinfo(pChostname, NULL, &hints, &result)) == 0)
    {
        struct addrinfo* res = result;

        /* prefer ip4 addresses */
        while (res)
        {
            if (res->ai_family == AF_INET)
            {
                result = res;
                break;
            }
            res = res->ai_next;
        }

#if defined(AF_INET6)
        if (result->ai_family == AF_INET6)
        {
            m_SockAddr6.sin6_port = htons(idwPort);
            m_SockAddr6.sin6_family = AF_INET6;
            m_SockAddr6.sin6_addr = ((struct sockaddr_in6*)(result->ai_addr))->sin6_addr;
            m_family = AF_INET6;
        }
        else
#endif
        if (result->ai_family == AF_INET)
        {
            m_SockAddr.sin_port = htons(idwPort);
            m_SockAddr.sin_family = AF_INET;
            m_SockAddr.sin_addr = ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
            m_family = AF_INET;
        }
        else
            idwResult = -1;

        freeaddrinfo(result);
    }

    m_idwSockfd = -1;
    m_idwPort = idwPort;

    m_boIsConnected = FALSE;
    m_boIsClosing = FALSE;
    m_boIsStarted = FALSE;
    m_boIsBlocked = TRUE;
    m_boIsMqttDoneSubcribe = FALSE;

    m_pbyBuffer = new u8_t[BUFFER_SOCKET_SIZE];
    m_idwBufferReadPos = 0;
    m_idwBufferWritePos = 0;

    m_pSMQTTRecvFunctor = NULL;
    m_pSendSocketThread = new LThread();
    m_SendSocketThreadFunctor = makeFunctor((threadFunctor_p)NULL, *this, &Transport::SendSocketThreadProc);
    m_pSendSocketThread->RegThreadFunctor(&m_SendSocketThreadFunctor);

    m_pReadSocketThread = new LThread();
    m_ReadSocketThreadFunctor = makeFunctor((threadFunctor_p)NULL, *this, &Transport::ReadSocketThreadProc);
    m_pReadSocketThread->RegThreadFunctor(&m_ReadSocketThreadFunctor);

    m_pProcessThread = new LThread();
    m_ProcessThreadFunctor = makeFunctor((threadFunctor_p)NULL, *this, &Transport::ProcessProc);
    m_pProcessThread->RegThreadFunctor(&m_ProcessThreadFunctor);

    m_pTransportLocker = new Locker();
}

/**
 * @func   ~Transport
 * @brief  None
 * @param  None
 * @retval None
 */
Transport::~Transport() {
    if (m_pSendSocketThread != NULL) {
        delete m_pSendSocketThread;
        m_pSendSocketThread = NULL;
    }
    if (m_pTransportLocker != NULL) {
        delete m_pTransportLocker;
        m_pTransportLocker = NULL;
    }
    if (m_pbyBuffer != NULL) {
        delete[] m_pbyBuffer;
        m_pbyBuffer = NULL;
    }
}

/**
 * @func   IsWritable
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::IsWritable(
    u32_t dwMsecTimeout
) {
    return TRUE;
}

/**
 * @func   IsReadable
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::IsReadable(u32_t dwMsecTimeout) {
    return TRUE;
}

/**
 * @func   getInstant
 * @brief  None
 * @param  None
 * @retval None
 */
Transport*
Transport::getInstant(
    const_char_p pChostname,
    int_t idwPort
) {
    if (s_pInstance == NULL) {
        s_pInstance = new Transport(pChostname, idwPort);
    }
    return s_pInstance;
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::Connect() {
    int_t type = SOCK_STREAM;
    struct timeval tv;
    int_t idwResult = -1;

    LOG_INFO("Transport connecting...");

    m_idwSockfd = socket(m_family, type, 0);
    if (m_idwSockfd != -1)
    {
#if defined(NOSIGPIPE)
        int opt = 1;
        if (setsockopt(*sock, SOL_SOCKET, SO_NOSIGPIPE, (void*)&opt, sizeof(opt)) != 0)
            Log(TRACE_MIN, -1, "Could not set SO_NOSIGPIPE for socket %d", *sock);
#endif

        if (m_family == AF_INET)
            idwResult = connect(m_idwSockfd, (struct sockaddr*)&m_SockAddr, sizeof(m_SockAddr));
#if defined(AF_INET6)
        else
            idwResult = connect(m_idwSockfd, (struct sockaddr*)&m_SockAddr6, sizeof(m_SockAddr6));
#endif
    }
    if (idwResult == -1) {
        LOG_INFO("Transport connecting failed - error = %d, m_idwSockfd = %d", errno, m_idwSockfd);
        m_idwSockfd = -1;
        return FALSE;
    }
    if (m_idwSockfd == INVALID_SOCKET) {
        LOG_INFO("Transport connecting failed");
        return FALSE;
    }

    tv.tv_sec = 1;  /* 1 second Timeout */
    tv.tv_usec = 0;
    setsockopt(m_idwSockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

    m_pTransportLocker->Lock();
    m_boIsConnected = TRUE;
    m_boIsClosing = FALSE;
    m_pTransportLocker->UnLock();
    LOG_INFO("Transport connecting success - m_idwSockfd = %d", m_idwSockfd);
    return TRUE;
}

bool_t Transport::Start() {
    bool_t boRetVal = FALSE;

    //Reset m_pbyBuffer index
    m_idwBufferReadPos = 0;
    m_idwBufferWritePos = 0;

    m_pTransportLocker->Lock();
    LOG_INFO("start thread Transport");
    if (!m_boIsStarted) {
        boRetVal = TRUE;
        if (m_pSendSocketThread->Start()) {
            LOG_INFO("Transport start thread Send success");
        } else {
            boRetVal = FALSE;
            LOG_ERROR("thread Transport fail - %d - %s", errno, strerror(errno));
        }
        if (m_pReadSocketThread->Start()) {
            LOG_INFO("Transport start thread Recv success");
        } else {
            boRetVal = FALSE;
            LOG_ERROR("thread Transport read fail - %d - %s", errno, strerror(errno));
        }
        m_boIsStarted = boRetVal;
    }
    m_pTransportLocker->UnLock();

    return boRetVal;
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::Close() {
    int_t idwResult;

    LOG_INFO("close sockfd %d", m_idwSockfd);
    m_pTransportLocker->Lock();
    if ((idwResult = shutdown(m_idwSockfd, SHUT_RDWR)) == SOCKET_ERROR) {
        m_pTransportLocker->UnLock();
        LOG_ERROR("shutdown fail error [%d]", errno);
    }
    m_pTransportLocker->UnLock();

    m_pTransportLocker->Lock();
    if ((idwResult = close(m_idwSockfd)) == SOCKET_ERROR) {
        m_pTransportLocker->UnLock();
        LOG_ERROR("close socket %d fail error [%d]", m_idwSockfd, errno);
        return FALSE;
    }
    m_pTransportLocker->UnLock();

    LOG_INFO("closed sockfd %d", m_idwSockfd);

    m_pTransportLocker->Lock();
    m_boIsConnected = FALSE;
    m_boIsClosing = TRUE;
    m_boIsMqttDoneSubcribe = FALSE;
    m_pTransportLocker->UnLock();

    return TRUE;

}

/**
 * @func   IsConnected
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::IsConnected() {
    return m_boIsConnected;
}

/**
 * @func   IsBlocked
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::IsBlocked() {
    return m_boIsBlocked;
}

/**
 * @func   SetNonBlocking
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::SetNonBlocking() {
    return FALSE;
}

/**
 * @func   SetBlocking
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::SetBlocking() {
    return TRUE;
}

/**
 * @func   TransportThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
Transport::SendSocketThreadProc(
    void_p pBuffer
) {
    while (TRUE) {
        m_pTransportLocker->Lock();
        if (!m_boIsConnected || !m_boIsMqttDoneSubcribe) {
            m_pTransportLocker->UnLock();
            continue;
        }
        m_pTransportLocker->UnLock();

        if (IsConnected() && IsWritable(0)) {
            Packet_p pPacket = NULL;
            m_pTransportLocker->Lock();
            if (!m_queTransportPacket.empty()) {
                pPacket = m_queTransportPacket.front();
                if (pPacket != NULL) {
                    int_t iLength = send(m_idwSockfd, pPacket->GetBuffer(), pPacket->Length(), 0);
                    m_queTransportPacket.pop();
                    if (iLength < 0) {
                        LOG_INFO("send data error [%d]", errno);
                    }
//                    else
//                        LOG_DEBUG("send data - m_idwSockfd = %d - [%d byte]", m_idwSockfd, pPacket->Length());
                    delete pPacket;
                    pPacket = NULL;
                }
            }
            m_pTransportLocker->UnLock();
        }
        usleep(50000);
    }
    m_pTransportLocker->UnLock();

    m_pTransportLocker->Lock();
    m_boIsClosing = FALSE;
    m_pTransportLocker->UnLock();

    m_pSendSocketThread->Stop();
    return NULL;
}

/**
 * @func   TransportReadSocketThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
Transport::ReadSocketThreadProc(
    void_p pBuffer
) {
    int_t idwResult;
    while (TRUE) {
        m_pTransportLocker->Lock();
        if (!m_boIsConnected || !m_boIsMqttDoneSubcribe) {
            m_pTransportLocker->UnLock();
            continue;
        }

        //Read socket data -> push to m_pbyBuffer
        idwResult = recv(m_idwSockfd, m_pbyBuffer + m_idwBufferWritePos, BUFFER_SOCKET_SIZE - m_idwBufferWritePos, 0);
        if (idwResult > 0) {
            LOG_DEBUG("receive data - m_idwSockfd = %d - [%d byte]", m_idwSockfd, idwResult);
            m_idwBufferWritePos += idwResult;
        } else if (idwResult == 0) {
            //if recv return 0 meaning that serrver disconnect
            LOG_WARN("receive data - m_idwSockfd = %d - [%d byte]", m_idwSockfd, idwResult);
            m_boIsConnected = FALSE;
            m_pTransportLocker->UnLock();
            continue;
        }

        //if m_idwBufferWritePos == BUFFER_SOCKET_SIZE mean read full buff -> cycle m_idwBufferWritePos to start of m_pbyBuffer
        if (m_idwBufferWritePos == BUFFER_SOCKET_SIZE) {
            m_idwBufferWritePos = 0;
            //If read to end of m_pbyBuffer, there are change has remaining data on socket
            // skip process data -> read one more time for remaining data
            m_pTransportLocker->UnLock();
            continue;
        }
        m_pTransportLocker->UnLock();

        if (m_idwBufferWritePos != m_idwBufferReadPos) {
//            LOG_DEBUG("m_idwBufferWritePos = %d", m_idwBufferWritePos);
//            LOG_DEBUG("m_idwBufferReadPos = %d", m_idwBufferReadPos);
//            if (!m_boIsProcessing) {
//                if (m_pProcessThread->Start()) {
//                    m_boIsProcessing = TRUE;
//                } else {
//                    LOG_ERROR("thread Transport m_pProcessThread fail - %d - %s", errno, strerror(errno));
//                }
//            }
            if (m_pSMQTTRecvFunctor != NULL) {
                (*m_pSMQTTRecvFunctor)();
            }
        }
        usleep(50000);
    }
    m_pTransportLocker->UnLock();

    m_pTransportLocker->Lock();
    m_boIsClosing = TRUE;
    m_boIsStarted = FALSE;
    m_pTransportLocker->UnLock();
    m_pReadSocketThread->Stop();
    return NULL;
}

/**
 * @func   TransportProcessProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p Transport::ProcessProc(void_p) {
    if (m_pSMQTTRecvFunctor != NULL) {
        (*m_pSMQTTRecvFunctor)();
    }
    m_boIsProcessing = FALSE;
    m_pProcessThread->Stop();
    return NULL;
}

/**
 * @func   RecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Transport::RecvFunctor(
    SMQTTFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pSMQTTRecvFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   PushBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
void
Transport::PushBuffer(
    u8_p pByBuffer,
    u32_t dwLength
) {
    m_pTransportLocker->Lock();
    Packet_p pPacket = new Packet(dwLength);
    pPacket->Push(pByBuffer, dwLength);
    m_queTransportPacket.push(pPacket);
    m_pTransportLocker->UnLock();
}

/**
 * @func   GetBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Transport::GetBuffer(
    u8_p pBuffer,
    u32_t idwLen
) {
    //Read data from m_pbyBuffer
    u32_t idwRemainBuffer = m_idwBufferWritePos > m_idwBufferReadPos ? m_idwBufferWritePos - m_idwBufferReadPos : BUFFER_SOCKET_SIZE - m_idwBufferReadPos + m_idwBufferWritePos;
    u32_t idwReadLen = idwLen > idwRemainBuffer ? idwRemainBuffer : idwLen;

    if ((m_idwBufferWritePos > m_idwBufferReadPos) || (idwReadLen + m_idwBufferReadPos < BUFFER_SOCKET_SIZE)) {
        //Not need cycle read from beginning of m_pbyBuffer
        memcpy(pBuffer, m_pbyBuffer + m_idwBufferReadPos, idwReadLen);
        m_idwBufferReadPos += idwReadLen;
    } else {
        //read from m_idwBufferReadPos to end of m_pbyBuffer
        u32_t idwNumByteReadToEndTransportBuffer = BUFFER_SOCKET_SIZE - m_idwBufferReadPos;
        memcpy(pBuffer, m_pbyBuffer + m_idwBufferReadPos, idwNumByteReadToEndTransportBuffer);
        //cycle read from from beginning of m_pbyBuffer
        memcpy(pBuffer + idwNumByteReadToEndTransportBuffer, m_pbyBuffer, idwReadLen - idwNumByteReadToEndTransportBuffer);
        m_idwBufferReadPos = (m_idwBufferReadPos + idwReadLen) - BUFFER_SOCKET_SIZE;
    }

//    LOG_DEBUG("m_idwBufferWritePos = %d", m_idwBufferWritePos);
//    LOG_DEBUG("m_idwBufferReadPos = %d", m_idwBufferReadPos);
    return idwReadLen;

}

/**
 * @func   DiSend
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Transport::DiSend(
    u8_p pbyBuffer,
    u32_t idwLen
) {
    if (IsConnected() && IsWritable(0)) {
        m_pTransportLocker->Lock();
        int idwRet = send(m_idwSockfd, pbyBuffer, idwLen, 0);
        m_pTransportLocker->UnLock();
        return idwRet;
//        return PushBuffer(pbyBuffer, idwLen);
    }
    else return 0;
}

/**
 * @func   DiGet
 * @brief  None
 * @param  None
 * @retval None
 */
int_t
Transport::DiGet(
    u8_p pbyBuffer,
    u32_t idwLen
) {
    if (m_boIsConnected) {
        m_pTransportLocker->Lock();
        int idwRet = recv(m_idwSockfd, pbyBuffer, idwLen, 0);
        m_pTransportLocker->UnLock();
        return idwRet;
//        return GetBuffer(pbyBuffer, idwLen);
    }
    else return 0;
}

bool_t Transport::IsStarted() const {
    return m_boIsStarted;
}

void_t
Transport::setMqttDoneSubcribe() {
    m_pTransportLocker->Lock();
    m_boIsMqttDoneSubcribe = TRUE;
    m_pTransportLocker->UnLock();
}
