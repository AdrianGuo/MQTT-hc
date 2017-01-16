#ifndef SSERVER_HPP_
#define SSERVER_HPP_

#include <TCPServer.hpp>
#include "JsonCommand.hpp"

#define CMDCLASS                                    0
#define CMD                                         1
#define DATA                                        2

typedef Functor1Ret_t(JsonCommand_p, bool_t)        HCCtrllerFunctor_t;
typedef HCCtrllerFunctor_t*                         HCCtrllerFunctor_p;

class SServer {
private:
   TCPServer_t m_ServerSock;
   SServerFunctor_t m_SServerFunctor;
   HCCtrllerFunctor_p m_pHCCtrllerFunctor;

   void_t ParseData(String strRawJsonCommand, int_t idwClientID);

public:
    SServer(int_t idwPort);
    ~SServer();

    bool_t Serve();
    bool_t Close();
    bool_t Start();

    bool_t RecvFunctor(HCCtrllerFunctor_p pRecvFunctor);

    void_t BufferToJsCmdClass(u8_p pBuffer, u32_t dwLen, int_t idwClientID);
    bool_t JsCommandToPacket(JsonCommand_p pJsonCommandOutput);
};

typedef SServer  SServer_t;
typedef SServer* SServer_p;

#endif /* !SSERVER_HPP_ */
