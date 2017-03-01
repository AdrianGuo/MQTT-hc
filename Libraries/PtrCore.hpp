/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: PtrCore.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 18 Feb 2017 16:48:38
 *
 ******************************************************************************/
#ifndef PTR_CORE_HPP_
#define PTR_CORE_HPP_

#include "Libraries/PtrBase.hpp"

template<class C>
class PtrCore : public PtrBase {
private:
    typedef typename std::remove_const<C>::type C_t;
    C* m_pObj;
public:
    PtrCore(C* pObj);
    virtual ~PtrCore();

    C* Obj();
    void_t SetObj(C *pObj);
};

/**
 * @func   PtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
PtrCore<C>::PtrCore(
    C* pObj
) : m_pObj (pObj) {

}

/**
 * @func   ~PtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
PtrCore<C>::~PtrCore(
) {
    if (m_iRef != 0) {
        throw Exception("ptr has ref > 0");
    }
    delete m_pObj;
}

/**
 * @func   Obj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline C*
PtrCore<C>::Obj() {
    return m_pObj;
}

/**
 * @func   SetObj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
PtrCore<C>::SetObj(
    C* pObj
) {
    m_pObj = pObj;
}

#endif /* PTR_CORE_HPP_ */
