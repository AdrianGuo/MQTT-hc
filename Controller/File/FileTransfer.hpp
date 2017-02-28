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

static const String HC_FW 		= String("hcos");
static const String HC_APP 		= String("hcapp");
static const String LC_FW 		= String("lcos");
static const String LC_APP 		= String("lcapp");
static const String DATABASE 	= String("database");
static const String LOG 		= String("log");

class FileTransfer {
private:
    Ftplib m_Ftp;
    String m_strMAC;
    String m_strIP;
    String m_strPort;
    String m_strUser;
    String m_strPassword;
    String m_strServerFolder;
    String m_strFileLocation;
    String m_strMD5;
    String m_strFullPath;

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

    FileTransfer(String strMAC);
    ~FileTransfer();

    bool   Connect();
    bool_t Login();
    void_t Logout();

    void_t SetServerIP(String);
    void_t SetServerPort(String);
    void_t SetServerUser(String);
    void_t SetServerPassword(String);
    void_t SetServerFolder(String);
    void_t SetFileLocation(String);
    void_t SetMD5(String);

    bool_t CreateSaveFolder();

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
