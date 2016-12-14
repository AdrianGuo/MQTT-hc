/*
 * Client.hpp
 *
 *  Created on: Oct 20, 2016
 *      Author: taho
 */
#include <typedefs.h>
#include <String.hpp>
#include <Locker.hpp>
#include <Packet.hpp>
#include <Queue.hpp>
#include <netinet/in.h>

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

typedef struct sockaddr_in                  sockaddr_t;
typedef sockaddr_t*                         sockaddr_p;

class Client {
private:
    int_t m_idwSockFd;
    sockaddr_p m_pSockAddr;

    String m_strType;
    String m_strMAC;

    u8_t m_byCount;
    bool_t m_boIsAuthenticated;
    bool_t m_boIsAlive;

    Queue<Packet_p> m_quePacket;
    Locker_p m_pClientLocker;

public:
    Client();
    ~Client();

    String m_strRemainder;
    Vector<String> m_vecStringJsonCommand;

    void_t SetSocketFd(int_t idwSocketFd);
    int_t GetSocketFd();

    void_t SetSocketAddress(sockaddr_p pSocketAddress);
    sockaddr_p GetSocketAddress();

    void_t SetMAC(String strMAC);
    String GetMAC();

    void_t SetType(String strMAC);
    String GetType();

    void_t Authenticate(bool_t boAuthen);
    bool_t IsAuthenticated();

    void_t Push(Packet_p);
    Packet_p Front();
    void_t Pop();
    u32_t PendingMessages();

    void_t SetCount();
    bool_t IsAuthenMessage();

    void_t SetAlive(bool_t boAlive);
    bool_t IsAlive();
};

typedef Client  Client_t;
typedef Client* Client_p;
#endif /* CLIENT_HPP_ */
