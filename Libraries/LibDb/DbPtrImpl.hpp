/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbPtrImpl.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trunstkn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-11-03 16:15:00 (Wen, 03 Oct 2016)
 *
 ******************************************************************************/

#ifndef DBPTRIMPL_HPP_
#define DBPTRIMPL_HPP_

#include "../Typedefs.h"
#include "../Exception.hpp"
#include "DbPtr.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
/**
 * @func   DbPtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
inline
DbPtrBase::DbPtrBase(
    DbContext* pDbContext
) : m_pDbContext  (pDbContext),
    m_boTriggered (TRUE),
    m_dwState     (new_obj | need_saved_obj) {
}

/**
 * @func   ~DbPtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
inline
DbPtrBase::~DbPtrBase() {

}

/**
 * @func   Triggered
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
DbPtrBase::Triggered(
    bool_t boTriggered
) {
    m_boTriggered = boTriggered;
}

/**
 * @func   IsTriggered
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
DbPtrBase::IsTriggered(
) const {
    return m_boTriggered;
}

/**
 * @func   SetDbContext
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
DbPtrBase::SetDbContext(
    DbContext* pDbContext
) {
    m_pDbContext = pDbContext;
}

/**
 * @func   GetDbContext
 * @brief  None
 * @param  None
 * @retval None
 */
inline DbContext_p
DbPtrBase::GetDbContext(
) const {
    return m_pDbContext;
}

/**
 * @func   Remove
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
 * @func   IsDeleted
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
DbPtrBase::IsDeleted(
) const {
    return (m_dwState & (need_deleted_obj | deleted_in_transaction)) != 0;
}

/**
 * @func   IsRefreshed
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
DbPtrBase::IsRefreshed(
) const {
    return (m_dwState & (refreshed | saved_in_transaction)) != 0;
}

/**
 * @func   SetTransaction
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

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

/**
 * @func   DbPtrCore
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtrCore<C>::DbPtrCore(
    C*         pObj,
    DbContext* pDbContext
) : DbPtrBase (pDbContext),
    m_pObj    (pObj) {
}

/**
 * @func   ~DbPtrCore
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtrCore<C>::~DbPtrCore() {
    delete m_pObj;
}

/**
 * @func   SetObject
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::SetObj(
    C* pObject
) {
    m_pObj = pObject;
}

/**
 * @func   Object
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
DbPtrCore<C>::Obj(
) const {
    return m_pObj;
}

/**
 * @func   SetId
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
 * @func   GetId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline typename
ConfigTable<C>::IdType
DbPtrCore<C>::GetId(
) const {
    return m_currId;
}

/**
 * @func   UpdateChange
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::UpdateChange() {
    DbContext_p pDbContext = GetDbContext();

    if (pDbContext != NULL) {
        if ((m_dwState & need_saved_obj) != 0) {
            m_dwState |= saving_obj;
            pDbContext->Save<C>(*this);
            m_dwState &= ~saving_obj;
            m_dwState |= saved_in_transaction;
        } else if ((m_dwState & need_deleted_obj) != 0) {
            m_dwState &= ~need_deleted_obj;
            pDbContext->Delete<C>(*this);
            m_dwState &= ~saving_obj;
            m_dwState |= deleted_in_transaction;
        } else if ((m_dwState & saving_obj) != 0) {

        }
    } else {
        LOG_ERROR("dbcontext null");
    }
}

/**
 * @func   BindId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtrCore<C>::BindId(
    SmartPtr<SqlStatement> pSqlStatement,
    int_t iColumn
) {
    pSqlStatement->bind(iColumn, m_currId);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

/**
 * @func   DbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(
) : m_pCoreObj (NULL){
}

/**
 * @func   DbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(
    C* pObj,
    DbContext* pDbContext
) : m_pCoreObj (NULL) {
    if (pObj != NULL) {
        m_pCoreObj = new DbPtrCore<C> (const_cast<C*>(pObj), pDbContext);
    }
}

/**
 * @func   DbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(
   DbPtrCore<C_t>* pCoreObj
) : m_pCoreObj (pCoreObj) {
}

/**
 * @func   DbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::DbPtr(
    const DbPtr<C>& copied
) : m_pCoreObj (copied.m_pCoreObj) {
}

/**
 * @func   ~DbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbPtr<C>::~DbPtr() {
    Release();
}

/**
 * @func   Release
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtr<C>::Release() {
    if ((m_pCoreObj != NULL) && !m_pCoreObj->IsTriggered()) {
        delete m_pCoreObj;
    }
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>&
DbPtr<C>::operator= (
    DbPtrCore<C_t>* copied
) {
    if (m_pCoreObj != copied) {
        Release();
        m_pCoreObj = copied;
    }
    return *this;
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>&
DbPtr<C>::operator= (
    const DbPtr<C>& copied
) {
    if (m_pCoreObj != copied.m_pCoreObj) {
        Release();
        m_pCoreObj = copied.m_pCoreObj;
    }
    return *this;
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<class D>
inline DbPtr<C>&
DbPtr<C>::operator= (
    const DbPtr<D>& copied
) {
    D* d = NULL;
    C* c = d;
    (void) c;

    if (m_pCoreObj != copied.m_pCoreObj) {
        Release();
        m_pCoreObj = copied.m_pCoreObj;
    }
    return *this;
}

/**
 * @func   Object
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtrCore<C>*
DbPtr<C>::Obj(
) const {
    return m_pCoreObj;
}

/**
 * @func   Modify
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
DbPtr<C>::Modify(
) const {
    if (m_pCoreObj != NULL) {
        return m_pCoreObj->Obj();
    } else {
        throw Exception("null dereference");
    }
}

/**
 * @func   operator->
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const C*
DbPtr<C>::operator->(
) const {
    const C* ptrC = get();
    if (ptrC == NULL) {
        throw ExceptionNullPtr("null dereference");
    }
    return ptrC;
}

/**
 * @func   operator*
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const C&
DbPtr<C>::operator*(
) const {
    if (m_pCoreObj != NULL) {
        return *m_pCoreObj->Obj();
    } else {
        throw ExceptionNullPtr("null dereference");
    }
}

/**
 * @func   get
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const C*
DbPtr<C>::get(
) const {
    if (m_pCoreObj != NULL) {
        return m_pCoreObj->Obj();
    } else {
        return NULL;
    }
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
DbPtr<C>::Id(
) const {
    if (m_pCoreObj != NULL) {
        return m_pCoreObj->GetId();
    } else {
        throw ExceptionNullPtr("null dereference");
    }
}

/**
 * @func   UpdateChange
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtr<C>::UpdateChange() {
    if (m_pCoreObj != NULL) {
        m_pCoreObj->UpdateChange();
    }
}

/**
 * @func   Remove
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbPtr<C>::Remove() {
    if (m_pCoreObj != NULL) {
        m_pCoreObj->Remove();
    }
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
DbPtr<C>::operator== (
    const DbPtr<C>& rhs
) const {
    return m_pCoreObj == rhs.m_pCoreObj;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
DbPtr<C>::operator!= (
    const DbPtr<C>& rhs
) const {
    return m_pCoreObj != rhs.m_pCoreObj;
}

/**
 * @func   operator<
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
DbPtr<C>::operator< (
     const DbPtr<C>& rhs
) const {
    return m_pCoreObj < rhs.m_pCoreObj;
}

#endif /* DBPTRIMPL_HPP_ */
