#include "debug.hpp"
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SServer::SServer(int_t idw) : m_ServerSock(idw){
    m_pHCCtrllerFunctor = NULL;
    m_SServerFunctor = makeFunctor((SServerFunctor_p) NULL, *this, &SServer::BufferToJsCmdClass);
    m_ServerSock.RecvFunctor(&m_SServerFunctor);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SServer::~SServer() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Serve(){
    return m_ServerSock.Serve();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Close(){
    return m_ServerSock.Close();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::Start(){
    return m_ServerSock.Start();
}

/**
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SServer::BufferToJsCmdClass(
    u8_p pBuffer,
    u32_t dwLen,
    int_t idwClientID /* aka: client sockfd */
) {
    String temp = String(reinterpret_cast<char*>(pBuffer));

    DEBUG2(" recv %s", temp.c_str());

    MapClients_p pClients = m_ServerSock.GetClients();

    String strJsonCommand = (*pClients)[idwClientID].m_strRemainder + temp;
    (*pClients)[idwClientID].m_strRemainder.clear();

    (*pClients)[idwClientID].m_vecStringJsonCommand.clear();
    ParseData(strJsonCommand, idwClientID);

    for (u32_t i = 0; i < ((*pClients)[idwClientID].m_vecStringJsonCommand.size() + 1)/3; i++) {
        String strCmdClass = (*pClients)[idwClientID].m_vecStringJsonCommand[i*3 + 0];
        String strCommand = (*pClients)[idwClientID].m_vecStringJsonCommand[i*3 + 1];
        String strJsonValue = (*pClients)[idwClientID].m_vecStringJsonCommand[i*3 + 2];

        JsonCommand_p pJsonCommand = new JsonCommand(strCmdClass, strCommand, strJsonValue);
        pJsonCommand->SetSrcFlag(JsonCommand::Flag::Client);
        pJsonCommand->SetClientID((u32_t) idwClientID);

        //Process Authen, KAlive
        if((*pClients)[idwClientID].IsAuthenMessage()) {
            if(strCmdClass == String("auth")) {
                Json::Value jsonVal = pJsonCommand->GetJsonOjbect();
                if(!jsonVal.isMember("mac") && !jsonVal.isMember("type")) {
                    m_ServerSock.RemoveClient((*pClients)[idwClientID]);
                    delete pJsonCommand;
                    break;
                }
                (*pClients)[idwClientID].Authenticate(TRUE);
                (*pClients)[idwClientID].SetMAC(String(jsonVal["mac"].asCString()));
                (*pClients)[idwClientID].SetType(String(jsonVal["type"].asCString()));
            }else {
                m_ServerSock.RemoveClient((*pClients)[idwClientID]);
                delete pJsonCommand;
                continue;
            }
        } else {
            if(!(*pClients)[idwClientID].IsAuthenticated()) {
                m_ServerSock.RemoveClient((*pClients)[idwClientID]);
                delete pJsonCommand;
                break;
            }

            if(strCmdClass == String("kalive")) {
                (*pClients)[idwClientID].SetAlive(TRUE);
                delete pJsonCommand;
                pJsonCommand = new JsonCommand(String("kalive"), String("res"), String("{}"));
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
 * @func
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

        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posBegin + STA.length(), posEqual - posBegin - STA.length()));
        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posEqual + DE2.length(), posParen - posEqual - DE2.length()));
        (*pClients)[idwClientID].m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posParen, posEnd - posParen));

        strRawJsonCommand = strRawJsonCommand.substr(posEnd + END.length(), strRawJsonCommand.length());
        (*pClients)[idwClientID].m_strRemainder = strRawJsonCommand;
        ParseData(strRawJsonCommand, idwClientID);
    } else {
        (*pClients)[idwClientID].m_strRemainder += strRawJsonCommand;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SServer::JsCommandToPacket(
    JsonCommand_p pJsonCommandOutput
) {
    String strOutput = STA + pJsonCommandOutput->GetCmdClass() +
                       DE2 + pJsonCommandOutput->GetCommand() +
                       pJsonCommandOutput->GetJsonValue() + END;

    strOutput.remove_char(ENDLN); // remove all '\n' characters
    strOutput.remove_char(SPACE);  // remove all spaces

    DEBUG2(" send %s", strOutput.c_str());

    Packet_p packet = new Packet(strOutput.size());

    void_p pByBuffer = malloc(strOutput.size() + 1);
    memcpy(pByBuffer, &strOutput.element[0], strOutput.size());
    packet->Push((u8_p) pByBuffer, strOutput.size());

    if (pJsonCommandOutput != NULL) {
        delete pJsonCommandOutput;
        pJsonCommandOutput = NULL;
    }

    strOutput.clear();
    (*m_ServerSock.GetClients())[pJsonCommandOutput->GetClientID()].Push(packet);
    return TRUE;
}
