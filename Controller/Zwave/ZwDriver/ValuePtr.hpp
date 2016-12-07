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
 * Last Changed:     Date: 2016-11-01 15:35:00 (Tue, 01 Nov 2016)
 *
 ******************************************************************************/

#ifndef VALUE_SHARE_HPP_
#define VALUE_SHARE_HPP_

#include <stddef.h>
#include "typedefs.h"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
#define ROOT_ORDER                  0
#define ZWAVE_NETW                  0

template<class T>
class ValuePtr {
private:
    ValuePtr() {}
    static T* m_pInstance;
public:
    virtual ~ValuePtr() {
        if (m_pInstance != NULL) {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    static T* GetInstance() {
        return m_pInstance;
    }

    template<typename P1>
    static T* GetInstance(P1& p1) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1);
        }
        return m_pInstance;
    }

    template<typename P1, typename P2>
    static T* GetInstance(P1& p1, P2& p2) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2);
        }
        return m_pInstance;
    }

    template<typename P1, typename P2, typename P3>
    static T* GetInstance(P1& p1, P2& p2, P3& p3) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3);
        }
        return m_pInstance;
    }

    template<typename P1, typename P2, typename P3, typename P4>
    static T* GetInstance(P1& p1, P2& p2, P3& p3, P4& p4) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3, p4);
        }
        return m_pInstance;
    }

    template<typename P1, typename P2, typename P3, typename P4, typename P5>
    static T* GetInstance(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3, p4, p5);
        }
        return m_pInstance;
    }
};

template<class T>
T* ValuePtr<T>::m_pInstance = NULL;

template<class T>
class ValueRef {
private:
    ValueRef() {}
    static T* m_pInstance;
    ValueRef(const ValueRef& rhs) {}
    ValueRef& operator= (const ValueRef& rhs) { return *this; }
public:
    virtual ~ValueRef() {
        if (m_pInstance != NULL) {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    static T& GetInstance() {
        return *m_pInstance;
    }

    template<typename P1>
    static T& GetInstance(P1& p1) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1);
        }
        return *m_pInstance;
    }

    template<typename P1, typename P2>
    static T& GetInstance(P1& p1, P2& p2) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2);
        }
        return *m_pInstance;
    }

    template<typename P1, typename P2, typename P3>
    static T& GetInstance(P1& p1, P2& p2, P3& p3) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3);
        }
        return *m_pInstance;
    }

    template<typename P1, typename P2, typename P3, typename P4>
    static T& GetInstance(P1& p1, P2& p2, P3& p3, P4& p4) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3, p4);
        }
        return *m_pInstance;
    }

    template<typename P1, typename P2, typename P3, typename P4, typename P5>
    static T& GetInstance(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3, p4, p5);
        }
        return *m_pInstance;
    }
    template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    static T& GetInstance(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6) {
        if (m_pInstance == NULL) {
            m_pInstance = new T(p1, p2, p3, p4, p5, p6);
        }
        return *m_pInstance;
    }
};

template<class T>
T* ValueRef<T>::m_pInstance = NULL;

#endif /* !VALUE_SHARE_HPP_ */
