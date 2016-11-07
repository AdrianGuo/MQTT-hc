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
 * Last Changed:     Date: 2016-11-03 16:15:00 (Wen, 03 Oct 2016)
 *
 ******************************************************************************/

#ifndef DBPTRIMPL_HPP_
#define DBPTRIMPL_HPP_

#include "DbPtr.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
DbPtrBase::DbPtrBase(
    DbContext* pDbContext
) {
    m_pDbContext = pDbContext;
    m_dwState = new_obj | need_saved_obj;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
DbPtrBase::Remove() {
    if (IsDeleted()) {
        // All ready delete
    } else if (IsRefreshed()) {
        m_dwState &= ~need_saved_obj;
        m_dwState |= need_deleted_obj;
        GetDbContext()->FlushObject(this);
    } else if (GetDbContext() != NULL) {
        GetDbContext()->DisCardChanges(this);
        SetDbContext(NULL);
        m_dwState &= ~need_saved_obj;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
DbPtrBase::SetTransaction(
    StateFlag_t stateFlag
) {
    m_dwState &= ~saving_obj;
    m_dwState |= stateFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtrCore<C>::DbPtrCore(
    C* pObject,
    DbContext* pDbContext
) : DbPtrBase (pDbContext),
    m_pObject (pObject),
    m_currId () {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::SetId(
    typename ConfigTable<C>::IdType currId
) {
    m_currId = currId;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename
ConfigTable<C>::IdType
DbPtrCore<C>::GetId() const {
    return m_currId;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::UpdateChange() {
    if ((m_dwState & need_saved_obj) != 0) {
//        m_dwState &= ~need_saved_obj;
        m_dwState |= saving_obj;
        GetDbContext()->Save(*this);
        m_dwState &= ~saving_obj;
        m_dwState |= saved_in_transaction;
    } else if ((m_dwState & need_deleted_obj) != 0) {
        m_dwState &= ~need_deleted_obj;
        GetDbContext()->Delete(*this);
        m_dwState &= ~saving_obj;
        m_dwState |= deleted_in_transaction;
    } else if ((m_dwState & saving_obj) != 0) {

    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::BindId(
    SqlStatement_p pSqlStatement,
    int_t iColumn
) {
    pSqlStatement->bind(iColumn, m_currId);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(
    C* pObject
) {
    m_pPtrCore = new DbPtrCore<C> (pObject);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(DbPtrCore<C>* pObject) {
    m_pPtrCore = pObject;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>&
DbPtr<C>::operator= (
    const DbPtr<C>& other
) {
    if (m_pPtrCore != other.Object()) {
        m_pPtrCore = other.Object();
    }

    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const C*
DbPtr<C>::operator->() const {
    const C* ptrC = m_pPtrCore->Object();
    return ptrC;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const C&
DbPtr<C>::operator*() const {
    return *m_pPtrCore->Object();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
DbPtr<C>::operator== (
    const DbPtr<C>& other
) const {
    return m_pPtrCore == other.Object();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
DbPtr<C>::operator!= (
    const DbPtr<C>& other
) const {
    return m_pPtrCore != other.Object();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename
ConfigTable<C>::IdType
DbPtr<C>::Id() const {
    return m_pPtrCore->GetId();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtr<C>::UpdateChange() {
    m_pPtrCore->UpdateChange();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtr<C>::Remove() {
    m_pPtrCore->Remove();
}


#endif /* LIBRARIES_LIBDB_DBPTRIMPL_HPP_ */
