/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDb.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-09 11:45:00 (Sun, 09 Oct 2016)
 *
 ******************************************************************************/

#ifndef VALUE_DB_HPP_
#define VALUE_DB_HPP_

#include "../Typedefs.h"
#include "../LogPlus.hpp"
#include "ValueDbCore.hpp"

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class ValueDb : public ValueDbCore {
protected:
    u32_t  m_dwFlags;
    String m_strColumnName;
    String m_strSqlType;
    String m_strForeignKeyTable;
    String m_strForeignKeyName;

    void_t Swap(ValueDb& other);
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

    typedef enum {
        Equal = 0,
        NotEqual = 1,
        Less = 2,
        Greater = 3,
        LessAndEqual = 4,
        GreaterAndEqual = 5
    } Compare;

    ValueDb(const String strColumnName = String(),
            Value::Type_t valueType = Value::Type_t::type_none,
            const_char_p pName = "ValueDb",
            const String strForeignKeyTable = String(),
            const String strForeignKeyName  = String(),
            u32_t dwFlags = BindNull);

    ValueDb(const ValueDb& copied);

    virtual ~ValueDb();
    virtual void_t SetValueDefault();

    ValueDb& operator= (const ValueDb& copied);

    void_t SetFlag(u32_t dwFlags);
    void_t SetSqlType(String strSqlType);
    void_t SetColumnName(const String strColumnName);
    void_t SetForeignKeyTable(const String strForeignKeyTable);
    void_t SetForeignKeyName (const String strForeignKeyName);

    u32_t  GetFlag()            const;
    String GetSqlType()         const;
    String GetColumnName()      const;
    String GetForeignKeyName () const;
    String GetForeignKeyTable() const;

    void_t SetNotNull   ();
    void_t SetInsteadId ();
    void_t SetNaturalId ();
    void_t SetForeignKey();
    void_t SetBindNull  ();

    void_t ResetNotNull   ();
    void_t ResetInsteadId ();
    void_t ResetNaturalId ();
    void_t ResetForeignKey();
    void_t ResetBindNull  ();

    bool_t IsNotNull()    const;
    bool_t IsInsteadId()  const;
    bool_t IsNaturalId()  const;
    bool_t IsIdField()    const;
    bool_t IsForeignkey() const;
    bool_t IsBindNull()   const;

    void_t SetOnDeleteCascade();
    void_t SetOnDeleteSetNull();
    void_t SetOnUpdateCascade();
    void_t SetOnUpdateSetNull();

    void_t ResetOnDeleteCascade();
    void_t ResetOnDeleteSetNull();
    void_t ResetOnUpdateCascade();
    void_t ResetOnUpdateSetNull();

    bool_t IsOnDeleteCascade() const;
    bool_t IsOnDeleteSetNull() const;
    bool_t IsOnUpdateCascade() const;
    bool_t IsOnUpdateSetNull() const;
};

typedef ValueDb  ValueDb_t;
typedef ValueDb* ValueDb_p;

#endif /* !VALUE_DB_HPP_ */
