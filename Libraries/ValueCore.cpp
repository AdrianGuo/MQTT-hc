/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueCore.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 22 Jan 2017 07:40:04
 *
 ******************************************************************************/

#include <algorithm>
#include "Libraries/LogPlus.hpp"
#include "Libraries/ValueCore.hpp"

i32_t ValueCore::m_siCountNbrObject = 0;
i32_t ValueCore::m_siCreateIndexObj = 0;
Vector<i32_t> ValueCore::m_vIndexObj;
pthread_mutex_t ValueCore::m_locker;

/**
 * @func   CreateIndex
 * @brief  Create an Index for object
 * @param  None
 * @retval None
 */
i32_t
ValueCore::CreateIndex() {
    bool_t boExistsIndex = FALSE;
    m_siCreateIndexObj++;
    for (u32_t i = 0; i < m_vIndexObj.size(); i++) {
        if (m_vIndexObj[i] == m_siCreateIndexObj) {
            boExistsIndex = TRUE;
            break;
        }
    }

    if (boExistsIndex) {
        return CreateIndex();
    } else {
        m_vIndexObj.push_back(m_siCreateIndexObj);
        return m_siCreateIndexObj;
    }
}

/**
 * @func   ClearIndex
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueCore::ClearIndex(
    i32_t iIndex
) {
    m_vIndexObj.erase(std::remove(m_vIndexObj.begin(),
    m_vIndexObj.end(), iIndex), m_vIndexObj.end());
}

/**
 * @func   ValueRef
 * @brief  Contructor
 * @param  None
 * @retval None
 */
ValueCore::ValueCore(
    const_char_p pNameValue
) : m_strObjName    (pNameValue),
    m_iParIndex     (0) {
    pthread_mutex_lock(&m_locker);
    m_iIndex = CreateIndex();
    m_siCountNbrObject++;

    #ifdef DB_VALUE_CORE
    MACRO_DBUG("[%18s] object [%05d] created from [%05d] --> count [%05d]",
    m_strObjName.c_str(), m_iIndex, m_iParIndex, m_siCountNbrObject);
    #endif /* DB_VALUE_CORE */

    pthread_mutex_unlock(&m_locker);
}

/**
 * @func   ValueRef
 * @brief  Copy contructor
 * @param  None
 * @retval None
 */
ValueCore::ValueCore(
    const ValueCore& copied
) : m_strObjName    (copied.m_strObjName),
    m_iParIndex     (copied.m_iIndex    ) {
    pthread_mutex_lock(&m_locker);
    m_iIndex = CreateIndex();
    m_siCountNbrObject++;

    #ifdef DB_VALUE_CORE
    MACRO_DBUG("[%18s] object [%05d] copied  from [%05d] --> count [%05d]",
    m_strObjName.c_str(), m_iIndex, m_iParIndex, m_siCountNbrObject);
    #endif /* DB_VALUE_CORE */

    pthread_mutex_unlock(&m_locker);
}

/**
 * @func   ~ValueRef
 * @brief  Destructor
 * @param  None
 * @retval None
 */
ValueCore::~ValueCore() {
    pthread_mutex_lock(&m_locker);
    ClearIndex(m_iIndex);
    m_siCountNbrObject--;

    #ifdef DB_VALUE_CORE
    MACRO_DBUG("[%18s] object [%05d] deleted            --> count [%05d]",
    m_strObjName.c_str(), m_iIndex, m_siCountNbrObject);
    #endif /* DB_VALUE_CORE */

    m_strObjName.clear();

    pthread_mutex_unlock(&m_locker);
}

/**
 * @func   operator=
 * @brief  Asignment
 * @param  None
 * @retval None
 */
ValueCore&
ValueCore::operator= (
    const ValueCore& copied
) {
    ValueCore temp(copied);
    Swap(temp);
    pthread_mutex_lock(&m_locker);
    m_siCountNbrObject++;

    #ifdef DB_VALUE_CORE
    MACRO_DBUG("[%18s] object [%05d] assigned from [%5d] --> count [%05d]",
    m_strObjName.c_str(), m_iIndex, m_iParIndex, m_siCountNbrObject);
    #endif /* DB_VALUE_CORE */

    pthread_mutex_unlock(&m_locker);

    return *this;
}

/**
 * @func   Swap
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueCore::Swap(
    ValueCore& other
) {
    std::swap(m_iParIndex,  other.m_iIndex      );
    std::swap(m_strObjName, other.m_strObjName  );
}

/**
 * @func   Index
 * @brief  None
 * @param  None
 * @retval None
 */
i32_t
ValueCore::Index(
) const {
    return m_iIndex;
}

/**
 * @func   ParIndex
 * @brief  None
 * @param  None
 * @retval None
 */
i32_t
ValueCore::ParIndex(
) const {
    return m_iParIndex;
}

/**
 * @func   Count
 * @brief  None
 * @param  None
 * @retval None
 */
i32_t
ValueCore::Count() {
    pthread_mutex_lock(&m_locker);
    i32_t iCount = m_siCountNbrObject;
    pthread_mutex_unlock(&m_locker);
    return iCount;
}
