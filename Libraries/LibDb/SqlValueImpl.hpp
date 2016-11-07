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
 * Last Changed:     Date: 2016-10-17 01:45:00 (Mon, 17 Oct 2016)
 *
 ******************************************************************************/

#ifndef SQLVALUEIMPL_HPP_
#define SQLVALUEIMPL_HPP_

#include "typedefs.h"
#include "SqlStatement.hpp"
#include "SqlValue.hpp"
#include "DbContext.hpp"
#include "DbContextImpl.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename C>
struct SqlValue<DbPtr<C>> {
    static DbPtr<C> Load(DbContext_p pDbContext, SqlStatement_p pSqlStatement, int_t& iColumn);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename C>
inline DbPtr<C>
SqlValue<DbPtr<C>>::Load(
    DbContext_p pDbContext,
    SqlStatement_p pSqlStatement,
    int_t& iColumn
) {
    return pDbContext->Load<C>(pSqlStatement, iColumn);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<>
struct SqlValue<int_t> {
    static int_t Load(DbContext_p pDbContext, SqlStatement_p pSqlStatement, int_t& iColumn);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline int_t
SqlValue<int_t>::Load(
    DbContext_p pDbContext,
    SqlStatement_p pSqlStatement,
    int_t& iColumn
) {
    int_t iValue = 0;
    pSqlStatement->reset();
    pSqlStatement->execute();
    pSqlStatement->get(iColumn++, &iValue);
    return iValue;
}

#endif /* LIBRARIES_LIBDB_SQLVALUEIMPL_HPP_ */
