/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbAction.hpp
 *
 * Author: TrungTQb
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-07 16:10:00 (Fri, 07 Oct 2016)
 *
 ******************************************************************************/
#ifndef DBACTION_HPP_
#define DBACTION_HPP_

#include "typedefs.h"
#include "debug.hpp"
#include "sqlite3.h"
#include "ValueDb.hpp"
#include "ValueIntDb.hpp"
#include "SetInfo.hpp"
#include "DbPtr.hpp"
#include "Config.hpp"
#include "ConfigImpl.hpp"
#include "SqlStatement.hpp"
#include "DbContext.hpp"

template<class C> class Collection;

/**
 * @func
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
 * @func
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
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t Belong(
    A& action,
    DbPtr<C>& dbPtr,
    const String& strForeignName,
    u32_t dwFlag = ValueDb::OnDeleteCascade
) {
    ValueDb value (strForeignName, ValueType_Integer);
    value.SetFlag(dwFlag | ValueDb::ForeignKey);

    action.ActDbPtr(dbPtr, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t Belong(
    A& action,
    DbPtr<C>& dbPtr,
    typename ConfigTable<C>::IdType& value,
    u32_t dwFlag = ValueDb::Constraint::OnDeleteCascade
) {
    value.SetFlag(dwFlag | ValueDb::ForeignKey);
    action.ActDbPtr(dbPtr, value);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class A, class C>
void_t HasMany(
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
    DbContext& m_dbContext;
    IMapTable& m_mapTable;
public:
    DbInit(DbContext& dbContext, IMapTable& mapTable);
    virtual ~DbInit() {}

    virtual void_t ActInitMapping(IMapTable_p pMapTable);
    template<class V> void_t ActId(V& value);
    template<class V> void_t ActColumn(V& value);
    template<class C> void_t ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
    template<class C> void_t ActCollection(Collection<DbPtr<C>>& collection,
                                           const String& strForeignName);
    template<class C> void_t Config(C& object);
};

typedef DbInit  DbInit_t;
typedef DbInit* DbInit_p;

/**
 * @func
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
 * @func
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
 * @func
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

    if (strFieldName.empty()) { strFieldName = pMapTable->InsteadIdName; }

    value.SetForeignKeyTable(pMapTable->GetTableName());
    value.SetForeignKeyName(value.GetColumnName());

    Column(*this, value);
}

/**
 * @func
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
    String strJoinName = (strForeignName != String()) ? strForeignName : strTableName;
    m_mapTable.SetInfo.push_back(SetInfo(strTableName, strForeignName, strJoinName));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
void_t
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
    virtual ~DbDrop() {}

    template<class V> void_t ActId(V& value);
    template<class V> void_t ActColumn(V& value);
    void_t DropTable(const String& strTable);

    template<class C> void_t ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value) {}
    template<class C> void_t ActCollection(Collection<DbPtr<C>>& collection,
                                           const String& strForeignName) {}
    template<class C> void_t Config(C& object);
};

typedef DbDrop  DbDrop_t;
typedef DbDrop* DbDrop_p;

/**
 * @func
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
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
void_t
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
    u32_t m_dwStatementIndex;
public:
    DbAction(DbPtrBase& dbPtrBase, IMapTable& mapTable);
    virtual ~DbAction() {}

    IMapTable& GetMapTable();
    template<class C> void_t ActCollection(Collection<DbPtr<C>>& collection,
                                           const String& strForeignName);
};

typedef DbAction  DbAction_t;
typedef DbAction* DbAction_p;

template<class C>
inline void_t
DbAction::ActCollection(
    Collection<DbPtr<C>>& collection,
    const String& strForeignName
) {
    DbContext_p pDbContex = m_pDbPtrBase->GetDbContext();

    u32_t dwStatementIndex = DbContext::SELECTSET + m_dwStatementIndex;
    String strSqlStatement = pDbContex->GetSqlStatement(m_pMapTable->TableName, dwStatementIndex);
    size_t posFrom  = strSqlStatement.find(" FROM ");
    String strSqlCountStatement = "SELECT COUNT(1) " + strSqlStatement.substr(posFrom);

    SqlStatement_p pSqlStatement = pDbContex->GetStatement(strSqlStatement);
    SqlStatement_p pSqlCountStatement = pDbContex->GetStatement(strSqlCountStatement);

    m_pDbPtrBase->BindId(pSqlStatement, 0);
    m_pDbPtrBase->BindId(pSqlCountStatement, 0);

    collection = Collection<DbPtr<C>>(pDbContex, pSqlStatement, pSqlCountStatement);

    m_dwStatementIndex++;
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class DbSaveAction : public DbAction {
private:

protected:
    SqlStatement_p m_pStatement;
    u32_t   m_dwColumn;
public:
    DbSaveAction(DbPtrBase& dbPtrBase,
                 IMapTable& mapTable,
                 SqlStatement_p pStatement = NULL,
                 u32_t dwColumn = 0);

    virtual ~DbSaveAction() { m_dwColumn = 0;}

    template<class V> void_t ActColumn(V& value);
    template<class C> void_t ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
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
    } else { // bind null
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
    if (value.IsChanged()) { // Not Complete
        value.ResetChange();
        value.ResetBindNull();
        dbPtr = m_pDbPtrBase->GetDbContext()->Load<C>(value);
    }

    DbPtrCore<C>* pPtrCore = dbPtr.Object();
    if (pPtrCore->Object() != NULL) {
        pPtrCore->BindId(m_pStatement, m_dwColumn++);
        value = pPtrCore->GetId();
        m_pDbPtrBase->GetDbContext()->FlushObject(pPtrCore);
    } else {
        Column(*this, value);
    }
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbSaveObject : public DbSaveAction {
private:
    DbPtrCore<C>& m_dbPtrCore;
    bool_t m_boIsInsert;
public:
    DbSaveObject(DbPtrCore<C>& dbPtrCore, DbContext::MapTable<C>& mapping);
    virtual ~DbSaveObject() {}

    template<class V> void_t ActId(V& value);

    void_t Config(C& object);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbSaveObject<C>::DbSaveObject(
    DbPtrCore<C>& dbPtrCore,
    DbContext::MapTable<C>& mapping
) : DbSaveAction (dbPtrCore, mapping),
    m_dbPtrCore (dbPtrCore),
    m_boIsInsert (TRUE) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbSaveObject<C>::Config(
    C& object
) {
    DbContext* pDbContext = m_dbPtrCore.GetDbContext();

    if (m_pStatement == NULL) {
        m_boIsInsert = !m_dbPtrCore.IsRefreshed();
        m_pStatement = (m_boIsInsert) ? pDbContext->GetStatement<C>(DbContext::INSERT) :
                                        pDbContext->GetStatement<C>(DbContext::UPDATE);
    } else {
        m_boIsInsert = FALSE;
    }

    typename ConfigTable<C>::IdType value = m_dbPtrCore.GetId();

    m_pStatement->reset(); m_pStatement->ubind();
    m_dwColumn = 0;

    Scan(object, *this);

    if (!m_boIsInsert) { m_pStatement->bind(m_dwColumn++, value); }

    if (m_pStatement->execute() != SQLITE_DONE) {
        if (!m_boIsInsert) { // Refresh if error
            m_pDbPtrBase->GetDbContext()->Refresh<C>(value);
        }
    }

    if (m_boIsInsert && (m_pMapTable->InsteadIdName != String())) {
        m_dbPtrCore.SetId(m_pStatement->insertId());
    }

    m_dbPtrCore.SetTransaction(DbPtrBase::saved_in_transaction);
    m_dwColumn = 0;
}

/**
 * @func
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
    SqlStatement_p m_pStatement;
    u32_t m_dwColumn;
public:
    DbLoadAction(DbPtrBase& dbPtrBase,
                 IMapTable& mapTable,
                 SqlStatement_p pStatement = NULL,
                 u32_t dwColumn = 0);

    virtual ~DbLoadAction() {}

    template<class V> void_t ActColumn(V& value);
    template<class C> void_t ActDbPtr(DbPtr<C>& dbPtr, typename ConfigTable<C>::IdType& value);
};

typedef DbLoadAction  DbLoadAction_t;
typedef DbLoadAction* DbLoadAction_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class V>
inline void_t
DbLoadAction::ActColumn(
    V& value
) {
    m_pStatement->get(m_dwColumn++, value);
}

/**
 * @func
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
    DbContext_p pDbContex = m_pDbPtrBase->GetDbContext();
    dbPtr = pDbContex->Load<C>(value);
}

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
template<class C>
class DbLoadObject : public DbLoadAction {
private:
    DbPtrCore<C>& m_dbPtrCore;
public:
    DbLoadObject(DbPtrCore<C>& dbPtrCore,
                 DbContext::MapTable<C>& mapping,
                 SqlStatement_p pSqlStatement = NULL);
    virtual ~DbLoadObject() {}

    void_t GetInsteadId();
    template<class V> void_t ActId(V& value);
    void_t Config(C& object);
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline
DbLoadObject<C>::DbLoadObject(
    DbPtrCore<C>& dbPtrCore,
    DbContext::MapTable<C>& mapping,
    SqlStatement_p pSqlStatement
) : DbLoadAction (dbPtrCore, mapping, pSqlStatement),
    m_dbPtrCore (dbPtrCore) {

}

/**
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbLoadObject<C>::Config(
    C& object
) {
    m_dwColumn = 0;

    if (m_pStatement == NULL) {
        DbContext_p pDbContex = m_pDbPtrBase->GetDbContext();
        m_pStatement = pDbContex->GetStatement<C>(DbContext::SELECTBYID);
        // Need bind ID
    }

    m_pStatement->execute();
    GetInsteadId();
    Scan(object, *this);
    m_dwColumn = 0;
}

/**
 * @func
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

#endif /* !DBACTION_HPP_ */
