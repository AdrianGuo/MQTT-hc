/*
 * FileTransfer.cpp
 *
 *  Created on: Dec 21, 2016
 *      Author: taho
 */

#include <FileTransfer.hpp>

#define SERVER_USERNAME     "ftpuser"
#define SERVER_PASSWORD     "ftpuser"
#define SERVER_LOCATION     "/Lumi/"
#define HC_LOCATION         "/Lumi/"

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
FileTransfer::FileTransfer(
    String strIP,
    String strPort
) {
    m_strIP         = strIP;
    m_strPort       = strPort;
    m_strUser       = SERVER_USERNAME;
    m_strPassword   = SERVER_PASSWORD;
    m_strLocation   = SERVER_LOCATION;

    m_boIsConnected = FALSE;

    m_pBackupThread  = new LThread();
    m_pRestoreThread = new LThread();
    m_pUpgradeThread = new LThread();
    m_BackupFunctor  = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::Backup);
    m_RestoreFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::Restore);
    m_UpgradeFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::Upgrade);
    m_pBackupThread->RegThreadFunctor(&m_BackupFunctor);
    m_pRestoreThread->RegThreadFunctor(&m_RestoreFunctor);
    m_pUpgradeThread->RegThreadFunctor(&m_UpgradeFunctor);

    m_pFileTransferTimer = RTimer::getTimerInstance();
    m_FileTransferFunctor = makeFunctor((timerFunctor_p) NULL, *this, &FileTransfer::HandleFileTransferProcess);
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
FileTransfer::~FileTransfer() {
    delete m_pBackupThread;
    delete m_pRestoreThread;
    delete m_pUpgradeThread;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::Connect() {
    m_boIsConnected = m_Ftp.Connect((m_strIP + ":" + m_strPort).c_str());
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
FileTransfer::Login() {
    return m_Ftp.Login(m_strUser.c_str(), m_strPassword.c_str());
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::Quit() {
    m_Ftp.Quit();
    m_boIsConnected = FALSE;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerIP(
    String strIP
) {
    m_strIP = strIP;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerPort(
    String strPort
) {
    m_strPort = strPort;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerUser(
    String strUser
) {
    m_strUser = strUser;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerPassword(
    String strPassword
) {
    m_strPassword = strPassword;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerLocation(
    String strLocation
) {
    m_strLocation = strLocation;
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::Do(
    FileTransfer::Action action
){
    if(action == FileTransfer::Action::BACKUP) {
        m_pBackupThread->Start();
    } else if(action == FileTransfer::Action::RESTORE) {
        m_pRestoreThread->Start();
    } else if(action == FileTransfer::Action::UPGRADE) {
        m_pUpgradeThread->Start();
    }
}
/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::Backup(
    void_p pBuffer
) {
    if(m_boIsConnected == FALSE) {
        m_pBackupThread->Stop();
        return NULL;
    }
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::Restore(
    void_p pBuffer
) {
    if(m_boIsConnected == FALSE) {
        m_pRestoreThread->Stop();
        return NULL;
    }
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::Upgrade(
    void_p pBuffer
) {
    if(m_boIsConnected == FALSE) {
        m_pUpgradeThread->Stop();
        return NULL;
    }
}

/**
 * @func   ListenThread
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::HandleFileTransferProcess(
    void_p pBuffer
) {

}
