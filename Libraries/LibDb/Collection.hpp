/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Collection.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-18 10:30:00 (Tue, 18 Oct 2016)
 *
 ******************************************************************************/

#ifndef COLLECTION_HPP_
#define COLLECTION_HPP_

#include <algorithm>
#include "../Typedefs.h"
#include "../LogPlus.hpp"
#include "../SmartPtr.hpp"
#include "SqlValueImpl.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"

// C: DbPtr<>
template<class C>
class Collection {
private:
    int_t                   m_iSize;
    DbPtrBase_p             m_pDbPtrBase;
    DbContext_p             m_pDbContext;
    SmartPtr<SqlStatement>  m_pSqlStatement;
    SmartPtr<SqlStatement>  m_pSqlCountStatement;

    SmartPtr<SqlStatement>  ExecuteStatement() const;
public:
    typedef C                   value_type;
    typedef value_type          key_type;
    typedef const value_type    const_value_type;
    typedef std::size_t         size_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;

    Collection(DbContext_p              pDbContext = NULL,
               SmartPtr<SqlStatement>   pSqlStatement = NULL,
               SmartPtr<SqlStatement>   pSqlCountStatement = NULL);

    Collection(const Collection<C>& copied);
    Collection& operator= (const Collection<C>& copied);

    virtual ~Collection();

    class iterator : public std::iterator<std::input_iterator_tag, C> {
    public:
        iterator();
        iterator(const Collection<C>& collection, SmartPtr<SqlStatement> pSqlStatement);
        iterator(const iterator& copied);

        struct core_impl {
            const Collection<C>&    m_collection;
            SmartPtr<SqlStatement>  m_pSqlStatement;
            value_type              m_current;
            int_t                   m_iCount;
            bool_t                  m_boIsLast;

            core_impl(
                const Collection<C>&    collection,
                SmartPtr<SqlStatement>  pSqlStatement);
            ~core_impl();

            value_type& Current();
            void_t   SetCurrent();
        };

        ~iterator();

        iterator& operator= (const iterator& copied);

        C& operator* ();
        C* operator->();

        bool_t operator== (const iterator& other) const;
        bool_t operator!= (const iterator& other) const;

        iterator& operator++ ();
        iterator& operator++ (int_t);
    private:
        SmartPtr<core_impl> m_pImpl;
    };

    class const_iterator : public std::iterator<std::input_iterator_tag, C> {
    private:
        typename Collection<C>::iterator m_impl;

    public:
        const_iterator();
        const_iterator(const Collection<C>&     collection,
                       SmartPtr<SqlStatement>   pSqlStatement);
        const_iterator(const const_iterator& copied);
        const_iterator(const typename Collection<C>::iterator& copied);

        ~const_iterator();

        const_iterator& operator= (const const_iterator& copied);
        const_iterator& operator= (const iterator& copied);

        const C* operator->();
        C operator*();

        bool_t operator== (const const_iterator& other) const;
        bool_t operator!= (const const_iterator& other) const;

        const_iterator operator++ ();
        const_iterator operator++ (int_t);
    };

    void_t insert(C c);
    void_t erase(C c);

    bool_t empty() const;
    void_t clear();

    int_t size() const;
    int_t resize();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end()   const;

    DbContext_p GetContext() const;
    void_t SetRelationData(DbPtrBase_p pDbPtrBase);
};

