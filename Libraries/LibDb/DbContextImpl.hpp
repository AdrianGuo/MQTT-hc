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

#ifndef DBCONTEXTIMPL_HPP_
#define DBCONTEXTIMPL_HPP_

#include "DbPtr.hpp"
#include "Query.hpp"
#include "DbAction.hpp"
#include "ConfigImpl.hpp"
#include "DbPtrImpl.hpp"
#include "DbContext.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::MapTable<C>::InitMapTable(
    DbContext& dbContext
) {
    typedef typename std::remove_const<C>::type C_t;
    if (!Initialized) {
        Initialized = TRUE;
        C_t tmpC;
        DbInit dbAction(dbContext, *this);
        dbAction.Config(tmpC);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::MapTable<C>::DropTable(
    DbContext& dbContext
) {
    typedef typename std::remove_const<C>::type C_t;
    C_t tmpC;
    DbDrop dbAction(dbContext, *this);
    dbAction.Config(tmpC);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline String
DbContext::MapTable<C>::PrimaryKeys() const {
    if (InsteadIdName != String()) { return InsteadIdName; }

    bool_t boFirstColumn = TRUE;
    String strPrimaryKeys;

    for (u32_t i = 0; i < Columns.size(); i++) {
        if (Columns[i].IsIdField()) {
            if (!boFirstColumn) {
                strPrimaryKeys += ", ";
            }
            strPrimaryKeys += "\"" + Columns[i].GetColumnName() + "\"";
            boFirstColumn = FALSE;
        }
    }

    return strPrimaryKeys;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Add(
    C* pObject
) {
    DbPtr<C> dbPtr(pObject);
    return Add(dbPtr);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Add(
    DbPtr<C>& dbPtr
) {
    DbPtrCore<C>* pDbPtrCore = dbPtr.Object();

    pDbPtrCore->SetDbContext(this);

    if (GetFlushMode() == MODE_AUTO) {
        FlushObject(pDbPtrCore);
    } else {
        m_vecObjecToAdd.push_back(pDbPtrCore);
    }

    return dbPtr;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Save(
    DbPtrCore<C>& dbPtr
) {
    MapTable<C>* pMapping = GetMapping<C>();
    DbSaveObject<C> dbAction(dbPtr, *pMapping);

    dbAction.Config(*dbPtr.Object());
    pMapping->Registry[dbPtr.GetId()] = &dbPtr;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Delete(
    DbPtrCore<C>& dbPtr
) {
    MapTable<C>* pMapping = GetMapping<C>();
    SqlStatement_p pStatement = GetStatement<C>(DbContext::DELETE);
    pStatement->reset();

    pStatement->bind(0, dbPtr.GetId());
    pStatement->execute();

    pMapping->Registry.erase(dbPtr.GetId());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Load(
    DbPtrCore<C>& dbPtrCore,
    SqlStatement_p pSqlStatement,
    int_t iColumn
) {
    MapTable<C>* pMapping = GetMapping<C>();
    DbLoadObject<C> dbAction(dbPtrCore, *pMapping, pSqlStatement);

    C* obj = new C();
    dbAction.Config(*obj);
    dbPtrCore.SetObject(obj);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Load(
    SqlStatement_p pSqlStatement,
    int_t iColumn
) {
    DbPtrCore<C>* pdbPtrCore = LoadId<C>(pSqlStatement, iColumn);

    if (pdbPtrCore != NULL) {
        return DbPtr<C>(pdbPtrCore);
    } else {
        return DbPtr<C>();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Load(
    const typename ConfigTable<C>::IdType& id
) {
    MapTable<C>* pMapping = GetMapping<C>();
    typename MapTable<C>::Registry_t::iterator_t it = pMapping->Registry.find(id);

    if (it == pMapping->Registry.end()) {
        DbPtrCore<C>* dbPtrCore = new DbPtrCore<C>();
        dbPtrCore->SetId(id);
        pMapping->Registry[id] = dbPtrCore;
        return DbPtr<C>(dbPtrCore);
    } else {
        return DbPtr<C>(it->second);
    }
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
void_t
DbContext::Refresh(
    const typename ConfigTable<C>::IdType& id
) {
    MapTable<C>* pMapping = GetMapping<C>();
    DbPtrCore<C>* pdbPtrCore;
    typename MapTable<C>::Registry_t::iterator_t it = pMapping->Registry.find(id);

    if (it == pMapping->Registry.end()) {
        pdbPtrCore = new DbPtrCore<C>(NULL, this);
        pMapping->Registry[id] = pdbPtrCore;
    } else {
        pdbPtrCore = it->second;
    }

    SqlStatement_p pSqlStatement = GetStatement<C>(SELECTBYID);
    pSqlStatement->bind(0, id);
    DbLoadObject<C> dbAction(*pdbPtrCore, *pMapping, pSqlStatement);

    dbAction.Config(*pdbPtrCore->Object());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtrCore<C>*
DbContext::LoadId(
    SqlStatement_p pSqlStatement,
    int_t iColumn
) {
    MapTable<C>* pMapping = GetMapping<C>();
    DbPtrCore<C>* pdbPtrCore = new DbPtrCore<C>(NULL, this);
    Load(*pdbPtrCore, pSqlStatement, iColumn); // Chu y

    typename MapTable<C>::Registry_t::iterator_t it =
                                    pMapping->Registry.find(pdbPtrCore->GetId());

    if (it == pMapping->Registry.end()) {
        pMapping->Registry[pdbPtrCore->GetId()] = pdbPtrCore;
        return pdbPtrCore;
    } else {
        delete pdbPtrCore;
        return it->second;
    }
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline Query<DbPtr<C>>
DbContext::Find(
    const String& strWhere
) {
    return Query<DbPtr<C>>(this, TableName<C>(), strWhere);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class R>
inline Query<R>
DbContext::Command(
    const String& strSql
) {
    return Query<R> (this, strSql);
}

#endif /* !DBCONTEXTIMPL_HPP_ */
