/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbContextImpl.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-05-16 11:45:00 (Tue, 16 May 2016)
 *
 ******************************************************************************/

#ifndef DB_CONTEXT_IMPL_HPP_
#define DB_CONTEXT_IMPL_HPP_

#include <iostream>
#include <type_traits>

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/DbPtr.hpp"
#include "Libraries/LibDb/Query.hpp"
#include "Libraries/LibDb/DbAction.hpp"
#include "Libraries/LibDb/ConfigImpl.hpp"
#include "Libraries/LibDb/DbPtrImpl.hpp"
#include "Libraries/LibDb/DbContext.hpp"


/**
 * @func   MapTable<C>::MapTable
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbContext::MapTable<C>::MapTable(
    String strTableName
) : IMapTable(strTableName) {
}

/**
 * @func   MapTable<C>::~MapTable
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbContext::MapTable<C>::~MapTable() {
    try {
//        LOG_DEBUG("delete core");
        for (typename Registry_t::iterator it =  Registry.begin();
                it != Registry.end(); ++it) {
            delete it->second;
            it->second = NULL;
        }
//        LOG_DEBUG("delete core done");
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   MapTable<C>::InitMapTable
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
 * @func   DropTable
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
 * @func   PrimaryKeys
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline String
DbContext::MapTable<C>::PrimaryKeys(
) const {
    if (InsteadIdName != String()) {
        return InsteadIdName;
    }

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
 * @func   Add
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Add(
    C* pObject
) {
    DbPtr<C> dbPtr(pObject, this);
    return Add(dbPtr);
}

/**
 * @func   Add
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Add(
    DbPtr<C>& dbPtr
) {
    if (dbPtr.Obj() != NULL) {
        if (dbPtr.Obj()->GetDbContext() == NULL) {
            dbPtr.Obj()->SetDbContext(this);
        }

        if (m_flushMode == MODE_AUTO) {
            FlushObject(dbPtr.Obj());
        } else {
            m_vecObjectToAdd.push_back(dbPtr.Obj());
        }
    }

    return dbPtr;
}

/**
 * @func   Save
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Save(
    DbPtrCore<C>& dbPtr
) {
    try {
        MapTable<C>* pMapping = GetMapping<C>();
        DbSaveObject<C> dbAction(dbPtr, *pMapping);

        dbAction.Config(*dbPtr.Obj());

        if (pMapping != NULL) {
            typename MapTable<C>::Registry_t::iterator it =
            pMapping->Registry.find(dbPtr.GetId());
            if (it == pMapping->Registry.end()) { // If new
                dbPtr.Triggered();
            }
            pMapping->Registry[dbPtr.GetId()] = &dbPtr; // update/add
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   Delete
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Delete(
    DbPtrCore<C>& dbPtr
) {
    try {
        MapTable<C>* pMapping = GetMapping<C>();
        if (pMapping != NULL) {
            SmartPtr<SqlStatement> pStatement =
            GetStatement<C>(DbContext::DELETE);
            pStatement->reset();
            pStatement->bind(0, dbPtr.GetId());
            int_t iRet = pStatement->execute();
            UNUSED(iRet);
//            if ((iRet == SQLITE_DONE) || (iRet == SQLITE_OK)) {
//                pMapping->Registry.erase(dbPtr.GetId());
//            }
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   Load
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Load(
    DbPtrCore<C>&           dbPtrCore,
    SmartPtr<SqlStatement>  pSqlStatement,
    int_t                   iColumn
) {
    MapTable<C>* pMapping = GetMapping<C>();
    DbLoadObject<C> dbAction(dbPtrCore, *pMapping, pSqlStatement);
    dbAction.Config(*dbPtrCore.Obj());
}

/**
 * @func   Load
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtr<C>
DbContext::Load(
    SmartPtr<SqlStatement>  pSqlStatement,
    int_t                   iColumn
) {
    DbPtrCore<C>* pPtrCore = LoadId<C>(pSqlStatement, iColumn);

    if (pPtrCore != NULL) {
        pPtrCore->SetTransaction(DbPtrBase::saved_in_transaction);
        return DbPtr<C>(pPtrCore);
    } else {
        LOG_DEBUG("can't load");
        return DbPtr<C>();
    }
}

/**
 * @func   LoadId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtrCore<C>*
DbContext::LoadId(
    const typename ConfigTable<C>::IdType& Id
) {
    try {
        MapTable<C>* pMapping = GetMapping<C>();
        if (pMapping != NULL) {
            typename MapTable<C>::Registry_t::iterator it =
            pMapping->Registry.find(Id);

            if (it == pMapping->Registry.end()) { // If new
                SmartPtr<SqlStatement> pSqlStatement =
                GetStatement<C>(SELECTBYID);
                pSqlStatement->bind(0, Id);
                return Load<C>(pSqlStatement, 0).Obj();
            } else { // If existed
                DbPtrCore<C>* pPtrCore = it->second;
                return pPtrCore;
            }
        } else {
            throw ExceptionNullPtr("mapping");
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
        throw Exception(ex.what());
    }
}

/**
 * @func   LoadId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbPtrCore<C>*
DbContext::LoadId(
    SmartPtr<SqlStatement>  pSqlStatement,
    int_t                   iColumn
) {
    MapTable<C>*  pMapping = GetMapping<C>();
    DbPtrCore<C>* pPtrCore = new DbPtrCore<C>(new C(), this);

    Load(*pPtrCore, pSqlStatement, iColumn);

    typename MapTable<C>::Registry_t::iterator it =
    pMapping->Registry.find(pPtrCore->GetId());

    if (it == pMapping->Registry.end()) { // If new
        pMapping->Registry[pPtrCore->GetId()] = pPtrCore;
        return pPtrCore;
    } else { // If existed
        delete pPtrCore;
        return it->second;
    }
}

/**
 * @func   Refresh
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::Refresh(
    const typename ConfigTable<C>::IdType& id
) {
    try {
        MapTable<C>* pMapping = GetMapping<C>();
        if (pMapping != NULL) {
            DbPtrCore<C>* pPtrCore = NULL;
            typename MapTable<C>::Registry_t::iterator it =
            pMapping->Registry.find(id);

            if (it == pMapping->Registry.end()) { // If new
                pPtrCore = new DbPtrCore<C>(new C(), this);
                pMapping->Registry[id] = pPtrCore;
                pPtrCore->Triggered();
            } else { // If existed
                pPtrCore = it->second;
            }

            SmartPtr<SqlStatement> pSqlStatement = GetStatement<C>(SELECTBYID);
            pSqlStatement->bind(0, id);

            DbLoadObject<C> dbAction(*pPtrCore, *pMapping, pSqlStatement);
            dbAction.Config(*pPtrCore->Obj());
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   Find
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
 * @func   Command
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

#endif /* !DB_CONTEXT_IMPL_HPP_ */
