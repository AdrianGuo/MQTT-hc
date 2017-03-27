/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SqlValue.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef SQLVALUE_HPP_
#define SQLVALUE_HPP_

#include "../Typedefs.h"
#include "DbPtr.hpp"
#include "DbContext.hpp"
#include "SqlStatement.hpp"

template<typename Result>
struct SqlValue {
    static Result Load(DbContext_p pDbContext, SmartPtr<SqlStatement> pSqlStatement, int_t& iColumn);
};

#endif /* !SQLVALUE_HPP_ */
