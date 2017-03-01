/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: PtrBase.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 18 Feb 2017 16:25:43
 *
 ******************************************************************************/

#ifndef PTR_BASE_HPP_
#define PTR_BASE_HPP_

#include "Libraries/typedefs.h"
#include "Libraries/ValueCore.hpp"

class PtrBase : public ValueCore {
protected:
    int_t m_iRef;
public:
    PtrBase();
    virtual ~PtrBase();

    void_t IncRef();
    void_t DecRef();
};

/**
 * @func   PtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
inline
PtrBase::PtrBase(
) : m_iRef (0) {

}

/**
 * @func   ~PtrBase
 * @brief  None
 * @param  None
 * @retval None
 */
inline
PtrBase::~PtrBase(
) {
}

/**
 * @func   IncRef
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
PtrBase::IncRef() {
    m_iRef++;
}

/**
 * @func   DecRef
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
PtrBase::DecRef() {
    m_iRef--;
    if (m_iRef == 0) {
        delete this;
    }
}

#endif /* !PTR_BASE_HPP_ */
