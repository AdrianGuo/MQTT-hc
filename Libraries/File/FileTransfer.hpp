/*
 * FileTransfer.hpp
 *
 *  Created on: Dec 21, 2016
 *      Author: taho
 */

#ifndef LIBRARIES_FILE_FILETRANSFER_HPP_
#define LIBRARIES_FILE_FILETRANSFER_HPP_

#include "String.hpp"
#include "Ftplib.h"
#include "LThread.hpp"
#include "Locker.hpp"
#include "RTimer.hpp"

#define SERVER_IP           "125.212.226.54"
#define SERVER_PORT         "21"

class FileTransfer {
private:
    Ftplib m_Ftp;
    String m_strIP;
    String m_strPort;
    String m_strUser;
    String m_strPassword;
    String m_strLocation;

    LThread_p m_pBackupThread;
    LThread_p m_pRestoreThread;
    LThread_p m_pUpgradeThread;
    threadFunctor_t m_BackupFunctor;
    threadFunctor_t m_RestoreFunctor;
    threadFunctor_t m_UpgradeFunctor;

    RTimer_p m_pFileTransferTimer;
    timerFunctor_t m_FileTransferFunctor;
    int_t m_iFileTransferHandle;

    Locker_p m_pLocker;

public:
    typedef enum {
        BACKUP = 0,
        RESTORE,
        UPGRADE
    } Action;

    FileTransfer(String strIP = SERVER_IP, String strPort = SERVER_PORT);
    ~FileTransfer();

    bool   Connect();
    bool_t Login();
    void_t Logout();

    void_t SetServerIP(String);
    void_t SetServerPort(String);
    void_t SetServerUser(String);
    void_t SetServerPassword(String);
    void_t SetServerLocation(String);

    void_t Do(FileTransfer::Action);
    void_p Backup(void_p);
    void_p Restore(void_p);
    void_p Upgrade(void_p);
    void_t UpgradeOS();
    void_t UpgradeHC();

    void_t HandleFileTransferProcess(void_p pBuffer);
};

#endif /* LIBRARIES_FILE_FILETRANSFER_HPP_ */
