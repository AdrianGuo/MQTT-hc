/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueCore.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 22 Jan 2017 07:36:46
 *
 ******************************************************************************/
#ifndef VALUE_CORE_HPP_
#define VALUE_CORE_HPP_

#include <pthread.h>
#include "Libraries/Typedefs.h"

class ValueCore {
private:
    static i32_t m_siCountNbrObject;
    static i32_t m_siCreateIndexObj;
    static pthread_mutex_t m_locker;
    static Vector<i32_t> m_vIndexObj;
    String m_strObjName;
    i32_t m_iParIndex;
    i32_t m_iIndex;
protected:
    void_t Swap(ValueCore& other);
public:
    ValueCore(const_char_p pNameValue = "ValueCore");
    ValueCore(const ValueCore& copied);
    virtual ~ValueCore();

    ValueCore& operator= (const ValueCore& copied);

    static i32_t  CreateIndex();
    static void_t ClearIndex(i32_t iIndex);

    i32_t Index() const;
    i32_t ParIndex() const;

    static i32_t Count();
};

typedef ValueCore  ValueCore_t;
typedef ValueCore* ValueCore_p;

#endif /* VALUE_CORE_HPP_ */
