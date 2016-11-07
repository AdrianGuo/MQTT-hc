#include <unistd.h>
#include "debug.hpp"
#include "Functor.hpp"
#include "SClient.hpp"

#ifndef DEBUG_SCLIENT
#define debug_sclient(x)
#else /* DEBUG_SCLIENT */
#define debug_sclient(x)                    DEBUG(x)
#endif /* DEBUG_SCLIENT */

#define SPACE                              (' ')
#define ENDLN                              ('\n')

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SClient::SClient(
    ClientSock_p pClientSock
) {
    m_pClientSock = pClientSock;
    m_SClientSendFunctor = makeFunctor((SClientFunctor_p) NULL, *this, &SClient::BufferToJsCmdClass);
    SClientSendFunctor();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SClient::~SClient() {
    if (m_pClientSock != NULL) {
        delete m_pClientSock;
        m_pClientSock = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Start() {
    if (m_pClientSock != NULL) {
        return m_pClientSock->Start();
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Connect() {
    if (m_pClientSock != NULL) {
        return m_pClientSock->Connect();
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::Close() {
    if (m_pClientSock != NULL) {
        return m_pClientSock->Close();
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
SClient::SClientSendFunctor() {
    if (m_pClientSock != NULL) {
        m_pClientSock->RecvFunctor(&m_SClientSendFunctor);
    }
}

/**
 * @func
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
 * @func
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

        size_t posParen = strRawJsonCommand.substr(posBegin, posEnd).find(DE1); // find {
        size_t posEqual = strRawJsonCommand.substr(posBegin, posParen).find(DE2); // find 1st =
        posBegin = strRawJsonCommand.substr(posBegin, posBegin + posEqual).find(STA); // find 1st $

        m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posBegin + STA.length(), posEqual - posBegin - STA.length()));
        m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posEqual + DE2.length(), posParen - posEqual - DE2.length()));
        m_vecStringJsonCommand.push_back(strRawJsonCommand.substr(posParen, posEnd - posParen));

        strRawJsonCommand = strRawJsonCommand.substr(posEnd + END.length(), strRawJsonCommand.length());
        m_strRemainder = strRawJsonCommand;
        ParseData(strRawJsonCommand);
    }
}

/**
 * @func
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

    DEBUG2(" recv %s", temp.c_str());

    String strJsonCommand = m_strRemainder + temp;
    m_strRemainder.clear();
    m_vecStringJsonCommand.clear();
    ParseData(strJsonCommand);

    for (u32_t i = 0; i < (m_vecStringJsonCommand.size() + 1)/3; i++) {
        String strCmdClass = m_vecStringJsonCommand[i*3 + 0];
        String strCommand = m_vecStringJsonCommand[i*3 + 1];
        String strJsonValue = m_vecStringJsonCommand[i*3 + 2];

        DEBUG2("parse $%s=%s%s$end", strCmdClass.c_str(), strCommand.c_str(), strJsonValue.c_str());

        JsonCommand_p pJsonCommand = new JsonCommand(strCmdClass, strCommand, strJsonValue);

        if (pJsonCommand->IsJsonAvailable()) {
            if (m_pHCCtrllerRecvFunctor != NULL) {
                m_pHCCtrllerRecvFunctor->operator ()(pJsonCommand);
            }
            continue;
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SClient::JsCommandToPacket(
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
    m_pClientSock->PushPacket(packet);

    return TRUE;
}


