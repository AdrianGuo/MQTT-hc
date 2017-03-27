/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SmartPtr.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 3 Feb 2017 18:04:45
 *
 ******************************************************************************/
#ifndef SMART_PTR_HPP_
#define SMART_PTR_HPP_

#include "Libraries/Typedefs.h"
#include "Libraries/Exception.hpp"
#include "Libraries/PtrCore.hpp"

template<class C>
class SmartPtr {
private:
    PtrCore<C>* m_pPtr;

    void_t TakeObj();
    void_t FreeObj();
protected:
    PtrCore<C>* Obj() const;
public:
    SmartPtr();
    SmartPtr(C* pObj);
    SmartPtr(const SmartPtr<C>& copied);
    template<class D>
    SmartPtr(const SmartPtr<D>& copied);

    virtual ~SmartPtr();

    C* operator-> () const;
    C& operator*  () const;
    const C* get() const;

    SmartPtr<C>& operator= (const SmartPtr<C>& copied);
    template<class D>
    SmartPtr<C>& operator= (const SmartPtr<D>& copied);

    bool_t operator== (const SmartPtr<C>& rhs) const;
    bool_t operator!= (const SmartPtr<C>& rhs) const;
    bool_t operator<  (const SmartPtr<C>& rhs) const;
};

/**
 * @func   SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
SmartPtr<C>::SmartPtr(
) : m_pPtr (NULL) {

}

/**
 * @func   SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
SmartPtr<C>::SmartPtr(
    C* pObj
) : m_pPtr (NULL) {
    if (pObj != NULL) {
        m_pPtr = new PtrCore<C>(const_cast<C*>(pObj));
        TakeObj();
    }
}

/**
 * @func   SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
SmartPtr<C>::SmartPtr(
    const SmartPtr<C>& copied
) : m_pPtr (copied.m_pPtr) {
    TakeObj();
}

/**
 * @func   SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<class D> inline
SmartPtr<C>::SmartPtr(
    const SmartPtr<D>& copied
) : m_pPtr (copied.m_pPtr) {
    D* d = NULL;
    C* c = d;
    (void) c;
    TakeObj();
}

/**
 * @func   ~SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
SmartPtr<C>::~SmartPtr() {
    FreeObj();
}

/**
 * @func   Obj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline PtrCore<C>*
SmartPtr<C>::Obj(
) const {
    return m_pPtr;
}

/**
 * @func   TakeObj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
SmartPtr<C>::TakeObj() {
    if (m_pPtr != NULL) {
        m_pPtr->IncRef();
    }
}

/**
 * @func   ~SmartPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
SmartPtr<C>::FreeObj() {
    if (m_pPtr != NULL) {
        m_pPtr->DecRef();
        m_pPtr = NULL;
    }
}

/**
 * @func   operator->
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
SmartPtr<C>::operator-> (
) const {
    if (m_pPtr != NULL) {
        return m_pPtr->Obj();
    } else {
        throw Exception("null dereference");
    }
}

/**
 * @func   operator*
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C&
SmartPtr<C>::operator* (
) const {
    if (m_pPtr != NULL) {
        return *m_pPtr->Obj();
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
SmartPtr<C>::get(
) const {
    if (m_pPtr != NULL) {
        return m_pPtr->Obj();
    } else {
        return NULL;
    }
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline SmartPtr<C>&
SmartPtr<C>::operator= (
    const SmartPtr<C>& copied
) {
    if (m_pPtr != copied.m_pPtr) {
        FreeObj();
        m_pPtr = copied.m_pPtr;
        TakeObj();
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
SmartPtr<C>&
SmartPtr<C>::operator= (
    const SmartPtr<D>& copied
) {
    D* d = NULL;
    C* c = d;
    (void_t) (c);

    if (m_pPtr != copied.m_pPtr) {
        FreeObj();
        m_pPtr = copied.m_pPtr;
        TakeObj();
    }
    return *this;
}

/**
 * @func   operator==
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
SmartPtr<C>::operator== (
    const SmartPtr<C>& rhs
) const {
    return m_pPtr == rhs.m_pPtr;
}

/**
 * @func   operator!=
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
SmartPtr<C>::operator!= (
    const SmartPtr<C>& rhs
) const {
    return m_pPtr != rhs.m_pPtr;
}

/**
 * @func   operator<
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
SmartPtr<C>::operator< (
    const SmartPtr<C>& rhs
) const {
    return m_pPtr < rhs.m_pPtr;
}

#endif /* !SMART_PTR_HPP_ */
