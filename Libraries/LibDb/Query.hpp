/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Query.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-12 00:50:00 (Wen, 12 Oct 2016)
 *
 ******************************************************************************/

#ifndef QUERY_HPP_
#define QUERY_HPP_

#include "Libraries/typedefs.h"
// 17.02.2017 TrungTQ Add
#include "Libraries/Exception.hpp"
#include "Libraries/SmartPtr.hpp"
// 17.02.2017 TrungTQ End
#include "Libraries/LibDb/SqlStatement.hpp"
#include "Libraries/LibDb/DbContext.hpp"

/******************************************************************************/
/*                                   STRUCT                                   */
/******************************************************************************/
/**
 * @func   CreateSelectColumns
 * @brief  None
 * @param  None
 * @retval None
 */
static String
CreateSelectColumns(
    const Vector<ValueDb>& Columns
) {
    String result;

    for (u32_t i = 0; i < Columns.size(); i++) {
        const ValueDb& column = Columns[i];
        if (!result.empty()) {
            result += ", ";
        }
        result += "\"" + column.GetColumnName() + "\"";
    }

    return result;
}

/**
 * @func   CreateQuerySelect
 * @brief  None
 * @param  None
 * @retval None
 */
static String
CreateQuerySelect(
    const String& strFrom,
    const String& strWhere,
    const Vector<ValueDb>& Columns
) {
    String result = "SELECT " + CreateSelectColumns(Columns) + " " + strFrom;

    if (!strWhere.empty()) { result += "WHERE " + strWhere; }

    return result;
}

/**
 * @func   CreateCompleteQuerySelect
 * @brief  None
 * @param  None
 * @retval None
 */
static String
CreateCompleteQuerySelect(
    const String& strSql,
    const String& strWhere
) {
    String result = strSql;

    if (!strWhere.empty()) { result += "WHERE " + strWhere; }

    return result;
}

/**
 * @func   CreateCountQuerySelect
 * @brief  None
 * @param  None
 * @retval None
 */
static String
CreateCountQuerySelect(
    const String& strFrom,
    const String& strWhere
) {
    String result = "SELECT COUNT(1) FROM \"" + strFrom + "\" ";

    if (!strWhere.empty()) {
        result += "WHERE " + strWhere;
    }

    return result;
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C> class Collection;

// R: Result
template<class R>
class Query {
private:
    DbContext_p             m_pDbContext;
    String                  m_strSql;
    String                  m_strTable;
    String                  m_strWhere;
    int_t                   m_iColumn;
    bool_t                  m_boCompleteQuery;
    SmartPtr<SqlStatement>  m_pSqlStatement;
    SmartPtr<SqlStatement>  m_pSqlCountStatement;

    SqlStatement_p SetStatement();
    SqlStatement_p SetCountStatement();

    void_t Reset();

    R SingleResults(const Collection<R>& results) const;

public:
    Query(DbContext_p pDbContext, const String& strSql);
    Query(DbContext_p pDbContext, const String& strTable, const String& strWhere);
    virtual ~Query();

    Query<R>& Where(const String& strWhere);
    template<typename V> Query<R>& Bind(const V& value);

    R ResultValue() const;
    Collection<R> ResultList() const;

    // Conversion Operator
    operator R() const;
    operator Collection<R>() const;
};

/**
 * @func   Query
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::Query(
    DbContext_p pDbContext,
    const String& strSql
) : m_pDbContext        (pDbContext),
    m_strSql            (strSql),
    m_iColumn           (0),
    m_boCompleteQuery   (TRUE) {
    size_t posFrom = m_strSql.find("FROM");
    m_strTable =
    m_strSql.substr(posFrom + String("FROM").length() + 1, m_strSql.length());

    m_pSqlStatement      = SetStatement();
    m_pSqlCountStatement = SetCountStatement();
}

/**
 * @func   Query
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::Query(
    DbContext_p   pDbContext,
    const String& strTable,
    const String& strWhere
) : m_pDbContext      (pDbContext),
    m_strTable        (strTable  ),
    m_strWhere        (strWhere  ),
    m_iColumn         (0),
    m_boCompleteQuery (FALSE) {
    m_strSql = "FROM \"" + strTable + "\" " + strWhere;

    m_pSqlStatement      = SetStatement();
    m_pSqlCountStatement = SetCountStatement();
}

/**
 * @func   ~Query
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::~Query() {

}

/**
 * @func   Reset
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline void_t
Query<R>::Reset() {
    try {
        LOG_DEBUG("query reset");
        m_pSqlStatement->reset();
        m_pSqlCountStatement->reset();
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   SetStatement
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline SqlStatement_p
Query<R>::SetStatement(
) {
    if (m_pDbContext != NULL) {

        Vector<ValueDb> columns;
        m_pDbContext->GetColumns(m_strTable, columns);

        String strQuery = m_boCompleteQuery ?
        CreateCompleteQuerySelect(m_strSql, m_strWhere) :
        CreateQuerySelect(m_strSql, m_strWhere, columns);

        return m_pDbContext->GetStatement(strQuery);

    } else {
        return NULL;
    }
}

/**
 * @func   SetCountStatement
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline SqlStatement_p
Query<R>::SetCountStatement(
) {
    if (m_pDbContext != NULL) {
        String strQuery = CreateCountQuerySelect(m_strTable, m_strWhere);
        return m_pDbContext->GetStatement(strQuery);
    } else {
        return NULL;
    }
}

/**
 * @func   Where
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>&
Query<R>::Where(
    const String& strWhere
) {
    try {
        if (!strWhere.empty()) {
            if (!m_strWhere.empty()) {
                m_strWhere += " AND ";
            }

            m_strWhere += strWhere;
        }

        m_pSqlStatement      = SetStatement();
        m_pSqlCountStatement = SetCountStatement();
        return *this;

    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
        throw Exception(ex.what());
    }
}

/**
 * @func   Bind
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
template<typename V>
inline Query<R>&
Query<R>::Bind(
    const V& value
) {
    try {
        m_pSqlStatement->bind(m_iColumn, value);
        m_pSqlCountStatement->bind(m_iColumn, value);
        m_iColumn++;

        return *this;
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
        throw Exception(ex.what());
    }
}

/**
 * @func   SingleResults
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline R
Query<R>::SingleResults(
    const Collection<R>& results
) const {
    typename Collection<R>::const_iterator it = results.begin();

    if (it == results.end()) {
        return R();
    } else {
        R result = *it;
        return result;
    }
}

/** 
 * @func   resultValue
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline R
Query<R>::ResultValue(
) const {
    return SingleResults(ResultList());
}

/**
 * @func   resultList
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Collection<R>
Query<R>::ResultList(
) const {
    try {
        return Collection<R>(m_pDbContext, m_pSqlStatement, m_pSqlCountStatement);
    } catch (std::exception &ex) {
        throw Exception(ex.what());
    }
}

/**
 * @func   operator R
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::operator R(
) const {
    return ResultValue();
}

/**
 * @func   operator Collection<R>
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline
Query<R>::operator Collection<R>(
) const {
    return ResultList();
}

#endif /* !QUERY_HPP_ */
