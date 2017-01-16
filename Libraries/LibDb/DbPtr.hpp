/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbPtr.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-11 11:45:00 (Tue, 11 Oct 2016)
 *
 ******************************************************************************/
#ifndef DBPTR_HPP_
#define DBPTR_HPP_

#include <stddef.h>
#include "typedefs.h"
#include "Config.hpp"
#include "ConfigImpl.hpp"
#include "DbContext.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbPtrBase {
private:
    DbContext* m_pDbContext;

protected:
    u32_t m_dwState;

public:
    typedef enum _State_ {
        // State
        new_obj = 0x00,
        refreshed = 0x01,
        //
        need_saved_obj = 0x02,
        saving_obj = 0x04,
        need_deleted_obj = 0x08,
        //
        saved_in_transaction = 0x10,
        deleted_in_transaction = 0x20
    } StateFlag_t;

    DbPtrBase(DbContext* pDbContext = NULL);
    virtual ~DbPtrBase() {}

    void_t SetDbContext(DbContext* pDbContext) { m_pDbContext = pDbContext; }
    DbContext* GetDbContext() const { return m_pDbContext; }

    virtual void_t Remove();
    virtual void_t UpdateChange() = 0;
    virtual void_t BindId(SqlStatement_p pSqlStatement, int_t iColumn) = 0;

    bool_t IsDeleted() const { return (m_dwState & (need_deleted_obj | deleted_in_transaction)) != 0; }
    bool_t IsRefreshed() const { return (m_dwState & (refreshed | saved_in_transaction)) != 0; }
    void_t SetTransaction(StateFlag_t stateFlag);
};

typedef DbPtrBase  DbPtrBase_t;
typedef DbPtrBase* DbPtrBase_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

template<class C>
class DbPtrCore : public DbPtrBase {
private:
    C* m_pObject;
    typename ConfigTable<C>::IdType m_currId;
public:
    DbPtrCore(C* pObject = NULL, DbContext* pDbContext = NULL);
    virtual ~DbPtrCore() {
        if (m_pObject != NULL) {
            delete m_pObject;
        }
    }

    void_t SetObject(C* pObject) { m_pObject = pObject; }
    C* Object() const { return m_pObject; }

    void_t SetId(typename ConfigTable<C>::IdType currId);
    typename ConfigTable<C>::IdType GetId() const;

    virtual void_t UpdateChange();
    virtual void_t BindId(SqlStatement_p pSqlStatement, int_t iColumn);
};

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbPtr {
private:
    DbPtrCore<C>* m_pPtrCore;
public:
    DbPtr(C* pObject = NULL);
    DbPtr(DbPtrCore<C>* pObject);

    virtual ~DbPtr() {}

    DbPtrCore<C>* Object() const { return m_pPtrCore; }
    C* Modify() const { return m_pPtrCore->Object(); }

    const C* operator->() const;
    const C& operator*()  const;

    const C* get() const;

    DbPtr<C>& operator= (const DbPtr<C>& other);

    bool_t operator== (const DbPtr<C>& other) const;
    bool_t operator!= (const DbPtr<C>& other) const;

    typename ConfigTable<C>::IdType Id() const;

    virtual void_t UpdateChange();
    virtual void_t Remove();
};

#endif /* !DBPTR_HPP_ */
