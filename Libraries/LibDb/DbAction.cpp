/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbAction.cpp
 * Description: Include file for application
 *
 * Author: TrungTQb
 *
 * Last Changed By:  Author: trungtq
 * Revision:         Revision: 1.0
 * Last Changed:     Date: 2016-10-07 16:10:00 (Fri, 07 Oct 2016)
 *
 ******************************************************************************/
#include "DbAction.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbInit::DbInit(
    DbContext& dbContext,
    IMapTable& mapTable
) : m_dbContext(dbContext),
    m_mapTable(mapTable) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbInit::ActInitMapping(
    IMapTable_p pMapTable
) {
    if (!pMapTable->Initialized) {
        pMapTable->InitMapTable(m_dbContext);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbDrop::DbDrop(
    DbContext& dbContext,
    IMapTable& mapTable
) : m_dbContext(dbContext),
    m_mapTable(mapTable) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbDrop::DropTable(
    const String& strTable
) {
    m_dbContext.Execute("DROP TABLE IF EXISTS " + strTable);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbAction::DbAction(
    DbPtrBase& dbPtrBase,
    IMapTable& mapTable
) : m_pDbPtrBase (&dbPtrBase),
    m_pMapTable (&mapTable),
    m_dwStatementIndex (0) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
IMapTable&
DbAction::GetMapTable() {
    return *m_pMapTable;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbSaveAction::DbSaveAction(
    DbPtrBase& dbPtrBase,
    IMapTable& mapTable,
    SqlStatement_p pStatement,
    u32_t dwColumn
) : DbAction(dbPtrBase, mapTable),
    m_pStatement (pStatement),
    m_dwColumn (dwColumn) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbLoadAction::DbLoadAction(
    DbPtrBase& dbPtrBase,
    IMapTable& mapTable,
    SqlStatement_p pStatement,
    u32_t dwColumn
) : DbAction (dbPtrBase, mapTable),
    m_pStatement (pStatement),
    m_dwColumn (dwColumn) {

}
