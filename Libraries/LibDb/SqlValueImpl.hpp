/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SqlValueImpl.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-17 01:45:00 (Mon, 17 Oct 2016)
 *
 ******************************************************************************/

#ifndef SQL_VALUE_IMPL_HPP_
#define SQL_VALUE_IMPL_HPP_

#include "../Typedefs.h"
#include "SqlStatement.hpp"
#include "SqlValue.hpp"
#include "DbContext.hpp"
#include "DbContextImpl.hpp"

template<typename C>
struct SqlValue<DbPtr<C>> {
    static DbPtr<C> Load(DbContext_p            pDbContext,
                         SmartPtr<SqlStatement> pSqlStatement,
                         int_t&                 iColumn);
};

/**
 * @func   Load
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename C>
inline DbPtr<C>
SqlValue<DbPtr<C>>::Load(
    DbContext_p             pDbContext,
    SmartPtr<SqlStatement>  pSqlStatement,
    int_t&                  iColumn
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
    static int_t
    Load(DbContext_p pDbContext, SmartPtr<SqlStatement> pSqlStatement, int_t& iColumn);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline int_t
SqlValue<int_t>::Load(
    DbContext_p             pDbContext,
    SmartPtr<SqlStatement>  pSqlStatement,
    int_t&                  iColumn
) {
    int_t iValue = 0;
    pSqlStatement->reset();
    pSqlStatement->execute();
    pSqlStatement->get(iColumn++, &iValue);
    return iValue;
}

#endif /* SQL_VALUE_IMPL_HPP_ */
