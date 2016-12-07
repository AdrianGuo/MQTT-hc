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
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/
#ifndef JSON_MESSAGE_MAP_HPP_
#define JSON_MESSAGE_MAP_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "JsonMessage.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class JsonMessagePtr {
private:
    JsonMessageMap<C>* m_pJsonMessageMap;
public:
    JsonMessagePtr(JsonMessageMap<C>* pJsonMessageMap);
    virtual ~JsonMessagePtr() { Refresh(); }

    JsonMessageMap<C>* Object() const;

    C* operator-> () const;
    C& operator*  () const;

    JsonMessagePtr<C>& operator= (const JsonMessagePtr<C>& other);

    bool_t operator== (const JsonMessagePtr<C>& other) const;
    bool_t operator!= (const JsonMessagePtr<C>& other) const;

    void_t Refresh() { m_pJsonMessageMap->Refresh(); }
};

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
JsonMessagePtr<C>::JsonMessagePtr(
    JsonMessageMap<C>* pJsonMessageMap
) {
    m_pJsonMessageMap = pJsonMessageMap;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
JsonMessageMap<C>*
JsonMessagePtr<C>::Object() const {
    return m_pJsonMessageMap;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
JsonMessagePtr<C>::operator-> () const {
    return m_pJsonMessageMap->Object();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C&
JsonMessagePtr<C>::operator* () const {
    return &m_pJsonMessageMap->Object();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline JsonMessagePtr<C>&
JsonMessagePtr<C>::operator= (
    const JsonMessagePtr<C>& other
) {
    if (m_pJsonMessageMap != other.Object()) {
        m_pJsonMessageMap = other.Object();
    }
    return *this;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
JsonMessagePtr<C>::operator== (
    const JsonMessagePtr<C>& other
) const {
    return m_pJsonMessageMap == other.Object();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline bool_t
JsonMessagePtr<C>::operator!= (
    const JsonMessagePtr<C>& other
) const {
    return m_pJsonMessageMap != other.Object();
}

#endif /* !JSON_MESSAGE_MAP_HPP_ */
