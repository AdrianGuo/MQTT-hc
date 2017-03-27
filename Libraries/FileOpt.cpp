/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: FileOpt.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 21 Feb 2017 13:22:31
 *
 ******************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>

#include "FileOpt.hpp"

FileOpt_p FileOpt::m_pInstance = NULL;

/**
 * @func   FileOpt
 * @brief  None
 * @param  None
 * @retval None
 */
FileOpt::FileOpt(
) {
}

/**
 * @func   ~FileOpt
 * @brief  None
 * @param  None
 * @retval None
 */
FileOpt::~FileOpt() {
    m_Locker.Lock();
    m_Locker.UnLock();
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
FileOpt&
FileOpt::GetInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new FileOpt();
    }
    return *m_pInstance;
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileOpt::ResetInstance() {
    if (m_pInstance != NULL) {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

/**
 * @func   Rename
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileOpt::Rename(
    String oldName,
    String newName
) {
    m_Locker.Lock();
    rename(oldName.c_str(), newName.c_str());
    m_Locker.UnLock();
}

/**
 * @func   Remove
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileOpt::Remove(
    String fileName
) {
    m_Locker.Lock();
    remove(fileName.c_str());
    m_Locker.UnLock();
}

/**
 * @func   CopyFile
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
FileOpt::CopyFile(
    String srcFile,
    String desFile
) {
    m_Locker.Lock();

    if (srcFile == desFile) {
        m_Locker.UnLock();
        return;
    }

    int_t read_fd;
    int_t write_fd;
    struct stat stat_buf;
    off_t offset = 0;
    read_fd = open(srcFile.c_str(), O_RDONLY);

    if (read_fd == -1) {
        m_Locker.UnLock();
        return;
    }

    fstat (read_fd, &stat_buf);
    write_fd = open(desFile.c_str(), O_WRONLY | O_CREAT, stat_buf.st_mode);

    if (write_fd == -1) {
        m_Locker.UnLock();
        return;
    }

    sendfile (write_fd, read_fd, &offset, stat_buf.st_size);
    close (read_fd);
    close (write_fd);

    m_Locker.UnLock();
}
