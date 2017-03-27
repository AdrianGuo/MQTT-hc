/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbAction.hpp
 *
 * Author: TrungTQb
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-07 16:10:00 (Fri, 07 Oct 2016)
 *
 ******************************************************************************/
#ifndef DBACTION_HPP_
#define DBACTION_HPP_

#include "../Typedefs.h"
#include "../SmartPtr.hpp"
#include "sqlite3.h"
#include "ValueDb.hpp"
#include "ValueIntDb.hpp"
#include "SetInfo.hpp"
#include "DbPtr.hpp"
#include "DbChecker.hpp"
#include "ConfigImpl.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"
#include "Config.hpp"

template<class C> class Collection;

/**
 * @func   Table
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename V, class C>
bool_t
Look(
    C&      obj,
    V       value,
    u32_t   dwIndex
) {
//    return obj.Check(value, dwIndex);
//    return Check<dwIndex, C, V>::Compare(obj, value);
    return FALSE;
}


/**
 * @func   Table
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t Scan(
    C& obj,
    A& action
) {
    obj.Table(action);
}

/**
 * @func   Column
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class V>
void_t Column(
    A& action,
    V& value,
    u32_t dwFlag = ValueDb::IsNull
) {
    value.SetFlag(dwFlag);
    action.ActColumn(value);
}

/**
 * @func   Id
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class V>
void_t Id(
    A& action,
    V& value,
    u32_t dwFlag = ValueDb::NaturalId
) {
    value.SetFlag(dwFlag);
    action.ActId(value);
}

/**
 * @func   Belong
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t
Belong(
    A& action,
    DbPtr<C>& dbPtr,
    const String& strForeignName,
    u32_t dwFlag = ValueDb::OnDeleteCascade
) {
    ValueDb value (strForeignName, Value::Type_t::type_interger);
    value.SetFlag(dwFlag | ValueDb::ForeignKey);

    action.ActDbPtr(dbPtr, value);
}

/**
 * @func   Belong
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t
Belong(
    A&          action,
    DbPtr<C>&   dbPtr,
    typename ConfigTable<C>::IdType& value,
    u32_t dwFlag = ValueDb::Constraint::OnDeleteCascade
) {
    value.SetFlag(dwFlag | ValueDb::ForeignKey);
    action.ActDbPtr(dbPtr, value);
}

/**
 * @func   HasMany
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t
HasMany(
    A& action,
    Collection<DbPtr<C>>& collection,
    const String& strForeignName = String()
) {
    action.ActCollection(collection, strForeignName);
}

/******************************************************************************/
/*                                 LIST CLASS                                 */
/******************************************************************************/


/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbInit {
private:
    u32_t      m_dwIndex;
    DbContext& m_dbContext;
    IMapTable& m_mapTable;
public:
    DbInit(DbContext& dbContext, IMapTable& mapTable);
    virtual ~DbInit();

    virtual void_t ActInitMapping(IMapTable_p pMapTable);
    template<class V> void_t ActId(V& value);
    template<class V> void_t ActColumn(V& value);
    template<class C> void_t ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
    template<class C> void_t
    ActCollection(Collection<DbPtr<C>>& collection, const String& strForeignName);
    template<class C> void_t Config(C& object);
};

typedef DbInit  DbInit_t;
typedef DbInit* DbInit_p;

