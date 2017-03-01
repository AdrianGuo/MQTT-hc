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

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/DbPtr.hpp"
#include "Libraries/LibDb/DbContext.hpp"
#include "Libraries/LibDb/SqlStatement.hpp"

template<typename Result>
struct SqlValue {
    static Result Load(DbContext_p pDbContext, SmartPtr<SqlStatement> pSqlStatement, int_t& iColumn);
};

#endif /* !SQLVALUE_HPP_ */
