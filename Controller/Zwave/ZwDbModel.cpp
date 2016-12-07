/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbModel.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 22:10:12
 *
 ******************************************************************************/
#include "ZwDbModel.hpp"

ZwDbModel* ZwDbModel::m_pInstance = NULL;

/**
 * @func   ZwDbModel
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbModel::ZwDbModel(
    const_char_p cDbName
) : DbContext(cDbName) {
    MapDbTable<ZwDbDeviceItem>(ZwDbDeviceItem::GetTableName());
    MapDbTable<ZwDbCtrllerItem>(ZwDbCtrllerItem::GetTableName());
    MapDbTable<ZwDbCmdClassItem>(ZwDbCmdClassItem::GetTableName());
    CreateTables();

    Devices     = Find<ZwDbDeviceItem>();
    CmdClasses  = Find<ZwDbCmdClassItem>();
    Controller  = Find<ZwDbCtrllerItem>();
}

/**
 * @func   ~ZwDbModel
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbModel::~ZwDbModel() {

}

/**
 * @func   CreateModel
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbModel_p
ZwDbModel::CreateModel(
    const_char_p cDbName
) {
    if (m_pInstance == NULL) {
        m_pInstance = new ZwDbModel(cDbName);
    }
    return m_pInstance;
}
