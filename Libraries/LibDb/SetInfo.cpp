/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SetInfo.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 4 Mar 2017 09:04:54
 *
 ******************************************************************************/
#include "SetInfo.hpp"

/**
 * @func   SetInfo
 * @brief  None
 * @param  None
 * @retval None
 */
SetInfo::SetInfo(
    const String strTableName,
    const String strForeignKeyName,
    const String strJoinName
) : TableName      (     strTableName),
    ForeignKeyName (strForeignKeyName),
    JoinName       (      strJoinName) {

}

/**
 * @func   ~SetInfo
 * @brief  None
 * @param  None
 * @retval None
 */
SetInfo::~SetInfo() {

}
