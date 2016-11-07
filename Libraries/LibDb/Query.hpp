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
 * Last Changed:     Date: 2016-10-12 00:50:00 (Wen, 12 Oct 2016)
 *
 ******************************************************************************/

#ifndef QUERY_HPP_
#define QUERY_HPP_

#include "typedefs.h"
#include "Vector.hpp"
#include "String.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"

/******************************************************************************/
/*                                   STRUCT                                   */
/******************************************************************************/
struct Param {
    virtual ~Param() {}
    virtual void_t BindValue(SqlStatement_p pSqlStatement, int_t iCloumn) = 0;
};

typedef struct Param  Param_t;
typedef struct Param* Param_p;

template<typename V>
struct VParam : public Param {
    VParam(V value) : m_value(value) {}
    virtual ~VParam() {}
    V GetValue() { return m_value; }
    void_t SetValue(V value) { m_value = value; }
    virtual void_t BindValue(SqlStatement_p pSqlStatement, int_t iCloumn) {
        pSqlStatement->bind(iCloumn, m_value);
    }

private:
    V m_value;
};

/**
 * @func
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
 * @func
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
 * @func
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
 * @func
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
    Vector<Param_p> m_vParams;
    DbContext_p     m_pDbContext;
    String          m_strSql;
    String          m_strTable;
    String          m_strWhere;
    bool_t          m_boCompleteQuery;

    SqlStatement_p SetStatement() const;
    SqlStatement_p SetCountStatement() const;
    R SingleResults(const Collection<R>& results) const;

public:
    Query(DbContext_p pDbContext, const String& strSql);
    Query(DbContext_p pDbContext, const String& strTable, const String& strWhere);
    virtual ~Query() {}

    Query<R>& Where(const String& strWhere);
    template<typename V> Query<R>& Bind(const V& value);

    R resultValue() const;
    Collection<R> resultList() const;

    // Conversion Operator
    operator R() const;
    operator Collection<R>() const;
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::Query(
    DbContext_p pDbContext,
    const String& strSql
) : m_pDbContext (pDbContext),
    m_strSql (strSql),
    m_boCompleteQuery (TRUE) {
    size_t posFrom = m_strSql.find("FROM");
    m_strTable = m_strSql.substr(posFrom + String("FROM").length() + 1, m_strSql.length());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::Query(
    DbContext_p pDbContext,
    const String& strTable,
    const String& strWhere
) : m_pDbContext (pDbContext),
    m_strTable (strTable),
    m_strWhere (strWhere),
    m_boCompleteQuery (FALSE) {
    m_strSql = "FROM \"" + strTable + "\" " + strWhere;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline SqlStatement_p
Query<R>::SetStatement() const {
    Vector<ValueDb> columns;
    m_pDbContext->GetColumns(m_strTable, columns);

    String strQuery = m_boCompleteQuery ?
            CreateCompleteQuerySelect(m_strSql, m_strWhere) :
            CreateQuerySelect(m_strSql, m_strWhere, columns);

    return m_pDbContext->GetStatement(strQuery);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline SqlStatement_p
Query<R>::SetCountStatement() const {
    String strQuery = CreateCountQuerySelect(m_strTable, m_strWhere);

    return m_pDbContext->GetStatement(strQuery);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>&
Query<R>::Where(
    const String& strWhere
) {
    if (!strWhere.empty()) {
        if (!m_strWhere.empty()) {
            m_strWhere += " AND";
        }

        m_strWhere += "(" + strWhere + ")";
    }
    return *this;
}

/**
 * @func
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
    m_vParams.push_back(new VParam<V> (value));

    return *this;
}

/**
 * @func
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
    R result = *it;

    return result;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline R
Query<R>::resultValue() const {
    return SingleResults(resultList());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Collection<R>
Query<R>::resultList() const {
    SqlStatement_p pSqlStatement = SetStatement();
    SqlStatement_p pSqlCountStatement = SetCountStatement();

    for (u32_t i = 0; i < m_vParams.size(); i++) {
        m_vParams[i]->BindValue(pSqlStatement, i);
        m_vParams[i]->BindValue(pSqlCountStatement, i);
    }

    return Collection<R>(m_pDbContext, pSqlStatement, pSqlCountStatement);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>::operator R() const {
    return resultValue();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline
Query<R>::operator Collection<R>() const {
    return resultList();
}

#endif /* !QUERY_HPP_ */