/**
 * @func   ActId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbInit::ActId(
    V& value
) {
    m_mapTable.NaturalIdName = value.GetColumnName();
    value.SetNaturalId();

    Column(*this, value);
}

/**
 * @func   ActColumn
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbInit::ActColumn(
    V& value
) {
    m_mapTable.Columns.push_back(value);
}

/**
 * @func   ActDbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbInit::ActDbPtr(
    DbPtr<C>& dbPtr,
    typename ConfigTable<C>::IdType& value
) {
    IMapTable_p pMapTable = m_dbContext.GetMapping<C>();
    ActInitMapping(pMapTable);

    String strFieldName = pMapTable->NaturalIdName;

    if (strFieldName.empty()) {
        strFieldName = pMapTable->InsteadIdName;
    }

    value.SetForeignKeyTable(pMapTable->GetTableName());
    value.SetForeignKeyName(value.GetColumnName());

    Column(*this, value);
}

/**
 * @func   ActCollection
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbInit::ActCollection(
    Collection<DbPtr<C>>& collection,
    const String& strForeignName
) {
    String strTableName = m_dbContext.TableName<C>();
    String strJoinName  =
    (strForeignName != String()) ? strForeignName : strTableName;
    m_mapTable.SetInfo.push_back(
    SetInfo(strTableName, strForeignName, strJoinName));
}

/**
 * @func   Config
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbInit::Config(C& object) {
    m_mapTable.InsteadIdName = ConfigTable<C>::InsteadIdField();
    Scan(object, *this);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbDrop {
private:
    DbContext& m_dbContext;
    IMapTable& m_mapTable;
public:
    DbDrop(DbContext& dbContext, IMapTable& mapTable);
    virtual ~DbDrop();

    template<class V> void_t ActId(V& value);
    template<class V> void_t ActColumn(V& value);
    void_t DropTable(const String& strTable);

    template<class C> void_t
    ActDbPtr(DbPtr<C>& dbPtr,
             typename ConfigTable<C>::IdType& value) {}
    template<class C> void_t
    ActCollection(Collection<DbPtr<C>>& collection,
                  const String& strForeignName) {}
    template<class C> void_t Config(C& object);
};

typedef DbDrop  DbDrop_t;
typedef DbDrop* DbDrop_p;

/**
 * @func   ActId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbDrop::ActId(
    V& value
) {

}

/**
 * @func   ActColumn
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbDrop::ActColumn(
    V& value
) {

}

/**
 * @func   Config
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbDrop::Config(C& object) {
    DropTable(m_mapTable.TableName);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbAction {
protected:
    DbPtrBase_p m_pDbPtrBase;
    IMapTable_p m_pMapTable;
    u32_t       m_dwStatementIdx;
public:
    DbAction(DbPtrBase& dbPtrBase, IMapTable& mapTable);
    virtual ~DbAction();

    IMapTable& GetMapTable();
    template<class C> void_t
    ActCollection(Collection<DbPtr<C>>& collection,
                  const String& strForeignName);
};

typedef DbAction  DbAction_t;
typedef DbAction* DbAction_p;

/**
 * @func   ActCollection
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbAction::ActCollection(
    Collection<DbPtr<C>>&   collection,
    const String&           strForeignName
) {
    try {
//        LOG_DEBUG("action collection");
        DbContext_p pDbContext = m_pDbPtrBase->GetDbContext();

        if (pDbContext != NULL) {
            u32_t dwStatementIndex = DbContext::SELECTSET + m_dwStatementIdx;

            String strSqlStatement =
            pDbContext->GetSqlStatement(m_pMapTable->TableName, dwStatementIndex);

            size_t posFrom = strSqlStatement.find(" FROM ");

            String strSqlCountStatement =
            "SELECT COUNT(1) " + strSqlStatement.substr(posFrom);

            SmartPtr<SqlStatement> pSqlStatement =
            pDbContext->GetStatement(strSqlStatement);

            SmartPtr<SqlStatement> pSqlCountStatement =
            pDbContext->GetStatement(strSqlCountStatement);

            m_pDbPtrBase->BindId(pSqlStatement, 0);
            m_pDbPtrBase->BindId(pSqlCountStatement, 0);

            collection =
            Collection<DbPtr<C>>(pDbContext, pSqlStatement, pSqlCountStatement);
            collection.SetRelationData(m_pDbPtrBase);

            m_dwStatementIdx++;
        }
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbSaveAction : public DbAction {
protected:
    SmartPtr<SqlStatement>  m_pStatement;
    u32_t                   m_dwColumn;
public:
    DbSaveAction(DbPtrBase&             dbPtrBase,
                 IMapTable&             mapTable,
                 SmartPtr<SqlStatement> pStatement  = NULL,
                 u32_t                  dwColumn    = 0);

    virtual ~DbSaveAction();

    template<class V> void_t ActColumn(V& value);
    template<class C> void_t
    ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
};

typedef DbSaveAction  DbSaveAction_t;
typedef DbSaveAction* DbSaveAction_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbSaveAction::ActColumn(
    V& value
) {
    if (value.IsChanged()) {
        value.ResetBindNull();
        value.ResetChange();
    }

    if (!value.IsBindNull()) {
        m_pStatement->bind(m_dwColumn++, value);
    } else {
        value.SetValueDefault();
        m_dwColumn++;
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
DbSaveAction::ActDbPtr(
    DbPtr<C>& dbPtr,
    typename ConfigTable<C>::IdType& value
) {
    DbContext_p pDbContext = m_pDbPtrBase->GetDbContext();

    if (value.IsChanged()) {
        value.ResetBindNull();
        if (pDbContext != NULL) {
            dbPtr = pDbContext->LoadId<C>(value);
        }
    }

    if (dbPtr.Obj() != NULL) {
        dbPtr.Obj()->BindId(m_pStatement, m_dwColumn++);
        value = dbPtr.Obj()->GetId();
        if (pDbContext != NULL) {
            pDbContext->FlushObject(dbPtr.Obj());
        }
    } else {
        value.SetBindNull();
        Column(*this, value);
    }
    value.ResetChange();
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbSaveObject : public DbSaveAction {
private:
    DbPtrCore<C>&   m_dbPtrCore;
    bool_t          m_boIsInsert;
public:
    DbSaveObject(DbPtrCore<C>& dbPtrCore, DbContext::MapTable<C>& mapping);
    virtual ~DbSaveObject();

    template<class V> void_t ActId(V& value);

    void_t Config(C& object);
};

/**
 * @func   DbSaveObject
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbSaveObject<C>::DbSaveObject(
    DbPtrCore<C>&           dbPtrCore,
    DbContext::MapTable<C>& mapping
) : DbSaveAction (dbPtrCore, mapping),
    m_dbPtrCore  (dbPtrCore),
    m_boIsInsert (TRUE) {
}

/**
 * @func   ~DbSaveObject
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbSaveObject<C>::~DbSaveObject() {

}

/**
 * @func   Config
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbSaveObject<C>::Config(
    C& object
) {
    try {
        DbContext_p pDbContext = m_dbPtrCore.GetDbContext();

        if (m_pStatement.get() == NULL) {
            m_boIsInsert = !m_dbPtrCore.IsRefreshed();
            if (pDbContext != NULL) {
                m_pStatement = (m_boIsInsert) ?
                pDbContext->GetStatement<C>(DbContext::INSERT):
                pDbContext->GetStatement<C>(DbContext::UPDATE);
            }
        } else {
            m_boIsInsert = FALSE;
        }

        typename ConfigTable<C>::IdType value = m_dbPtrCore.GetId();

        m_pStatement->reset();
        m_pStatement->ubind();
        m_dwColumn = 0;

        Scan(object, *this); // Binding statements

        if (!m_boIsInsert) { // if update
            m_pStatement->bind(m_dwColumn++, value); // Binding Id
        }

        int_t iRet = m_pStatement->execute();


        if ((iRet != SQLITE_DONE) || (iRet != SQLITE_DONE)) {
            if (!m_boIsInsert && (pDbContext != NULL)) { // If update
                pDbContext->Refresh<C>(value); // Refresh if error
            }
        }

        if (m_boIsInsert && (m_pMapTable->InsteadIdName != String())) { // If insert
            m_dbPtrCore.SetId(m_pStatement->insertId());
        }

        m_dbPtrCore.SetTransaction(DbPtrBase::saved_in_transaction);
        m_dwColumn = 0;
    } catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    }
}

/**
 * @func   ActId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<class V>
inline void_t
DbSaveObject<C>::ActId(
    V& value
) {
    Column(*this, value);
    value.ResetChange();
    m_dbPtrCore.SetId(value);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbLoadAction : public DbAction {
protected:
    SmartPtr<SqlStatement> m_pStatement;
    u32_t                  m_dwColumn;
public:
    DbLoadAction(DbPtrBase&             dbPtrBase,
                 IMapTable&             mapTable,
                 SmartPtr<SqlStatement> pStatement = NULL,
                 u32_t dwColumn = 0);

    virtual ~DbLoadAction();

    template<class V> void_t ActColumn(V& value);
    template<class C> void_t
    ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
};

typedef DbLoadAction  DbLoadAction_t;
typedef DbLoadAction* DbLoadAction_p;

/**
 * @func   ActColumn
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbLoadAction::ActColumn(
    V& value
) {
    if (m_pStatement->get(m_dwColumn++, value)) {
        value.ResetBindNull();
    }
}

/**
 * @func   ActDbPtr
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbLoadAction::ActDbPtr(
    DbPtr<C>& dbPtr,
    typename ConfigTable<C>::IdType& value
) {
    Column(*this, value);
    if (!value.IsBindNull()) {
        DbContext_p pDbContext = m_pDbPtrBase->GetDbContext();
        if (pDbContext != NULL) {
            dbPtr = pDbContext->LoadId<C>(value);
        }
    }
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbLoadObject : public DbLoadAction {
private:
    DbPtrCore<C>& m_dbPtrCore;
public:
    DbLoadObject(DbPtrCore<C>&           dbPtrCore,
                 DbContext::MapTable<C>& mapping,
                 SmartPtr<SqlStatement>  pSqlStatement = NULL);
    virtual ~DbLoadObject();

    void_t GetInsteadId();
    template<class V> void_t ActId(V& value);
    void_t Config(C& object);
};

/**
 * @func   DbLoadObject
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbLoadObject<C>::DbLoadObject(
    DbPtrCore<C>&           dbPtrCore,
    DbContext::MapTable<C>& mapping,
    SmartPtr<SqlStatement>  pSqlStatement
) : DbLoadAction (dbPtrCore, mapping, pSqlStatement),
    m_dbPtrCore  (dbPtrCore) {
}

/**
 * @func   ~DbLoadObject
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbLoadObject<C>::~DbLoadObject() {

}

/**
 * @func   GetInsteadId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbLoadObject<C>::GetInsteadId() {
    if (GetMapTable().InsteadIdName != String()) {
        int_t iInsteadId = 0;
        m_pStatement->get(m_dwColumn++, &iInsteadId);
        m_dbPtrCore.SetId(iInsteadId);
    }
}

/**
 * @func   Config
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbLoadObject<C>::Config(
    C& object
) {
    m_pStatement->execute();
    m_dwColumn = 0;
    GetInsteadId();
    Scan(object, *this);
    m_dwColumn = 0;
}

/**
 * @func   ActId
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<class V>
inline void_t
DbLoadObject<C>::ActId(
    V& value
) {
    Column(*this, value);
    m_dbPtrCore.SetId(value);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbFind {
public:
    DbFind();
    virtual ~DbFind();
};

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbFindObj : public DbFind {
public:
    DbFindObj();
    virtual ~DbFindObj();

    template<typename V>
    bool_t Find(DbPtrCore<C>* pDbPtrObj, V value, u32_t dwIndex);
};

/**
 * @func   DbFindObj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbFindObj<C>::DbFindObj(
) {
}

/**
 * @func   ~DbFindObj
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbFindObj<C>::~DbFindObj() {

}

/**
 * @func   Config
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
template<typename V>
inline bool_t
DbFindObj<C>::Find(
    DbPtrCore<C>*   pDbPtrObj,
    V               value,
    u32_t           dwIndex
) {
    return Look(*pDbPtrObj->Obj(), value, dwIndex);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

#endif /* !DBACTION_HPP_ */
