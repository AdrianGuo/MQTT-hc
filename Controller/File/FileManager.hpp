/*
 * FileManager.hpp
 *
 *  Created on: Feb 23, 2017
 *      Author: taho
 */

#ifndef CONTROLLER_FILE_FILEMANAGER_HPP_
#define CONTROLLER_FILE_FILEMANAGER_HPP_

#include "Queue.hpp"
#include "JsonCommand.hpp"
#include "LThread.hpp"
#include "ICtrller.hpp"
#include "File/JsonFileSession.hpp"
#include "FileTransfer.hpp"

typedef Functor1_t(JsonCommand_p)   	HandlerFileCmdFunctor_t;
typedef HandlerFileCmdFunctor_t*      	HandlerFileCmdFunctor_p;

class FileManager {
private:
	FileTransfer_t		 	m_FileTransfer;
    LThread_p            	m_pFileManagerThread;
    threadFunctor_t      	m_pFileManagerThreadFunctor;
    CtrllerFunctor_p     	m_pCtrllerFunctor;
    Queue<JsonCommand_p> 	m_queJsonCommand;
    JsonRecvFileSession_p 	m_pJsonRecvSession;
    JsonSendFileSession_p 	m_pJsonSendSession;
    Locker_p m_pLocker;

    typedef Map<String, HandlerFileCmdFunctor_t> MapHandlerFunctor;
    MapHandlerFunctor   m_mapHandlerFunctor;

    void_t RegisterHandler(String strJsonCommand, HandlerFileCmdFunctor_t funcTor);
    void_t HandlerCmdFileInfoRes(JsonCommand_p pJsonCommand);
    void_t HandlerCmdFileRes(JsonCommand_p pJsonCommand);
    void_t HandlerCmdFwInfoRes(JsonCommand_p pJsonCommand);
    void_t HandlerCmdFwForce(JsonCommand_p pJsonCommand);

public:
    FileManager();
    ~FileManager();

    void_t Start();

    void_p FileManagerThreadProc(void_p pInBuffer);
    void_t ProcessHandler(JsonCommand_p);
    void_t CtrllerRecvFunctor(CtrllerFunctor_p pRecvFunctor);

    void_t PushJsonCommand(void_p pBuffer);

    void_t RequestServerInfo();
    void_t RequestFileInfo(u8_t, String);
    void_t RequestLatestFwVersion();
    void_t ResponseFwVersion();
};

typedef FileManager  FileManager_t;
typedef FileManager* FileManager_p;

#endif /* CONTROLLER_FILE_FILEMANAGER_HPP_ */
