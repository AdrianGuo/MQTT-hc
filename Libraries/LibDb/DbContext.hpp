/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbContext.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-08 07:10:00 (Sat, 08 Oct 2016)
 *
 ******************************************************************************/
#ifndef DBCONTEXT_HPP_
#define DBCONTEXT_HPP_

#include <iostream>
#include <typeinfo>
#include <type_traits>

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/SetInfo.hpp"
#include "Libraries/LibDb/ValueDb.hpp"
#include "Libraries/LibDb/Config.hpp"
#include "Libraries/LibDb/Sqlite3Statement.hpp"
#include "Libraries/LibDb/SqlStatement.hpp"

/******************************************************************************/
/*                                 LIST CLASS                                 */
/******************************************************************************/
class DbContext;
class DbPtrBase;

template<class C> class DbPtr;
template<class C> class DbPtrCore;
template<class R> class Query;

/******************************************************************************/
/*                                   STRUCT                                   */
/******************************************************************************/
struct IMapTable {
    String              TableName;
    String              InsteadIdName;
    String              NaturalIdName;
    Vector<ValueDb>     Columns;
    Vector<SetInfo_t>   SetInfo;
    Vector<String>      Statements;
    bool_t              Initialized;

    IMapTable(String strTableName = String());
    virtual ~IMapTable();

    void_t SetTableName(String strTableName);
    String GetTableName() const;

    virtual void_t InitMapTable(DbContext& dbContext) = 0;
    virtual void_t DropTable   (DbContext& dbContext) = 0;
    virtual String PrimaryKeys() const = 0;
};

typedef struct IMapTable  IMapTable_t;
typedef struct IMapTable* IMapTable_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
typedef enum _FlushMode_ { MODE_AUTO = 0, MODE_MANUAL } FlushMode_t;

class DbContext {
private:
    template<class C>
    struct MapTable : public IMapTable {
        typedef Map<typename ConfigTable<C>::IdType, DbPtrCore<C>*> Registry_t;
        Registry_t Registry;

        MapTable(String strTableName = String());
        virtual ~MapTable();

        virtual void_t DropTable(DbContext& dbContext);
        virtual void_t InitMapTable(DbContext& dbContext);
        virtual String PrimaryKeys() const;
    };

    Database_p  m_pDatabase;
    String      m_strRamFileName;
    String      m_strFlashFileName;

    typedef const std::type_info* const_type_info_ptr;
    typedef Map<String, IMapTable_p>                TableRegistry;
    typedef Map<const_type_info_ptr, IMapTable_p>   ClassRegistry;

    ClassRegistry       m_classRegistry;
    TableRegistry       m_tableRegistry;
    FlushMode_t         m_flushMode;
    List<DbPtrBase*>    m_lstObject;
    Vector<DbPtrBase*>  m_vecObjectToAdd;

    void_t InitSchema();
    void_t InitStatements(IMapTable_p pMapTable);
    void_t CreateTable(IMapTable_p pMapTable, Set<String>& tablesCreated);
    String ConsTraintString(IMapTable_p pMapTable, ValueDb& valueForeign);
    String ConsTraintName(const String strTableName, const String strForeignName);

public:
    enum { INSERT = 0, UPDATE = 1, DELETE = 2, SELECTBYID = 3, SELECTSET = 4 };

    DbContext(const String& strDbName = String());
    virtual ~DbContext();

    template<class C> void_t MapDbTable(const String strTableName);
    template<class C> MapTable<C>* GetMapping() const;
    IMapTable_p GetMapping(const String strTableName) const;

    template<class C> const String TableName() const;

    template<class C> SqlStatement_p GetStatement(u32_t dwStatementId) const;
    SqlStatement_p GetStatement(String strSql);

    const String& GetSqlStatement(const String strTableName, u32_t dwStatementId) const;

    template<class C> void_t GetColumns(Vector<ValueDb>& Result);
    void_t GetColumns(const String strTableName, Vector<ValueDb>& result);

    template<class C> DbPtr<C> Add(C* pObject);
    template<class C> DbPtr<C> Add(DbPtr<C>& dbPtr);

    template<class C> void_t Save  (DbPtrCore<C>& dbPtr);
    template<class C> void_t Delete(DbPtrCore<C>& dbPtr);

    template<class C> void_t Load(DbPtrCore<C>& dbPtr, SmartPtr<SqlStatement> pSqlStatement, int_t iColumn);
    template<class C> DbPtr<C> Load(SmartPtr<SqlStatement> pSqlStatement, int_t iColumn);

    template<class C> DbPtrCore<C>* LoadId(const typename ConfigTable<C>::IdType& id);
    template<class C> DbPtrCore<C>* LoadId(SmartPtr<SqlStatement> pSqlStatement, int_t iColumn);
    template<class C> void_t Refresh(const typename ConfigTable<C>::IdType& id);

    template<class C> Query<DbPtr<C>> Find(const String& strWhere = String());
    template<class R> Query<R> Command(const String& strSql);

    void_t DropTables();
    void_t CreateTables();
    void_t UpdateChanges();

    void_t ExecuteSql(const String& strSql);

    void_t FlushObject   (DbPtrBase* pDbPtrObj);
    void_t DisCardChanges(DbPtrBase* pDbPtrObj);

    FlushMode_t GetFlushMode() const;
    void_t SetFlushMode(FlushMode_t flushMode);
};

typedef DbContext  DbContext_t;
typedef DbContext* DbContext_p;

/**
 * @func   MapDbTable
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::MapDbTable(
    const String strTableName
) {
    MapTable<C>* pMapTable = new MapTable<C>(strTableName);
    pMapTable->SetTableName(strTableName);

    m_classRegistry[  &typeid(C)] = pMapTable;
    m_tableRegistry[strTableName] = pMapTable;
}

/**
 * @func   MapTable
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline DbContext::MapTable<C>*
DbContext::GetMapping(
) const {
    typedef typename std::remove_const<C>::type C_t;
    ClassRegistry::const_iterator it = m_classRegistry.find(&typeid(C_t));
    if (it != m_classRegistry.end()) {
        return dynamic_cast<MapTable<C>*>(it->second);
    }

    return NULL;
}

/**
 * @func   GetColumns
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline void_t
DbContext::GetColumns(
    Vector<ValueDb>& Result
) {
    typedef typename std::remove_const<C>::type C_t;
    ClassRegistry::const_iterator it = m_classRegistry.find(&typeid(C_t));
    if (it != m_classRegistry.end()) {
        IMapTable_p pMapTable = it->second;
        if (pMapTable != NULL) {
            Result.insert(Result.end(), pMapTable->Columns.begin(),
            pMapTable->Columns.end());
        }
    }
}

/**
 * @func   TableName
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline const String
DbContext::TableName(
) const {
    typedef typename std::remove_const<C>::type C_t;
    ClassRegistry::const_iterator it = m_classRegistry.find(&typeid(C_t));
    if (it != m_classRegistry.end()) {
        return it->second->TableName;
    }
    return String();
}

/**
 * @func   GetStatement
 * @brief  None
 * @param  None
 * @retval None
 */
template<class C>
inline SqlStatement_p
DbContext::GetStatement(
    u32_t dwStatementId
) const {
    ClassRegistry::const_iterator it = m_classRegistry.find(&typeid(C));
    String strSql = it->second->Statements[dwStatementId];

    return new Sqlite3Statement(m_pDatabase, strSql);
}

#endif /* !DBCONTEXT_HPP_ */
