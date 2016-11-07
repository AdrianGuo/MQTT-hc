#ifndef SCLIENT_HPP
#define SCLIENT_HPP

#include "typedefs.h"
#include "Vector.hpp"
#include "ClientSock.hpp"
#include "Vector.hpp"
#include "String.hpp"
#include "JsonCommand.hpp"

#define CMDCLASS                                    0
#define CMD                                         1
#define DATA                                        2

typedef Functor1Ret_t(JsonCommand_p, bool_t)        HCCtrllerFunctor_t;
typedef HCCtrllerFunctor_t*                         HCCtrllerFunctor_p;

const String STA = String("$");
const String END = String("$end");
const String DE1 = String("{");
const String DE2 = String("=");

class SClient {
private:
    ClientSock_p m_pClientSock;
    String m_strRemainder;
    SClientFunctor_t m_SClientSendFunctor;
    HCCtrllerFunctor_p m_pHCCtrllerRecvFunctor;
    Vector<String> m_vecStringJsonCommand;

    void_t ParseData(String strRawJsonCommand);
public:
    SClient(ClientSock_p pClientSock = NULL);
    ~SClient();

    void_t SClientSendFunctor();
    bool_t SClientRecvFunctor(HCCtrllerFunctor_p pRecvFunctor);

    bool_t Start();
    bool_t Connect();
    bool_t Close();

    void_t BufferToJsCmdClass(u8_p pBuffer, u32_t dwLen);
    bool_t JsCommandToPacket(JsonCommand_p pJsonCommandOutput);
};

typedef SClient  SClient_t;
typedef SClient* SClient_p;


#endif /* !SCLIENT_HPP */
