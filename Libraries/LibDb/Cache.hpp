/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Cache.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2 Mar 2017 13:31:54
 *
 ******************************************************************************/
#ifndef CACHE_HPP_
#define CACHE_HPP_

#include <stdlib.h>
#include <algorithm>

#include "../Typedefs.h"
#include "../Exception.hpp"
#include "Aggregation.hpp"
#include "DbContext.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C> class DbFindObj;

#ifndef SPACE
#define SPACE       (' ')
#endif /* SPACE */

// C : Class
template<class C>
class Cache {
private:
    struct ColumnInfo {
        String Name;
        i32_t  Index;

        ColumnInfo(String strName = String(), i32_t iIndex = -1);
        ~ColumnInfo();
    };

    typedef Vector<DbPtr<C>>    VecCache;
    typedef Vector<ColumnInfo>  VecColumn;
    DbContext_p     m_pDbContext;
    u32_t           m_dwIndex;
    String          m_strTable;
    VecCache        m_vecCache;
    VecColumn       m_vecColumn;

//    R SingleResults(const Collection<R>& results) const;
    void_t Parse(const String strWhere);
public:
    Cache(DbContext_p pDbContext, const String& strTable, const String& strWhere);
    virtual ~Cache();

    Cache<C>& Where(const String& strWhere);
    template<typename V> Cache<C>& Bind(const V& value);

    operator Aggregation<DbPtr<C>>() const;
    operator DbPtr<C>() const;
};

/**
 * @func   ColumnInfo::ColumnInfo
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Cache<C>::ColumnInfo::ColumnInfo(
    String strName,
    i32_t  iIndex
) : Name (strName),
    Index (iIndex) {

}

/**
 * @func   ColumnInfo::~ColumnInfo
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Cache<C>::ColumnInfo::~ColumnInfo() {

}

/**
 * @func   Parse
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Cache<C>::Parse(
    const String strWhere
) {
    try {
        static const String _QUESTION( "=?");
        static const String UPPER_AND("AND");

        String strCopy = strWhere;

        strCopy.erase(
        std::remove(strCopy.begin(), strCopy.end(), SPACE), strCopy.end());
        String strUpperCopy = strCopy;
        std::transform(
        strUpperCopy.begin(), strUpperCopy.end(), strUpperCopy.begin(), toupper);

        size_t posBegin = 0;
        size_t posQuestion = strCopy.find(_QUESTION);
        size_t posUpperAnd = strUpperCopy.find(UPPER_AND);

        if (posBegin != String::npos) {
            String strColumn = strWhere.substr(posBegin, posQuestion);
            DbContext::MapTable<C>* pMapTable = m_pDbContext->GetMapping<C>();
            if (pMapTable != NULL) {
                i32_t iIndex = -1;
                for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
                    if (pMapTable->Columns.at(i).GetColumnName() == strColumn) {
                        iIndex = i;
                        break;
                    }
                }

                m_dwIndex++;
                m_vecColumn.push_back(ColumnInfo(strColumn, iIndex));
                if (posUpperAnd != String::npos) {
                    strCopy = strCopy.substr(
                    posUpperAnd + UPPER_AND.length(), strCopy.length());
                    Parse(strCopy);
                }
            }
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   Cache
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Cache<C>::Cache(
    DbContext_p             pDbContext,
    const String&           strTable,
    const String&           strWhere
) : m_pDbContext (pDbContext),
    m_dwIndex (0),
    m_strTable (strTable) {
    DbContext::MapTable<C>* pMapTable = pDbContext->GetMapping<C>();
    for (typename DbContext::MapTable<C>::Registry_t::const_iterator
            it  = pMapTable->Registry.begin();
            it != pMapTable->Registry.end(); it++) {
        m_vecCache.push_back(DbPtr<C>(it->second));
    }

    if (!strWhere.empty()) { Parse(strWhere); }
}

/**
 * @func   ~Query
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Cache<C>::~Cache() {

}

/**
 * @func   Where
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Cache<C>&
Cache<C>::Where(
    const String& strWhere
) {
    if (!strWhere.empty()) { Parse(strWhere); }
    return *this;
}

/**
 * @func   Bind
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<typename V>
inline Cache<C>&
Cache<C>::Bind(
    const V& value
) {
    if (m_vecColumn.size() > 0) {
        DbFindObj<C> dbFind;
        for (typename VecCache::iterator it = m_vecCache.begin(); it != m_vecCache.end(); ) {
            u32_t dwScandIndex = m_vecColumn.size() - m_dwIndex;
            if (!dbFind.Find(it->Obj(), value, m_vecColumn[dwScandIndex].Index)) {
                it = m_vecCache.erase(it);
            } else {
                it++;
            }
        }
        m_dwIndex--;
    }
    return *this;
}

/**
 * @func   operator DbPtr<C>
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Cache<C>::operator DbPtr<C>(
) const {
    if (m_vecCache.size() > 0) {
        return m_vecCache.at(0);
    }
    return DbPtr<C>();
}

/**
 * @func   operator DbPtr<C>
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Cache<C>::operator Aggregation<DbPtr<C>>(
) const {
    return Aggregation<DbPtr<C>>(m_vecCache);
}

#endif /* CACHE_HPP_ */
