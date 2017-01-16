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
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef SQLVALUE_HPP_
#define SQLVALUE_HPP_

#include "typedefs.h"
#include "DbPtr.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"

template<typename Result>
struct SqlValue {
    static Result Load(DbContext_p pDbContext, SqlStatement_p pSqlStatement, int_t& iColumn);
};

#endif /* !SQLVALUE_HPP_ */
