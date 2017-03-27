/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: FileOpt.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 21 Feb 2017 13:22:05
 *
 ******************************************************************************/
#ifndef FILE_OPT_HPP_
#define FILE_OPT_HPP_

#include "Typedefs.h"
#include "Locker.hpp"
#include "ValueCore.hpp"

class FileOpt : public ValueCore {
private:
    static FileOpt*     m_pInstance;
    Locker_t            m_Locker;

    FileOpt();
    virtual ~FileOpt();

    FileOpt(const FileOpt& copied) = delete;
    FileOpt& operator= (const FileOpt& copied) = delete;
public:
    static FileOpt& GetInstance();
    static void_t  ResetInstance();

    void_t Rename(String oldName, String newName);
    void_t Remove(String fileName);
    void_t CopyFile(String srcFile, String desFile);
};

typedef FileOpt  FileOpt_t;
typedef FileOpt* FileOpt_p;

#endif /* !FILE_OPT_HPP_ */
