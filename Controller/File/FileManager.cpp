/*
 * FileManager.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#include "LogPlus.hpp"
#include "JsonFile/JsonFileInfoReq.hpp"
#include "JsonFile/JsonFileInfoRes.hpp"
#include "JsonFile/JsonFileGet.hpp"
#include "JsonFile/JsonFileRes.hpp"
#include "JsonFile/JsonFwInfoReq.hpp"
#include "JsonFile/JsonFwInfoRes.hpp"
#include "JsonFile/JsonFwForce.hpp"
#include "Version.hpp"

#include "File/FileManager.hpp"

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
FileManager::FileManager(
) : m_FileTransfer ()
{
    m_pFileManagerThread = new LThread();
    m_pFileManagerThreadFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileManager::FileManagerThreadProc);
    m_pFileManagerThread->RegThreadFunctor(&m_pFileManagerThreadFunctor);

    m_pLocker = new Locker();

    m_pJsonRecvSession = JsonRecvFileSession::CreateSession();
    m_pJsonRecvSession->MapJsonMessage<JsonFileInfoRes>(JsonFileInfoRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFileRes>(JsonFileRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFwInfoRes>(JsonFwInfoRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFwForce>(JsonFwForce::GetStrCmd());

    RegisterHandler(JsonFileInfoRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFileInfoRes));
    RegisterHandler(JsonFileRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFileRes));
    RegisterHandler(JsonFwInfoRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFwInfoRes));
    RegisterHandler(JsonFwForce::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFwForce));

    m_pJsonSendSession = JsonSendFileSession::CreateSession();
    m_pJsonSendSession->MapJsonMessage<JsonFileInfoReq>(JsonFileInfoReq::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFileGet>(JsonFileGet::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFwInfoReq>(JsonFwInfoReq::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFwInfoRes>(JsonFwInfoRes::GetStrCmd());
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
FileManager::~FileManager() {
    delete m_pJsonRecvSession;
    delete m_pJsonSendSession;
    delete m_pLocker;
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::Start() {
    if (m_pFileManagerThread != NULL) {
    	m_pFileManagerThread->Start();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::CtrllerRecvFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pCtrllerFunctor = pRecvFunctor;
    }
}

/**
 * @func   ZbCtrlllerThreadProc
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileManager::FileManagerThreadProc(
    void_p pInBuffer
) {
    while (TRUE) {
        JsonCommand_p pJsonCommand = NULL;
        m_pLocker->Lock();
        if (!m_queJsonCommand.empty()) {
        	pJsonCommand = m_queJsonCommand.front();
            m_queJsonCommand.pop();
        }
        m_pLocker->UnLock();

        if (pJsonCommand != NULL) {
			String strJsonCommandName = pJsonCommand->GetFullCommand();
			MapHandlerFunctor::const_iterator it = m_mapHandlerFunctor.find(strJsonCommandName);
			if (it != m_mapHandlerFunctor.end()) {
				m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
			}
			delete pJsonCommand;
        }
    }

    pthread_exit(NULL);
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::RegisterHandler(
    String strJsonCommand,
	HandlerFileCmdFunctor_t funcTor
) {
    m_mapHandlerFunctor[strJsonCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::ProcessHandler(
    JsonCommand_p pJsonCommand
) {
	m_pLocker->Lock();
	m_queJsonCommand.push(pJsonCommand);
	m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::HandlerCmdFileInfoRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileInfoRes ==============");
	JsonMessagePtr<JsonFileInfoRes> jsonFileInfoRes = m_pJsonRecvSession->GetJsonMapping<JsonFileInfoRes>();

	if (!jsonFileInfoRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	JsonFileInfoRes::Server_t server = jsonFileInfoRes->ServerInfo();
	m_FileTransfer.SetServerIP(server.host);
	m_FileTransfer.SetServerPort(server.port);
	m_FileTransfer.SetServerUser(server.user);
	m_FileTransfer.SetServerPassword(server.password);
	m_FileTransfer.SetServerLocation(server.path);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::HandlerCmdFileRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileRes ==============");
	JsonMessagePtr<JsonFileRes> jsonFileRes = m_pJsonRecvSession->GetJsonMapping<JsonFileRes>();

	if (!jsonFileRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	JsonFileRes::FileRes_t file = jsonFileRes->FileInfo();
	if(file.type == HC_APP) {
		m_FileTransfer.SetFileLocation(file.path);
		m_FileTransfer.SetMD5(file.md5);
		m_FileTransfer.Do(FileTransfer::Action::UPGRADE_HC);
	}

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::HandlerCmdFwInfoRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileRes ==============");
	JsonMessagePtr<JsonFwInfoRes> jsonFwInfoRes = m_pJsonRecvSession->GetJsonMapping<JsonFwInfoRes>();

	if (!jsonFwInfoRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	String fwVersion = jsonFwInfoRes->Version();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::HandlerCmdFwForce(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFwForce ==============");
	JsonMessagePtr<JsonFwForce> jsonFwForce = m_pJsonRecvSession->GetJsonMapping<JsonFwForce>();

	if (!jsonFwForce->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	JsonFwForce::Firmware_t fw = jsonFwForce->Firmware();
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::PushJsonCommand(
    void_p pInBuffer
) {
    if ((m_pCtrllerFunctor != NULL) && (pInBuffer != NULL)) {
        (*m_pCtrllerFunctor)((JsonCommand_p) pInBuffer);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::RequestServerInfo(
) {
    JsonMessagePtr<JsonFileInfoReq> jsonFileInfoReq = m_pJsonSendSession->GetJsonMapping<JsonFileInfoReq>();
    JsonCommand_p pJsonCommand = jsonFileInfoReq->CreateJsonCommand();
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::RequestFileInfo(
	u8_t fileType,
	String strVersion
) {
	JsonFileGet::FileGet_t fileGet;
	fileGet.type = fileType;
	fileGet.version = strVersion;

    JsonMessagePtr<JsonFileGet> jsonFileGet = m_pJsonSendSession->GetJsonMapping<JsonFileGet>();
    JsonCommand_p pJsonCommand = jsonFileGet->CreateJsonCommand(fileGet);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::RequestLatestFwVersion(
) {
    JsonMessagePtr<JsonFwInfoReq> jsonFwInfoReq = m_pJsonSendSession->GetJsonMapping<JsonFwInfoReq>();
    JsonCommand_p pJsonCommand = jsonFwInfoReq->CreateJsonCommand();
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::ResponseFwVersion(
) {
    JsonMessagePtr<JsonFwInfoRes> jsonFwInfoRes = m_pJsonSendSession->GetJsonMapping<JsonFwInfoRes>();
    JsonCommand_p pJsonCommand = jsonFwInfoRes->CreateJsonCommand(VERSION);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}
