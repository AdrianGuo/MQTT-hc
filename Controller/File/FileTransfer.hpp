/*
 * FileTransfer.hpp
 *
 *  Created on: Dec 21, 2016
 *      Author: taho
 */

#ifndef LIBRARIES_FILE_FILETRANSFER_HPP_
#define LIBRARIES_FILE_FILETRANSFER_HPP_

#include "String.hpp"
#include "LogPlus.hpp"
#include "Ftplib.h"
#include "LThread.hpp"
#include "Locker.hpp"
#include "RTimer.hpp"

#define HC_FW					(1)
#define HC_APP					(2)
#define LC_FW					(3)
#define LC_APP					(4)

class FileTransfer {
private:
    Ftplib m_Ftp;
    String m_strIP;
    String m_strPort;
    String m_strUser;
    String m_strPassword;
    String m_strLocation;
    String m_strFileLocation;
    String m_strMD5;

    LThread_p m_pBackupThread;
    LThread_p m_pRestoreThread;
    LThread_p m_pUpgradeOSThread;
    LThread_p m_pUpgradeHCThread;
    threadFunctor_t m_BackupFunctor;
    threadFunctor_t m_RestoreFunctor;
    threadFunctor_t m_UpgradeOSFunctor;
    threadFunctor_t m_UpgradeHCFunctor;

    RTimer_p m_pFileTransferTimer;
    TimerFunctor_t m_FileTransferFunctor;
    int_t m_iFileTransferHandle;

    Locker_p m_pLocker;

public:
    typedef enum {
        BACKUP = 0,
        RESTORE,
        UPGRADE_OS,
        UPGRADE_HC
    } Action;

    FileTransfer();
    ~FileTransfer();

    bool   Connect();
    bool_t Login();
    void_t Logout();

    void_t SetServerIP(String);
    void_t SetServerPort(String);
    void_t SetServerUser(String);
    void_t SetServerPassword(String);
    void_t SetServerLocation(String);
    void_t SetFileLocation(String);
    void_t SetMD5(String);

    void_t Do(FileTransfer::Action);
    void_p Backup(void_p);
    void_p Restore(void_p);
    void_p UpgradeOS(void_p);
    void_p UpgradeHC(void_p);

    void_t HandleFileTransferProcess(void_p pBuffer);
};

typedef FileTransfer  FileTransfer_t;
typedef FileTransfer* FileTransfer_p;

#endif /* LIBRARIES_FILE_FILETRANSFER_HPP_ */