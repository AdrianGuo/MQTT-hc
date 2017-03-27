/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SetInfo.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-28 08:45:00 (Fri, 28 Oct 2016)
 *
 ******************************************************************************/

#ifndef SETINFO_HPP_
#define SETINFO_HPP_

#include "../Typedefs.h"

struct SetInfo {
    String TableName;
    String ForeignKeyName;
    String JoinName;

    SetInfo(const String strTableName      = String(),
            const String strForeignKeyName = String(),
            const String strJoinName       = String());

    ~SetInfo();
};

typedef struct SetInfo  SetInfo_t;
typedef struct SetInfo* SetInfo_p;

#endif /* !SETINFO_HPP_ */
