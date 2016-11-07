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
 * Last Changed:     Date: 2016-10-09 11:45:00 (Sun, 09 Oct 2016)
 *
 ******************************************************************************/

#ifndef VALUE_DB_HPP_
#define VALUE_DB_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "Value.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ValueDb : public Value {
private:
    String m_strColumnName;
    String m_strSqlType;
    String m_strForeignKeyTable;
    String m_strForeignKeyName;
    u32_t  m_dwFlags;
public:
    enum {
        IsNull      = 0x0000,
        NotNull     = 0x0001,
        InsteadId   = 0x0002,
        NaturalId   = 0x0004,
        ForeignKey  = 0x0008,
        BindNull    = 0x0010,
    };

    typedef enum {
        OnDeleteCascade = 0x0100,
        OnDeleteSetNull = 0x0200,
        OnUpdateCascade = 0x0400,
        OnUpdateSetNull = 0x0800
    } Constraint;

    ValueDb(const String strColumnName = "",
            ValueType_t valueType = ValueType_None,
            const String strForeignKeyTable = "",
            const String strForiegnKeyName = "",
            u32_t dwFlags = BindNull);

    virtual ~ValueDb() {}

    void_t SetColumnName(const String strColumn) {
        m_strColumnName = strColumn;
    }

    void_t SetForeignKeyTable(const String strForeignKeyTable) {
        m_strForeignKeyTable = strForeignKeyTable;
    }

    void_t SetForeignKeyName(const String strForeignKeyNam) {
        m_strForeignKeyName = strForeignKeyNam;
    }

    void_t SetFlag(u32_t dwFlags) { m_dwFlags |= dwFlags; }

    String GetColumnName() const { return m_strColumnName; }
    String GetForeignKeyTable() const { return m_strForeignKeyTable; }
    String GetForeignKeyName() const { return m_strForeignKeyName; }

    u32_t  GetFlag() const { return m_dwFlags; }
    void_t SetSqlType(String strSqlType) { m_strSqlType = strSqlType; }
    String GetSqlType() const { return m_strSqlType; }

    void_t SetNotNull() { m_dwFlags |= NotNull; }
    void_t SetInsteadId() { m_dwFlags |= InsteadId; }
    void_t SetNaturalId() { m_dwFlags |= NaturalId; }
    void_t SetForeignKey() { m_dwFlags |= ForeignKey; }
    void_t SetBindNull() { m_dwFlags |= BindNull; }

    void_t ResetNotNull() { m_dwFlags &= ~NotNull; }
    void_t ResetInsteadId() { m_dwFlags &= ~InsteadId; }
    void_t ResetNaturalId() { m_dwFlags &= ~NaturalId; }
    void_t ResetForeignKey() { m_dwFlags &= ~ForeignKey; }
    void_t ResetBindNull() { m_dwFlags &= ~BindNull; }

    bool_t IsNotNull() const { return (m_dwFlags & NotNull) != 0; }
    bool_t IsInsteadId() const { return (m_dwFlags & InsteadId) != 0; }
    bool_t IsNaturalId() const { return (m_dwFlags & NaturalId) != 0; }
    bool_t IsIdField() const { return (m_dwFlags & (InsteadId | NaturalId)) != 0; }
    bool_t IsForeignkey() const { return (m_dwFlags & ForeignKey) != 0; }
    bool_t IsBindNull() const { return (m_dwFlags & BindNull) != 0; }

    void_t SetOnDeleteCascade() { m_dwFlags |= OnDeleteCascade; }
    void_t SetOnDeleteSetNull() { m_dwFlags |= OnDeleteSetNull; }
    void_t SetOnUpdateCascade() { m_dwFlags |= OnUpdateCascade; }
    void_t SetOnUpdateSetNull() { m_dwFlags |= OnUpdateSetNull; }

    void_t ResetOnDeleteCascade() { m_dwFlags &= ~OnDeleteCascade; }
    void_t ResetOnDeleteSetNull() { m_dwFlags &= ~OnDeleteSetNull; }
    void_t ResetOnUpdateCascade() { m_dwFlags &= ~OnUpdateCascade; }
    void_t ResetOnUpdateSetNull() { m_dwFlags &= ~OnUpdateSetNull; }

    bool_t IsOnDeleteCascade() const { return (m_dwFlags & OnDeleteCascade) != 0; }
    bool_t IsOnDeleteSetNull() const { return (m_dwFlags & OnDeleteSetNull) != 0; }
    bool_t IsOnUpdateCascade() const { return (m_dwFlags & OnUpdateCascade) != 0; }
    bool_t IsOnUpdateSetNull() const { return (m_dwFlags & OnUpdateSetNull) != 0; }
};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueDb::ValueDb(
    const String strColumnName,
    ValueType_t valueType,
    const String strForeignKeyTable,
    const String strForeignKeyName,
    u32_t dwFlags
) : Value (valueType),
    m_strColumnName (strColumnName),
    m_strForeignKeyTable (strForeignKeyTable),
    m_strForeignKeyName (strForeignKeyName),
    m_dwFlags (dwFlags) {

    if (valueType == ValueType_Integer) {
        m_strSqlType = "INTEGER";
    } else if (valueType == ValueType_String) {
        m_strSqlType = "TEXT";
    } else if (valueType == ValueType_Decimal) {
        m_strSqlType = "REAL";
    }
}

typedef ValueDb  ValueDb_t;
typedef ValueDb* ValueDb_p;

#endif /* !VALUE_DB_HPP_ */
