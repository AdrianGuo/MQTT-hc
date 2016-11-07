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

#ifndef JSONMESSAGE_SESSION_HPP_
#define JSONMESSAGE_SESSION_HPP_

#include <iostream>
#include <typeinfo>
#include <type_traits>
#include "typedefs.h"
#include "Map.hpp"
#include "JsonMessagePtr.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class JsonMessageSession {
private:
    typedef const std::type_info* const_type_info_ptr;
    typedef Map<const_type_info_ptr, JsonMessageBase_p> JsonTypeRegistry;
    JsonTypeRegistry m_JsonTypeRegistry;

    typedef Map<String, JsonMessageBase_p> JsonNameRegistry;
    JsonNameRegistry m_JsonNameRegistry;
public:
    template<class C> void_t MapJsonMessage(const String strJsonMessageName);
    JsonMessageSession() {}
    virtual ~JsonMessageSession();
    template<class C> JsonMessageMap<C>* GetJsonMapping() const;
    JsonMessageBase_p GetJsonMapping(const String strJsonMessageName) const;
};

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
inline
JsonMessageSession::~JsonMessageSession() {
    for (JsonTypeRegistry::iterator_t it = m_JsonTypeRegistry.begin();
            it != m_JsonTypeRegistry.end(); it++) {
        delete it->second;
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
JsonMessageSession::MapJsonMessage(
    const String strJsonMessageName
) {
    JsonMessageMap<C>* pJsonMessage = new JsonMessageMap<C>();
    m_JsonTypeRegistry[&typeid(C)] = pJsonMessage;
    m_JsonNameRegistry[strJsonMessageName] = pJsonMessage;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline JsonMessageMap<C>*
JsonMessageSession::GetJsonMapping() const {
    typedef typename std::remove_const<C>::type C_t;
    JsonTypeRegistry::const_iterator_t it = m_JsonTypeRegistry.find(&typeid(C_t));
    if (it != m_JsonTypeRegistry.end()) {
        return dynamic_cast<JsonMessageMap<C>*>(it->second);
    }

    return NULL;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonMessageBase_p
JsonMessageSession::GetJsonMapping(
    String strJsonMessageName
) const {
    JsonNameRegistry::const_iterator_t it = m_JsonNameRegistry.find(strJsonMessageName);
    if (it != m_JsonNameRegistry.end()) {
        return it->second;
    }

    return NULL;
}

#endif /* JSONMESSAGE_SESSION_HPP_ */
