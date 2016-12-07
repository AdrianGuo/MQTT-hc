/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbCmdClass.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 21:18:16
 *
 ******************************************************************************/
#include "ZwDbInfo.hpp"
#include "ZwDbCmdClassItem.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbCmdClassItem::ZwDbCmdClassItem() :
    DbItem (ZWCMDCLASS),
    CmdId   (-1, "CmdId"),
    CmdHex  (String(), "Hex"),
    CmdName (String(), "Name"),
    DevId   (-1, "DevId") {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbCmdClassItem::~ZwDbCmdClassItem() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDbCmdClassItem::GetTableName() {
    return ZWCMDCLASS;
}
