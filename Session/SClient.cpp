#include <unistd.h>
#include "LogPlus.hpp"
#include "Functor.hpp"
#include "SClient.hpp"

#define SPACE                              (' ')
#define ENDLN                              ('\n')
#define AT                                 ('@')

static const String STA = String("$");
static const String END = String("$end");
static const String DE1 = String("{");
static const String DE2 = String("=");

/**
 * @func   SClient
 * @brief  None
 * @param  None
 * @retval None
 */
SClient::SClient(
    const_char_p pChostname,
    int_t idwPort
) : m_ClientSock (pChostname, idwPort) {
    m_ClientSock.SetNonBlocking();
    m_SClientSendFunctor =
    makeFunctor((SClientFunctor_p) NULL, *this, &SClient::BufferToJsCmdClass);
    SClientSendFunctor();
}

/**
 * @func   ~SClient
 * @brief  None
 * @param  None
 * @retval None
 */
SClient::~SClient() {
    m_strRemainder.clear();
    m_vecStringJsonCommand.clear();
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Start() {
    return m_ClientSock.Start();
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Connect() {
    return m_ClientSock.Connect();
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Close() {
    return m_ClientSock.Close();
}

/**
 * @func   SClientSendFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SClient::SClientSendFunctor() {
    m_ClientSock.RecvFunctor(&m_SClientSendFunctor);
}

/**
 * @func   SClientRecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::SClientRecvFunctor(
    HCCtrllerFunctor_p pHCCtrllerRecvFunctor
) {
    if (pHCCtrllerRecvFunctor != NULL) {
        m_pHCCtrllerRecvFunctor = pHCCtrllerRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   ParseData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SClient::ParseData(
    String strRawJsonCommand
) {
    size_t posBegin = 0;
    size_t posEnd = strRawJsonCommand.find(END); // find 1st $end

    if (posEnd != String::npos) { // has $end
        size_t posParen = strRawJsonCommand.substr(posBegin, posEnd).find(DE1); // find 1st {
        size_t posEqual = strRawJsonCommand.substr(posBegin, posParen).find(DE2); // find 1st =
        posBegin = strRawJsonCommand.substr(posBegin, posBegin + posEqual).find(STA); // find 1st $

        m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posBegin + STA.length(), posEqual - posBegin - STA.length()));
        m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posEqual + DE2.length(), posParen - posEqual - DE2.length()));
        m_vecStringJsonCommand.push_back(
        strRawJsonCommand.substr(posParen, posEnd - posParen));

        strRawJsonCommand = strRawJsonCommand.substr(posEnd + END.length(), strRawJsonCommand.length());
        m_strRemainder = strRawJsonCommand;
        ParseData(strRawJsonCommand);
    } else {
        m_strRemainder += strRawJsonCommand;
    }
}

/**
 * @func   BufferToJsCmdClass
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SClient::BufferToJsCmdClass(
    u8_p pBuffer,
    u32_t dwLen
) {
    String temp = String(reinterpret_cast<char*>(pBuffer));

    LOG_INFO(" recv %s", temp.c_str());

    String strJsonCommand = m_strRemainder + temp;
    m_strRemainder.clear();
    m_vecStringJsonCommand.clear();
    ParseData(strJsonCommand);

    for (u32_t i = 0; i < (m_vecStringJsonCommand.size() + 1)/3; i++) {
        String strCmdClass  = m_vecStringJsonCommand[i*3 + 0];
        String strCommand   = m_vecStringJsonCommand[i*3 + 1];
        String strJsonValue = m_vecStringJsonCommand[i*3 + 2];
        String strFullCommand = strCmdClass + "=" + strCommand;

//        LOG_DEBUG("parse $%s%s$end", strFullCommand.c_str(), strJsonValue.c_str());

        JsonCommand_p pJsonCommand = new JsonCommand(strFullCommand, strJsonValue);

        if (pJsonCommand->IsJsonAvailable()) {
            if (m_pHCCtrllerRecvFunctor != NULL) {
                (*m_pHCCtrllerRecvFunctor)(pJsonCommand);
            }
            continue;
        }
    }
}

/**
 * @func   JsCommandToPacket
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::JsCommandToPacket(
    JsonCommand_p pJsonCommand
) {
    String strOutput = STA + pJsonCommand->GetFullCommand() +
                       pJsonCommand->GetJsonValue() + END;

    strOutput.erase(std::remove(strOutput.begin(), strOutput.end(), ENDLN), strOutput.end()); //remove_char(ENDLN); // remove all '\n' characters
    strOutput.erase(std::remove(strOutput.begin(), strOutput.end(), SPACE), strOutput.end()); //remove_char(SPACE); // remove all spaces
    std::replace(strOutput.begin(), strOutput.end(), AT, SPACE);

    LOG_INFO(" send %s", strOutput.c_str());

    Packet_p packet = new Packet(strOutput.size());

    void_p pByBuffer = malloc(strOutput.size() + 1);
    memcpy(pByBuffer, &strOutput[0], strOutput.size());
    packet->Push((u8_p) pByBuffer, strOutput.size());

    if (pJsonCommand != NULL) {
        delete pJsonCommand;
        pJsonCommand = NULL;
    }

    strOutput.clear();
    m_ClientSock.PushPacket(packet);

    return TRUE;
}
