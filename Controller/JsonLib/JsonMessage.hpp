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

#ifndef JSON_MESSAGE_BASE_HPP_
#define JSON_MESSAGE_BASE_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonCommand.hpp"

template<class C> class JsonMessagePtr;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class JsonMessageBase {
public:
    JsonMessageBase() {}
    virtual ~JsonMessageBase() {}
    virtual void_t Refresh() { }
};

typedef JsonMessageBase  JsonMessageBase_t;
typedef JsonMessageBase* JsonMessageBase_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class JsonMessageMap : public JsonMessageBase {
private:
    C* m_pObject;
public:
    JsonMessageMap();
    virtual ~JsonMessageMap();
    C* Object() const { return m_pObject; }

    C& operator* () const { return *m_pObject; }
    C* operator-> () const { return m_pObject; }
    operator JsonMessagePtr<C> () const;
    void_t Refresh() { m_pObject->Refresh(); }
};

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
JsonMessageMap<C>::JsonMessageMap() {
    m_pObject = new C();
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
JsonMessageMap<C>::~JsonMessageMap() {
    if (m_pObject != NULL) {
        delete m_pObject;
        m_pObject = NULL;
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
JsonMessageMap<C>::operator JsonMessagePtr<C> () const {
    return JsonMessagePtr<C>(this);
}

#endif /* !JSON_MESSAGE_BASE_HPP_ */
