/*
 * FileManager.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#include "LogPlus.hpp"
#include "Version.hpp"

#include "File/FileManager.hpp"

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
FileManager::FileManager(
	String strMAC
) : m_FileTransfer (strMAC)
{
    m_pFileManagerThread = new LThread();
    m_pFileManagerThreadFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileManager::FileManagerThreadProc);
    m_pFileManagerThread->RegThreadFunctor(&m_pFileManagerThreadFunctor);

    m_pLocker = new Locker();

    m_pJsonRecvSession = JsonRecvFileSession::CreateSession();
    m_pJsonRecvSession->MapJsonMessage<JsonFileAccRes>(JsonFileAccRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFileGetRes>(JsonFileGetRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFwVerRes>(JsonFwVerRes::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonFwForce>(JsonFwForce::GetStrCmd());

    RegisterHandler(JsonFileAccRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFileAccRes));
    RegisterHandler(JsonFileGetRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFileGetRes));
    RegisterHandler(JsonFwVerRes::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFwVerRes));
    RegisterHandler(JsonFwForce::GetStrCmd(), makeFunctor((HandlerFileCmdFunctor_p) NULL, *this, &FileManager::HandlerCmdFwForce));

    m_pJsonSendSession = JsonSendFileSession::CreateSession();
    m_pJsonSendSession->MapJsonMessage<JsonFileAccReq>(JsonFileAccReq::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFileGetReq>(JsonFileGetReq::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFwVerReq>(JsonFwVerReq::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonFwVerRes>(JsonFwVerRes::GetStrCmd());
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
        usleep(50000);
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
FileManager::HandlerCmdFileAccRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileInfoRes ==============");
	JsonMessagePtr<JsonFileAccRes> jsonFileInfoRes = m_pJsonRecvSession->GetJsonMapping<JsonFileAccRes>();

	if (!jsonFileInfoRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	JsonFileAccRes::Server_t server = jsonFileInfoRes->ServerInfo();
	m_FileTransfer.SetServerIP(server.host);
	m_FileTransfer.SetServerPort(server.port);
	m_FileTransfer.SetServerUser(server.user);
	m_FileTransfer.SetServerPassword(server.password);
	m_FileTransfer.SetServerFolder(server.path);

	m_FileTransfer.CreateSaveFolder();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileManager::HandlerCmdFileGetRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileRes ==============");
	JsonMessagePtr<JsonFileGetRes> jsonFileRes = m_pJsonRecvSession->GetJsonMapping<JsonFileGetRes>();

	if (!jsonFileRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

	JsonFileGetRes::FileInfo_t file = jsonFileRes->FileInfo();
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
FileManager::HandlerCmdFwVerRes(
	JsonCommand_p pJsonCommand
) {
	LOG_DEBUG("============== HandlerCmdFileRes ==============");
	JsonMessagePtr<JsonFwVerRes> jsonFwInfoRes = m_pJsonRecvSession->GetJsonMapping<JsonFwVerRes>();

	if (!jsonFwInfoRes->ParseJsonCommand(pJsonCommand)) {
		return;
	}

//	String fwVersion = jsonFwInfoRes->Version();
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
    JsonMessagePtr<JsonFileAccReq> jsonFileInfoReq = m_pJsonSendSession->GetJsonMapping<JsonFileAccReq>();
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
	JsonFileGetReq::FileInfo_t fileGet;
	fileGet.type = fileType;
	fileGet.version = strVersion;

    JsonMessagePtr<JsonFileGetReq> jsonFileGet = m_pJsonSendSession->GetJsonMapping<JsonFileGetReq>();
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
	String strType
) {
    JsonMessagePtr<JsonFwVerReq> jsonFwInfoReq = m_pJsonSendSession->GetJsonMapping<JsonFwVerReq>();
    JsonCommand_p pJsonCommand = jsonFwInfoReq->CreateJsonCommand(strType);
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
	JsonFwVerRes::VerInfo_t verInfo
) {
    JsonMessagePtr<JsonFwVerRes> jsonFwInfoRes = m_pJsonSendSession->GetJsonMapping<JsonFwVerRes>();
    JsonCommand_p pJsonCommand = jsonFwInfoRes->CreateJsonCommand(verInfo);
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
FileManager::ResponseFilePut(
	JsonFilePut::FileInfo_t fileInfo
) {
    JsonMessagePtr<JsonFilePut> jsonFwInfoRes = m_pJsonSendSession->GetJsonMapping<JsonFilePut>();
    JsonCommand_p pJsonCommand = jsonFwInfoRes->CreateJsonCommand(fileInfo);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}
