/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-28 08:45:00 (Fri, 28 Oct 2016)
 *
 ******************************************************************************/

#ifndef SETINFO_HPP_
#define SETINFO_HPP_

#include "typedefs.h"
#include "String.hpp"

struct SetInfo {
    String TableName;
    String ForeignKeyName;
    String JoinName;
    SetInfo(const String strTableName = String(),
            const String strForeignKeyName = String(),
            const String strJoinName = String()
    ) {
        TableName = strTableName;
        ForeignKeyName = strForeignKeyName;
        JoinName = strJoinName;
    }
};

typedef struct SetInfo  SetInfo_t;
typedef struct SetInfo* SetInfo_p;

#endif /* !SETINFO_HPP_ */
