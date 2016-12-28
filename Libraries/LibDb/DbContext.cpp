#include "ConfigDb.hpp"
#include "HelperHc.hpp"
#include "DbPtr.hpp"
#include "DbContext.hpp"

/**
 * @func   DbContext
 * @brief  None
 * @param  None
 * @retval None
 */
DbContext::DbContext(
    const String& strDbName
) : m_strRamFileName (PATH_RAM),
    m_strFlashFileName(PATH_FLASH) {
    m_strRamFileName += strDbName;
    m_strFlashFileName += strDbName;
    CopyFile(m_strFlashFileName.c_str(), m_strRamFileName.c_str());
    m_pDatabase = new Database(strDbName);
    m_flushMode = MODE_MANUAL;
}

/**
 * @func   ~DbContext
 * @brief  None
 * @param  None
 * @retval None
 */
DbContext::~DbContext() { }

/**
 * @func   GetMapping
 * @brief  None
 * @param  None
 * @retval None
 */
IMapTable_p
DbContext::GetMapping(
    const String strTableName
) const {
    TableRegistry::const_iterator_t it = m_tableRegistry.find(strTableName);
    if (it != m_tableRegistry.end()) {
        return it->second;
    }

    return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SqlStatement_p
DbContext::GetStatement(
    String strSql
) {
    return new Sqlite3Statement(m_pDatabase, strSql);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const String&
DbContext::GetSqlStatement(
    const String strTableName,
    u32_t dwStatementId
) const {
    return GetMapping(strTableName)->Statements[dwStatementId];
}

/**
 * @func   UpdateChanges
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::UpdateChanges() {
    for (u32_t i = 0; i < m_vecObjecToAdd.size(); i++) {
        FlushObject(m_vecObjecToAdd[i]);
    }

    m_vecObjecToAdd.clear();

    List<DbPtrBase_p>::iterator_t it = m_lstObjectManager.begin();

    while (it != m_lstObjectManager.end()) {
       (*it)->UpdateChange();
       m_lstObjectManager.erase(it++);
    }

    remove(m_strFlashFileName.c_str());
    CopyFile(m_strRamFileName.c_str(), m_strFlashFileName.c_str());
}

/**
 * @func   Execute
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::Execute(
    const String& strSql
) {
    SqlStatement_p pSqlStatement = GetStatement(strSql);
    pSqlStatement->execute();
    delete pSqlStatement;
}

/**
 * @func   DisCardChanges
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::DisCardChanges(
    DbPtrBase_p pDbPtrObject
) {
    m_lstObjectManager.remove(pDbPtrObject);
}

/**
 * @func   GetColumns
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::GetColumns(
    const String strTableName,
    Vector<ValueDb>& result
) {
    IMapTable_p pMapTable = GetMapping(strTableName);

    if (pMapTable == NULL) { return; }

    if (pMapTable->InsteadIdName != String()) {
        ValueDb value (pMapTable->InsteadIdName, Value::Type_t::type_interger);
        value.SetNaturalId();
        result.push_back(value);
    }

    result.insert(result.end(), pMapTable->Columns.begin(), pMapTable->Columns.end());
}

/**
 * @func   CreateTables
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::CreateTables() {
    InitSchema();

    Execute("PRAGMA encoding = \"UTF-8\"");
    Execute("PRAGMA foreign_keys = ON");

    Set<String> tablesCreated;

    for (ClassRegistry::iterator_t it = m_classRegistry.begin();
            it != m_classRegistry.end(); it++) {
        CreateTable(it->second, tablesCreated);
    }
}

/**
 * @func   DropTables
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::DropTables() {
    for (ClassRegistry::iterator_t it = m_classRegistry.begin();
            it != m_classRegistry.end(); it++) {
        it->second->DropTable(*this);
    }
}

/**
 * @func   InitSchema
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::InitSchema() {
    for (ClassRegistry::const_iterator_t it = m_classRegistry.begin();
            it != m_classRegistry.end(); it++) {
        it->second->InitMapTable(*this);
    }

    for (ClassRegistry::const_iterator_t it = m_classRegistry.begin();
                it != m_classRegistry.end(); it++) {
        InitStatements(it->second);
    }
}

/**
 * @func   ConsTraintString
 * @brief  None
 * @param  None
 * @retval None
 */
String
DbContext::ConsTraintString(
    IMapTable_p pMapTable,
    ValueDb& valueForeign
) {
    String strConsTraint;
    strConsTraint.clear();

    strConsTraint = "CONSTRAINT \"FK_" + pMapTable->TableName + "_" +
                    valueForeign.GetForeignKeyTable() + "\" FOREIGN KEY (\"" +
                    valueForeign.GetColumnName();

    IMapTable_p pOtherMapTable = GetMapping(valueForeign.GetForeignKeyTable());

    strConsTraint += "\") REFERENCES \"" + pOtherMapTable->TableName + "\" (" +
                     pOtherMapTable->PrimaryKeys() +")";

    if (valueForeign.IsOnDeleteCascade()) {
        strConsTraint += " ON DELETE CASCADE";
    } else if (valueForeign.IsOnDeleteSetNull()) {
        strConsTraint += " ON DELETE SET NULL";
    }

    if (valueForeign.IsOnUpdateCascade()) {
        strConsTraint += " ON UPDATE CASCADE";
    } else if (valueForeign.IsOnUpdateSetNull()) {
        strConsTraint += " ON UPDATE SET NULL";
    }


    return strConsTraint;
}

/**
 * @func   ConsTraintName
 * @brief  None
 * @param  None
 * @retval None
 */
String
DbContext::ConsTraintName(
    const String strTableName,
    const String strForeignName
) {
    String strConsTraintName;
    strConsTraintName = "FK_" + strTableName +"_" + strForeignName;
    return strConsTraintName;
}

/**
 * @func   FlushObject
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::FlushObject(
    DbPtrBase_p pDbPtrObject
) {
    m_lstObjectManager.push_back(pDbPtrObject);
}

/**
 * @func   InitStatements
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::InitStatements(
    IMapTable_p pMapTable
) {
    bool_t boFirstColumn = TRUE;
    String strSql;

    /* Insert */
    strSql += "INSERT INTO \"" + pMapTable->TableName + "\" (";
    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (!boFirstColumn) { strSql += ", "; }
        strSql += "\"" + pMapTable->Columns[i].GetColumnName() +"\"";
        boFirstColumn = FALSE;
    }
    strSql += ") VALUES (";
    boFirstColumn = TRUE;
    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (!boFirstColumn) { strSql += ", "; }
        strSql += "?";
        boFirstColumn = FALSE;
    }
    strSql += ")";
    pMapTable->Statements.push_back(strSql);

    /* Update */
    strSql.clear();
    boFirstColumn = TRUE;
    strSql += "UPDATE \"" + pMapTable->TableName + "\" SET ";
    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (!boFirstColumn) { strSql += ", "; }
        strSql += "\"" + pMapTable->Columns[i].GetColumnName() + "\" = ?";
        boFirstColumn = FALSE;
    }

    strSql += " WHERE";

    String strCondition;

    if (pMapTable->InsteadIdName == String()) {
        boFirstColumn = TRUE;
        for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
            if (pMapTable->Columns[i].IsNaturalId()) {
                if (!boFirstColumn) { strCondition = " AND "; }
                strCondition += " \"" + pMapTable->NaturalIdName + "\" = ?";
                boFirstColumn = FALSE;
            }
        }
    } else {
        strCondition += " \"" + pMapTable->InsteadIdName + "\" = ?";
    }

    strSql += strCondition;
    pMapTable->Statements.push_back(strSql);

    /* Delete */
    strSql.clear();
    strSql = "DELETE FROM \"" + pMapTable->TableName + "\" WHERE" + strCondition;
    pMapTable->Statements.push_back(strSql);

    /* Select by Id */
    strSql.clear();
    boFirstColumn = TRUE;
    strSql = "SELECT ";

    if (pMapTable->InsteadIdName != String()) {
        strSql += " \"" + pMapTable->InsteadIdName + "\"";
        boFirstColumn = FALSE;
    }

    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (!boFirstColumn) { strSql += ", "; }
        strSql += "\"" + pMapTable->Columns[i].GetColumnName() + "\"";
        boFirstColumn = FALSE;
    }
    strSql += " FROM \"" + pMapTable->TableName + "\" WHERE" + strCondition;
    pMapTable->Statements.push_back(strSql);

    /* Collection */
    String strFkConditions;

    for (u32_t i = 0; i < pMapTable->SetInfo.size(); i++) {
        strSql.clear();
        IMapTable_p pOtherMapTable = GetMapping(pMapTable->SetInfo[i].TableName);
        strSql = "SELECT ";
        boFirstColumn = TRUE;
        if (pOtherMapTable->InsteadIdName != String()) {
            strSql += "\"" + pOtherMapTable->InsteadIdName + "\"";
            boFirstColumn = FALSE;
        }

        for (u32_t j = 0; j < pOtherMapTable->Columns.size(); j++) {
            if (!boFirstColumn) { strSql += ", "; }
            boFirstColumn = FALSE;
            strSql += "\"" + pOtherMapTable->Columns[j].GetColumnName() + "\"";
            if (pOtherMapTable->Columns[j].IsForeignkey() &&
                pOtherMapTable->Columns[j].GetForeignKeyTable() == pMapTable->GetTableName()) {
                if (!strFkConditions.empty()) { strFkConditions += " AND "; }
                strFkConditions += "\"" + pOtherMapTable->Columns[j].GetColumnName() + "\" = ?";
            }
        }

        strSql += " FROM \"" + pOtherMapTable->TableName + "\" WHERE " + strFkConditions;
        pMapTable->Statements.push_back(strSql);
    }
    strSql.clear();
}