/**
 * @func   Collection
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Collection<C>::Collection(
    DbContext_p             pDbContext,
    SmartPtr<SqlStatement>  pSqlStatement,
    SmartPtr<SqlStatement>  pSqlCountStatement
) : m_pDbPtrBase         (NULL),
    m_pDbContext         (pDbContext),
    m_pSqlStatement      (pSqlStatement),
    m_pSqlCountStatement (pSqlCountStatement) {
    try {
        int_t iSize = 0;
        m_iSize = -1;

        m_pSqlCountStatement->reset();
        m_pSqlCountStatement->execute();
        m_pSqlCountStatement->get(0, &iSize);
        m_iSize = iSize;
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   Collection
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::Collection(
    const Collection<C>& copied
) : m_iSize         (copied.m_iSize),
    m_pDbPtrBase    (copied.m_pDbPtrBase),
    m_pDbContext    (copied.m_pDbContext),
    m_pSqlStatement (copied.m_pSqlStatement),
    m_pSqlCountStatement (copied.m_pSqlCountStatement) {
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Collection<C>&
Collection<C>::operator= (
    const Collection<C>& copied
) {
    m_iSize         = copied.m_iSize;
    m_pDbContext    = copied.m_pDbContext;
    m_pSqlStatement = copied.m_pSqlStatement;
    m_pSqlCountStatement = copied.m_pSqlCountStatement;

    return *this;
}

/**
 * @func   ~Collection
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::~Collection() {
}

/**
 * @func   SetRelationData
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::SetRelationData(
    DbPtrBase_p pDbPtrBase
) {
    m_pDbPtrBase = pDbPtrBase;
    m_pDbContext = pDbPtrBase->GetDbContext();
}

/**
 * @func   ExecuteStatement
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline SmartPtr<SqlStatement>
Collection<C>::ExecuteStatement(
) const {
    return m_pSqlStatement;
}

/**
 * @func   insert
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::insert(
    C c
) {

}

/**
 * @func   erase
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::erase(
    C c
) {

}

/**
 * @func   empty
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::empty(
) const {
    return size() == 0;
}

/**
 * @func   clear
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::clear(
) {
    if (m_pDbPtrBase != NULL) {
        String sql = m_pSqlStatement->sql();
        size_t fromPos = sql.find(" FROM ");
        String deleteSql = "DELETE" + sql.substr(fromPos);
        SmartPtr<SqlStatement> pSqlStatement =
        m_pDbContext->GetStatement(deleteSql);
        m_pDbPtrBase->BindId(pSqlStatement, 0);
        m_pDbContext->ExecuteSql(deleteSql);
    }
}

/**
 * @func   size
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline int_t
Collection<C>::size(
) const {
    if (m_iSize != -1) { return m_iSize; }

    return 0;
}

/**
 * @func   resize
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline int_t
Collection<C>::resize(
) {
    int_t iCount = -1;

    m_pSqlCountStatement->reset();
    m_pSqlCountStatement->execute();
    m_pSqlCountStatement->get(0, &iCount);
    m_iSize = iCount;

    return iCount;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator
Collection<C>::begin(
) {
    return iterator(*this, ExecuteStatement());
}

/**
 * @func   end
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator
Collection<C>::end() {
    return iterator();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::begin(
) const {
    return const_iterator(*this, ExecuteStatement());
}

/**
 * @func   end
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::end(
) const {
    return const_iterator();
}

/**
 * @func   GetContext
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbContext_p
Collection<C>::GetContext(
) const {
    return m_pDbContext;
}

/**
 * @func   const_iterator::const_iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
) : m_impl() {

}

/**
 * @func   const_iterator::~const_iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::~const_iterator() {

}

/**
 * @func   const_iterator::const_iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const Collection<C>&    collection,
    SmartPtr<SqlStatement>  pSqlStatement
) : m_impl (collection, pSqlStatement) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const const_iterator& copied
) : m_impl (copied.m_impl) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::const_iterator::const_iterator(
    const typename Collection<C>::iterator& copied
) : m_impl (copied) {

}

/**
 * @func   const_iterator::operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator&
Collection<C>::const_iterator::operator= (
    const const_iterator& copied
) {
    m_impl = copied.m_impl;

    return *this;
}

/**
 * @func   const_iterator::operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator&
Collection<C>::const_iterator::operator= (
    const iterator& copied
) {
    m_impl = copied;

    return *this;
}

/**
 * @func   const_iterator::operator==
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::const_iterator::operator== (
    const const_iterator& other
) const {
    return m_impl == other.m_impl;
}

/**
 * @func   const_iterator::operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::const_iterator::operator!= (
    const const_iterator& other
) const {
    return m_impl != other.m_impl;
}

/**
 * @func   const_iterator::operator++
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::const_iterator::operator++ () {
    ++m_impl;

    return *this;
}

/**
 * @func   const_iterator::operator++
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::const_iterator
Collection<C>::const_iterator::operator++(int_t) {
    m_impl++;

    return *this;
}

/**
 * @func   const_iterator::operator->()
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
const C*
Collection<C>::const_iterator::operator->() {
    return m_impl.operator ->();
}

/**
 * @func   const_iterator::operator*()
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C
Collection<C>::const_iterator::operator*() {
    return m_impl.operator *();
}

/**
 * @func   iterator::iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator(
) : m_pImpl (NULL) {

}

/**
 * @func   iterator::iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator(
    const Collection<C>&    collection,
    SmartPtr<SqlStatement>  pSqlStatement
) {
    m_pImpl = new core_impl(collection, pSqlStatement);
}

/**
 * @func   iterator::iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::iterator(
    const iterator& copied
) : m_pImpl (copied.m_pImpl) {

}

/**
 * @func   iterator::~iterator
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::~iterator() {
}

/**
 * @func   iterator::operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator= (
    const iterator& copied
) {
    m_pImpl = copied.m_pImpl;
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C&
Collection<C>::iterator::operator* () {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) {
        return m_pImpl->Current();
    }
    return *(new C());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
Collection<C>::iterator::operator->() {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) {
        return &m_pImpl->Current();
    }
    return new C();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::iterator::operator== (
    const iterator& other
) const {
    if ((m_pImpl == other.m_pImpl) ||
        ((m_pImpl == NULL) && other.m_pImpl->m_boIsLast) ||
        (m_pImpl->m_boIsLast && (other.m_pImpl == NULL))) {
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
Collection<C>::iterator::operator!= (
    const iterator& other
) const {
    return !(*this == other);
}

/**
 * @func   iterator::operator++
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator++ () {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) {
        m_pImpl->SetCurrent();
    }

    return *this;
}

/**
 * @func   iterator::operator++
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::iterator&
Collection<C>::iterator::operator++ (int_t) {
    if ((m_pImpl != NULL) && !m_pImpl->m_boIsLast) {
        m_pImpl->SetCurrent();
    }

    return *this;
}

/**
 * @func   iterator::core_impl::core_impl
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::core_impl::core_impl(
    const Collection<C>&    collection,
    SmartPtr<SqlStatement>  pSqlStatement
) : m_collection    (   collection),
    m_pSqlStatement (pSqlStatement),
    m_iCount   (0),
    m_boIsLast (FALSE) {
    m_pSqlStatement->reset();
    SetCurrent();
}

/**
 * @func   iterator::core_impl::~core_impl
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
Collection<C>::iterator::core_impl::~core_impl() {
}

/**
 * @func   iterator::core_impl::Current
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename Collection<C>::value_type&
Collection<C>::iterator::core_impl::Current() {
    return m_current;
}

/**
 * @func   iterator::core_impl::SetCurrent
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
Collection<C>::iterator::core_impl::SetCurrent(
) {
    if ((m_iCount == m_collection.size()) || m_boIsLast) {
        m_boIsLast = TRUE;
        return;
    }

    DbContext_p pDbContext = m_collection.GetContext();

    if (m_iCount < m_collection.size()) {
        int_t iColumn = 0;
        m_current = SqlValue<C>::Load(pDbContext, m_pSqlStatement, iColumn);
        m_iCount++;
    }
}

#endif /* !COLLECTION_HPP_ */
