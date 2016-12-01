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
    m_boIsAlive = FALSE;

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
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t Client::GetSocketFd() {
    return m_idwSockFd;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetSocketAddress(sockaddr_p pSocketAddress) {
    if(pSocketAddress != NULL) {
        m_pClientLocker->Lock();
        m_pSockAddr = pSocketAddress;
        m_pClientLocker->UnLock();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
sockaddr_p Client::GetSocketAddress() {
    return m_pSockAddr;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetMAC(String strMAC) {
    m_strMAC = strMAC;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String Client::GetMAC() {
    return m_strMAC;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetType(String strType) {
    m_strType = strType;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String Client::GetType() {
    return m_strType;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::Authenticate(bool_t boAuthen) {
    m_boIsAuthenticated = boAuthen;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t Client::IsAuthenticated() {
    return m_boIsAuthenticated;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::Push(Packet_p pPacket) {
    if (pPacket != NULL) {
        m_pClientLocker->Lock();
        m_quePacket.push(pPacket);
        m_pClientLocker->UnLock();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p Client::Front() {
    if(m_quePacket.size() > 0)
        return m_quePacket.front();
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::Pop() {
    if(m_quePacket.size() > 0)
        m_quePacket.pop();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t Client::PendingMessages() {
    return m_quePacket.size();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetCount() {
    m_pClientLocker->Lock();
    if(m_byCount > 1)
        m_byCount = 2;
    else
        m_byCount++;
    m_pClientLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t Client::IsAuthenMessage() {
    if(m_byCount == 1)
        return TRUE;
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Client::SetAlive(bool_t boAlive) {
    m_boIsAlive = boAlive;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t Client::IsAlive() {
    m_pClientLocker->Lock();
    return m_boIsAlive;
    m_pClientLocker->UnLock();
}