/**
 * @func   CreateTable
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DbContext::CreateTable(
    IMapTable_p  pMapTable,
    Set<String>& tablesCreated
) {
    tablesCreated.insert(pMapTable->TableName);
    bool_t boFirstColumn = TRUE;
    String strSql;

    strSql.clear();
    strSql = "CREATE TABLE IF NOT EXISTS \"" + pMapTable->TableName + "\"(\n";

    if (pMapTable->InsteadIdName != String()) {
        strSql += " \"" + pMapTable->InsteadIdName + "\" INTEGER PRIMARY KEY AUTOINCREMENT";
        boFirstColumn = FALSE;
    }

    String strPrimaryKey;

    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (!boFirstColumn) { strSql += ",\n"; }
        strSql += " \"" + pMapTable->Columns[i].GetColumnName() + "\" ";
        strSql += pMapTable->Columns[i].GetSqlType();

        if (pMapTable->Columns[i].IsNotNull()) { strSql += " NOT NULL"; }

        if (pMapTable->Columns[i].IsNaturalId()) {
            strPrimaryKey = pMapTable->Columns[i].GetColumnName();
        }

        boFirstColumn = FALSE;
    }

    if (!strPrimaryKey.empty()) {
        if (!boFirstColumn) { strSql += ",\n"; }
        strSql += " PRIMARY KEY (\"" + strPrimaryKey + "\")";
    }

    for (u32_t i = 0; i < pMapTable->Columns.size(); i++) {
        if (pMapTable->Columns[i].IsForeignkey()) {
            if (!boFirstColumn) { strSql += ",\n"; }
            strSql += " " + ConsTraintString(pMapTable, pMapTable->Columns[i]);
        }
    }

    strSql += "\n)";

    SqlStatement_p pSqlStatement = GetStatement(strSql);
    pSqlStatement->execute();
    delete pSqlStatement;
}
