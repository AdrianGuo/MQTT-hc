/*
 * Client.cpp
 *
 *  Created on: Oct 20, 2016
 *      Author: taho
 */

#include <Client.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Client::Client() {
    m_idwSockFd = -1;
    m_pSockAddr = NULL;

    m_strType = "";
    m_strMAC = "";

    m_byCount = 0;
    m_boIsAuthenticated = FALSE;
    m_boIsAlive = TRUE;

    m_pClientLocker = new Locker();

    m_strRemainder = "";
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Client::~Client() {
//    delete m_pSockAddr;
//    delete m_pClientLocker;
}

/**
 * @func   SetSocketFd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetSocketFd(int_t idwSocketFd) {
    if(idwSocketFd > 0) {
        m_pClientLocker->Lock();
        m_idwSockFd = idwSocketFd;
        m_pClientLocker->UnLock();
    }
}

/**
 * @func   GetSocketFd
 * @brief  None
 * @param  None
 * @retval None
 */
int_t Client::GetSocketFd() {
    return m_idwSockFd;
}

/**
 * @func   SetSocketAddress
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetSocketAddress(sockaddr_p pSocketAddress) {
    if(pSocketAddress != NULL) {
        m_pSockAddr = pSocketAddress;
    }
}

/**
 * @func   GetSocketAddress
 * @brief  None
 * @param  None
 * @retval None
 */
sockaddr_p
Client::GetSocketAddress() {
    return m_pSockAddr;
}

/**
 * @func   SetMAC
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::SetMAC(String strMAC) {
    m_strMAC = strMAC;
}

/**
 * @func   GetMAC
 * @brief  None
 * @param  None
 * @retval None
 */
String
Client::GetMAC() {
    return m_strMAC;
}

/**
 * @func   SetType
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::SetType(String strType) {
    m_strType = strType;
}

/**
 * @func   GetType
 * @brief  None
 * @param  None
 * @retval None
 */
String
Client::GetType() {
    return m_strType;
}

/**
 * @func   Authenticate
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::Authenticate(
    bool_t boAuthen
) {
    m_boIsAuthenticated = boAuthen;
}

/**
 * @func   IsAuthenticated
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Client::IsAuthenticated() {
    return m_boIsAuthenticated;
}

/**
 * @func   Push
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::Push(
    Packet_p pPacket
) {
    if (pPacket != NULL) {
        m_pClientLocker->Lock();
        m_quePacket.push(pPacket);
        m_pClientLocker->UnLock();
    }
}

/**
 * @func   Front
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p
Client::Front() {
    if(m_quePacket.size() > 0)
        return m_quePacket.front();
    return NULL;
}

/**
 * @func   Pop
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::Pop() {
    if(m_quePacket.size() > 0)
        m_quePacket.pop();
}

/**
 * @func   PendingMessages
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Client::PendingMessages() {
    return m_quePacket.size();
}

/**
 * @func   SetCount
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::SetCount() {
    m_pClientLocker->Lock();
    if(m_byCount > 1) {
        m_byCount = 2;
    } else {
        m_byCount++;
    }
    m_pClientLocker->UnLock();
}

/**
 * @func   IsAuthenMessage
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t Client::IsAuthenMessage() {
    if(m_byCount == 1) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   SetAlive
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Client::SetAlive(
    bool_t boAlive
) {
    m_pClientLocker->Lock();
    m_boIsAlive = boAlive;
    m_pClientLocker->UnLock();
}

/**
 * @func   IsAlive
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Client::IsAlive() {
    return m_boIsAlive;
}
