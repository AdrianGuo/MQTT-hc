/*
 * FileTransfer.cpp
 *
 *  Created on: Dec 21, 2016
 *      Author: taho
 */

#include <unistd.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <md5.hpp>
#include <HelperHc.hpp>

#include "File/FileTransfer.hpp"

#define SERVER_IP           "125.212.226.54"
#define SERVER_PORT         "21"
#define SERVER_USERNAME     "ftpuser"
#define SERVER_PASSWORD     "ftpuser"
#define SERVER_LOCATION     "/home/ftpuser/MAC/"

#define STO_LOCATION        "/Lumi/"
#define EXE_LOCATION        "/tmp/"
#define HC_LOCATION         "/Lumi/hc"
#define HC_BAK_LOCATION     "/Lumi/hc.bak"
#define FW_LOCATION         "/tmp/hc.img"
#define UPGRADE_COMMAND     "sysupgrade -c hc.img"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
FileTransfer::FileTransfer(
	String strMAC
) {
	m_strMAC			= strMAC;
    m_strIP         	= SERVER_IP;
    m_strPort       	= SERVER_PORT;
    m_strUser       	= SERVER_USERNAME;
    m_strPassword   	= SERVER_PASSWORD;
    m_strServerFolder 	= SERVER_LOCATION;
    m_strFileLocation 	= "";
    m_strMD5      		= "";
    m_strFullPath		= m_strServerFolder + "/" + m_strMAC;

    m_pBackupThread  = new LThread();
    m_pRestoreThread = new LThread();
    m_pUpgradeHCThread = new LThread();
    m_pUpgradeOSThread = new LThread();
    m_BackupFunctor  = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::Backup);
    m_RestoreFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::Restore);
    m_UpgradeOSFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::UpgradeOS);
    m_UpgradeHCFunctor = makeFunctor((threadFunctor_p) NULL, *this, &FileTransfer::UpgradeHC);
    m_pBackupThread->RegThreadFunctor(&m_BackupFunctor);
    m_pRestoreThread->RegThreadFunctor(&m_RestoreFunctor);
    m_pUpgradeOSThread->RegThreadFunctor(&m_UpgradeOSFunctor);
    m_pUpgradeHCThread->RegThreadFunctor(&m_UpgradeHCFunctor);

    m_pFileTransferTimer = RTimer::getTimerInstance();
    m_FileTransferFunctor = makeFunctor((TimerFunctor_p) NULL, *this, &FileTransfer::HandleFileTransferProcess);

    m_pLocker = new Locker();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
FileTransfer::~FileTransfer() {
    delete m_pBackupThread;
    delete m_pRestoreThread;
    delete m_pUpgradeOSThread;
    delete m_pUpgradeHCThread;
    delete m_pLocker;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool
FileTransfer::Connect() {
    return m_Ftp.Connect((m_strIP + ":" + m_strPort).c_str());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
FileTransfer::Login() {
    return m_Ftp.Login(m_strUser.c_str(), m_strPassword.c_str());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::Logout() {
    m_Ftp.Quit();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerIP(
    String strIP
) {
    m_pLocker->Lock();
    m_strIP = strIP;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerPort(
    String strPort
) {
    m_pLocker->Lock();
    m_strPort = strPort;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerUser(
    String strUser
) {
    m_pLocker->Lock();
    m_strUser = strUser;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerPassword(
    String strPassword
) {
    m_pLocker->Lock();
    m_strPassword = strPassword;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetServerFolder(
    String strServerFolder
) {
    m_pLocker->Lock();
    m_strServerFolder 	= strServerFolder;
    m_strFullPath		= m_strServerFolder + "/" + m_strMAC;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetFileLocation(
    String strFileLocation
) {
    m_pLocker->Lock();
    m_strFileLocation = strFileLocation;
    m_pLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::SetMD5(
    String strMD5
) {
    m_strMD5 = strMD5;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
FileTransfer::CreateSaveFolder(
) {
    if(!Connect()) {
        return FALSE;
    }

    if (Login()) {
    	if(m_Ftp.Mkdir(m_strFullPath.c_str()) != 1)
    		return FALSE;
    	Logout();
    } else {
    	return FALSE;
    }

	return TRUE;
}

/**
 * @func
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
    } else if(action == FileTransfer::Action::UPGRADE_OS) {
        m_pUpgradeOSThread->Start();
    } else if(action == FileTransfer::Action::UPGRADE_HC) {
        m_pUpgradeHCThread->Start();
    }
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::Backup(
    void_p pBuffer
) {
    if(!Connect()) {
        m_pBackupThread->Stop();
        return NULL;
    }

    if (Login()) {
    	if(m_Ftp.Put(HC_LOCATION, m_strFullPath.c_str(), Ftplib::image) == 1)
    		LOG_DEBUG("File uploaded!");
        Logout();
    }

    m_pBackupThread->Stop();
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::Restore(
    void_p pBuffer
) {
    if(!Connect()) {
        m_pRestoreThread->Stop();
        return NULL;
    }

    if (Login()) {

        Logout();
    }

    m_pRestoreThread->Stop();
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::UpgradeHC(
    void_p pBuffer
) {
	LOG_DEBUG("============== UpgradeHC - Start ==============");
    if(!Connect()) {
        m_pUpgradeOSThread->Stop();
        return NULL;
    }

    if (Login()) {
        u8_t byCount = 0;
        remove(HC_BAK_LOCATION);
        while(byCount < 3) {
            if(m_Ftp.Get(HC_BAK_LOCATION, m_strFileLocation.c_str(), Ftplib::image) == 1)
                byCount = 4;
            else
                byCount++;
        }

        if(byCount == 4 &&
                String(md5file(HC_LOCATION).c_str()) == m_strMD5
        ) {
        	LOG_DEBUG("============== UpgradeHC - Downloaded ==============");
            remove(HC_LOCATION);
            CopyFile(HC_BAK_LOCATION, HC_LOCATION);
            chmod(HC_LOCATION, S_IRWXU);
            sync();
            reboot(RB_AUTOBOOT);
        }

        Logout();
    }

    m_pUpgradeOSThread->Stop();
    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p
FileTransfer::UpgradeOS(
    void_p pBuffer
) {
    if(!Connect()) {
        m_pUpgradeOSThread->Stop();
        return NULL;
    }

    if (Login()) {
        u8_t byCount = 0;
        remove(FW_LOCATION);
        while(byCount < 3) {
            if(m_Ftp.Get(FW_LOCATION, m_strServerFolder.c_str(), Ftplib::image) == 1)
                byCount = 4;
            else
                byCount++;
        }

        if(byCount == 4 &&
                String(md5file(FW_LOCATION).c_str()) == m_strMD5
        ) {
            sync();
            system(UPGRADE_COMMAND);
        }
    }

    m_pUpgradeOSThread->Stop();
    return NULL;
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileTransfer::HandleFileTransferProcess(
    void_p pBuffer
) {

}
