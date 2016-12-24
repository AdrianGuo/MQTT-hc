#include "debug.hpp"
#include "LogPlus.hpp"
#include "String.hpp"
#include "Client.hpp"

#include "SServer.hpp"

#define SPACE                              (' ')
#define ENDLN                              ('\n')

const String STA = String("$");
const String END = String("$end");
const String DE1 = String("{");
const String DE2 = String("=");

/**
 * @func   SServer
 * @brief  None
 * @param  None
 * @retval None
 */
SServer::SServer(
    int_t idw
) : m_ServerSock(idw){
    m_pHCCtrllerFunctor = NULL;
    m_SServerFunctor =
    makeFunctor((SServerFunctor_p) NULL, *this, &SServer::BufferToJsCmdClass);
    m_ServerSock.RecvFunctor(&m_SServerFunctor);
}

/**
 * @func   ~SServer
 * @brief  None
 * @param  None
 * @retval None
 */
SServer::~SServer() {

}

/**
 * @func   Serve
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Serve(){
    return m_ServerSock.Serve();
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Close(){
    return m_ServerSock.Close();
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Start(){
    return m_ServerSock.Start();
}

/**
 * @func   RecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::RecvFunctor(
    HCCtrllerFunctor_p pHCCtrllerRecvFunctor
) {
    if (pHCCtrllerRecvFunctor != NULL) {
        m_pHCCtrllerFunctor = pHCCtrllerRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   BufferToJsCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SServer::BufferToJsCmdClass(
    u8_p pBuffer,
    u32_t dwLen,
    int_t idwClientId /* aka: client sockfd */
) {
    String temp = String(reinterpret_cast<char*>(pBuffer));

    LOG_INFO("recv %s", temp.c_str());

    MapClients_p pClients = m_ServerSock.GetClients();

    String strJsonCommand = (*pClients)[idwClientId].m_strRemainder + temp;
    (*pClients)[idwClientId].m_strRemainder.clear();
    (*pClients)[idwClientId].m_vecStringJsonCommand.clear();
    ParseData(strJsonCommand, idwClientId);

    for (u32_t i = 0; i < ((*pClients)[idwClientId].m_vecStringJsonCommand.size() + 1)/3; i++) {
        String strCmdClass = (*pClients)[idwClientId].m_vecStringJsonCommand[i*3 + 0];
        String strCommand  = (*pClients)[idwClientId].m_vecStringJsonCommand[i*3 + 1];
        String strJsonValue = (*pClients)[idwClientId].m_vecStringJsonCommand[i*3 + 2];

        JsonCommand_p pJsonCommand = new JsonCommand((strCmdClass + "=" + strCommand), strJsonValue);
        pJsonCommand->SetSrcFlag(JsonCommand::Flag::Client);
        pJsonCommand->SetClientId((u32_t) idwClientId);

        //Process Authen, KAlive
        if((*pClients)[idwClientId].IsAuthenMessage()) {
            if(strCmdClass == String("auth")) {
                Json::Value jsonVal = pJsonCommand->GetJsonOjbect();
                if(!jsonVal.isMember("mac") && !jsonVal.isMember("type")) {
                    m_ServerSock.RemoveClient((*pClients)[idwClientId]);
                    delete pJsonCommand;
                    break;
                }
                (*pClients)[idwClientId].SetAlive(TRUE);
                (*pClients)[idwClientId].Authenticate(TRUE);
                (*pClients)[idwClientId].SetMAC(String(jsonVal["mac"].asCString()));
                (*pClients)[idwClientId].SetType(String(jsonVal["type"].asCString()));
            } else {
                m_ServerSock.RemoveClient((*pClients)[idwClientId]);
                delete pJsonCommand;
                continue;
            }
        } else {
            if(!(*pClients)[idwClientId].IsAuthenticated()) {
                m_ServerSock.RemoveClient((*pClients)[idwClientId]);
                delete pJsonCommand;
                break;
            }

            if(strCmdClass == String("kalive")) {
                (*pClients)[idwClientId].SetAlive(TRUE);
                delete pJsonCommand;
                pJsonCommand = new JsonCommand(String("kalive=res"), String("{}"));
                pJsonCommand->SetClientId(idwClientId);
                JsCommandToPacket(pJsonCommand);
                continue;
            }
        }
        if (pJsonCommand->IsJsonAvailable()) {
            if (m_pHCCtrllerFunctor != NULL) {
                m_pHCCtrllerFunctor->operator ()(pJsonCommand);
            }
            continue;
        }
    }

    pClients = NULL;
}

/**
 * @func   ParseData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SServer::ParseData(
    String strRawJsonCommand,
    int_t idwClientID
) {
    MapClients_p pClients = m_ServerSock.GetClients();
    size_t posBegin = 0;
    size_t posEnd = strRawJsonCommand.find(END); // find 1st $end

    if (posEnd != String::npos) { // has $end

        size_t posParen = strRawJsonCommand.substr(posBegin, posEnd).find(DE1); // find {
        size_t posEqual = strRawJsonCommand.substr(posBegin, posParen).find(DE2); // find 1st =
        posBegin = strRawJsonCommand.substr(posBegin, posBegin + posEqual).find(STA); // find 1st $

        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posBegin + STA.length(), posEqual - posBegin - STA.length()));
        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posEqual + DE2.length(), posParen - posEqual - DE2.length()));
        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posParen, posEnd - posParen));

        strRawJsonCommand = strRawJsonCommand.substr(posEnd + END.length(), strRawJsonCommand.length());
        (*pClients)[idwClientID].m_strRemainder = strRawJsonCommand;
        ParseData(strRawJsonCommand, idwClientID);
    } else {
        (*pClients)[idwClientID].m_strRemainder += strRawJsonCommand;
    }
}

/**
 * @func   JsCommandToPacket
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::JsCommandToPacket(
    JsonCommand_p pJsonCommand
) {
    String strOutput = STA + pJsonCommand->GetFullCommand() +
                       pJsonCommand->GetJsonValue() + END;

    strOutput.remove_char(ENDLN); // remove all '\n' characters
    strOutput.remove_char(SPACE);  // remove all spaces

    LOG_INFO("SServer: send %s", strOutput.c_str());

    Packet_p packet = new Packet(strOutput.size());

    void_p pByBuffer = malloc(strOutput.size() + 1);
    memcpy(pByBuffer, &strOutput.element[0], strOutput.size());
    packet->Push((u8_p) pByBuffer, strOutput.size());

    strOutput.clear();
    if(pJsonCommand->GetClientId() != 0)
        (*m_ServerSock.GetClients())[pJsonCommand->GetClientId()].Push(packet);

    if (pJsonCommand != NULL) {
        delete pJsonCommand;
        pJsonCommand = NULL;
    }
    return TRUE;
}
