/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbPtr.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-11 11:45:00 (Tue, 11 Oct 2016)
 *
 ******************************************************************************/
#ifndef DBPTR_HPP_
#define DBPTR_HPP_

#include "../Typedefs.h"
#include "../SmartPtr.hpp"
#include "../PtrBase.hpp"
#include "Config.hpp"
#include "ConfigImpl.hpp"
#include "DbContext.hpp"

class DbContext;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbPtrBase {
private:
    DbContext*  m_pDbContext;
protected:
    bool_t      m_boTriggered;
    u32_t       m_dwState;
public:
    typedef enum _State_ {
        // State
        new_obj = 0x000,
        refreshed = 0x001,
        orphaned = 0x002,
        // flags
        need_saved_obj = 0x010,
        saving_obj = 0x020,
        need_deleted_obj = 0x040,
        //
        saved_in_transaction = 0x100,
        deleted_in_transaction = 0x200
    } StateFlag_t;

    DbPtrBase(DbContext* pDbContext = NULL);
    virtual ~DbPtrBase();

    void_t Triggered(bool_t boTriggered = TRUE);
    bool_t IsTriggered() const;

    DbContext* GetDbContext() const;
    void_t SetDbContext(DbContext* pDbContext);

    virtual void_t Remove();
    virtual void_t UpdateChange() = 0;
    virtual void_t BindId(SmartPtr<SqlStatement> pSqlStatement, int_t iColumn) = 0;

    bool_t IsDeleted()   const;
    bool_t IsRefreshed() const;

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
    C* m_pObj;
    typename ConfigTable<C>::IdType m_currId; // Primary key
public:
    DbPtrCore(C* pObj, DbContext* pDbContext = NULL);
    virtual ~DbPtrCore();

    C* Obj() const;
    void_t SetObj(C* pObj);

    void_t SetId(typename ConfigTable<C>::IdType currId);
    typename ConfigTable<C>::IdType GetId() const;

    virtual void_t UpdateChange();
    virtual void_t BindId(SmartPtr<SqlStatement> pSqlStatement, int_t iColumn);
};

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbPtr {
private:
    typedef typename std::remove_const<C>::type C_t;
    DbPtrCore<C_t>* m_pCoreObj;

    void_t Release();
public:
    DbPtr();
    DbPtr(C* pObj, DbContext* pDbContext = NULL);
    DbPtr(DbPtrCore<C_t>* pCoreObj);

    DbPtr(const DbPtr<C>& copied);
    template<class D>
    DbPtr(const DbPtr<D>& copied);

    virtual ~DbPtr();

    C* Modify() const;
    DbPtrCore<C>* Obj() const;

    const C* operator->() const;
    const C& operator*()  const;
    const C* get()        const;

    DbPtr<C>& operator= (DbPtrCore<C_t>* copied);
    DbPtr<C>& operator= (const DbPtr<C>& copied);
    template<class D>
    DbPtr<C>& operator= (const DbPtr<D>& copied);

    typename ConfigTable<C>::IdType Id() const;

    virtual void_t UpdateChange();
    virtual void_t Remove();

    bool_t operator== (const DbPtr<C>& rhs) const;
    bool_t operator!= (const DbPtr<C>& rhs) const;
    bool_t operator<  (const DbPtr<C>& rhs) const;
};

#endif /* !DBPTR_HPP_ */
