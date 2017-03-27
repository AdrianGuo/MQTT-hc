/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: NCache.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 13 Mar 2017 19:13:16
 *
 ******************************************************************************/
#ifndef NCACHE_HPP_
#define NCACHE_HPP_

#include <algorithm>

#include "../Typedefs.h"
#include "Aggregation.hpp"
#include "DbContext.hpp"

template<class C>
class NCache {
private:
    typedef Vector<DbPtr<C>> VecCache;
    DbContext_p     m_pDbContext;
    String          m_strTable;
    VecCache        m_vecCache;
    VecCache        m_vecInsert;
public:
    NCache(DbContext_p pDbContext, const String& strTable);
    virtual ~NCache();

    template<class UnaryPredicate>
    NCache<C>& Where(UnaryPredicate pred);

    operator Aggregation<DbPtr<C>>() const;
    operator DbPtr<C>() const;
};

/**
 * @func   NCache
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
NCache<C>::NCache(
    DbContext_p pDbContext,
    const String& strTable
) : m_pDbContext (pDbContext),
    m_strTable (strTable) {
    DbContext::MapTable<C>* pMapTable = pDbContext->GetMapping<C>();
    for (typename DbContext::MapTable<C>::Registry_t::const_iterator
            it  = pMapTable->Registry.begin();
            it != pMapTable->Registry.end(); it++) {
        m_vecCache.push_back(DbPtr<C>(it->second));
    }
}

/**
 * @func   ~NCache
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
NCache<C>::~NCache() {

}

/**
 * @func   Where
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<class UnaryPredicate>
NCache<C>&
NCache<C>::Where(
    UnaryPredicate pred
) {
    std::copy_if(m_vecCache.begin(), m_vecCache.end(), std::back_inserter(m_vecInsert), pred);
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
NCache<C>::operator DbPtr<C>(
) const {
    if (m_vecInsert.size() > 0) {
        return m_vecInsert.at(0);
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
NCache<C>::operator Aggregation<DbPtr<C>>(
) const {
    return Aggregation<DbPtr<C>>(m_vecInsert);
}

#endif /* !NCACHE_HPP_ */
