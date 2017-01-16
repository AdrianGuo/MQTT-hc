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
 * Last Changed:     Date: 2016-10-13 09:40:00 (Thu, 13 Oct 2016)
 *
 ******************************************************************************/

#ifndef SQLITE3_BACKEND_HPP_
#define SQLITE3_BACKEND_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "Database.hpp"
#include "SqlStatement.hpp"

class Sqlite3Statement : public SqlStatement {
private:
    Database_p      m_pDatabase;
    sqlite3_stmt*   m_pStmt;
    String          m_strSql;
public:
    Sqlite3Statement(Database_p pDatabase, const String& strSql);
    virtual ~Sqlite3Statement();

    virtual int_t bind(int_t column, i8_t  value);
    virtual int_t bind(int_t column, i16_t value);
    virtual int_t bind(int_t column, i32_t value);
    virtual int_t bind(int_t column, i64_t value);
    virtual int_t bind(int_t column, flo_t value);
    virtual int_t bind(int_t column, dob_t value);
    virtual int_t bind(int_t column, const_char_p value);
    virtual int_t bind(int_t column, void_p value, int_t n);
    virtual int_t bind(int_t column, const String& value);
    virtual int_t bind(int_t column);

    virtual int_t bind(int_t column, ValueIntDb value);
    virtual int_t bind(int_t column, ValueStrDb value);

    virtual int_t bind(const_char_p name, i8_t  value);
    virtual int_t bind(const_char_p name, i16_t value);
    virtual int_t bind(const_char_p name, i32_t value);
    virtual int_t bind(const_char_p name, i64_t value);
    virtual int_t bind(const_char_p name, flo_t value);
    virtual int_t bind(const_char_p name, dob_t value);
    virtual int_t bind(const_char_p name, const_char_p value);
    virtual int_t bind(const_char_p name, void_p value, int_t n);
    virtual int_t bind(const_char_p name, const String& value);
    virtual int_t bind(const_char_p name);

    virtual bool_t get(int_t column, i8_p  value);
    virtual bool_t get(int_t column, i16_p value);
    virtual bool_t get(int_t column, i32_p value);
    virtual bool_t get(int_t column, i64_p value);
    virtual bool_t get(int_t column, flo_p value);
    virtual bool_t get(int_t column, dob_p value);
    virtual bool_t get(int_t column, String* value);
    virtual bool_t get(int_t column, void_p value);

    virtual bool_t get(int_t column, ValueIntDb_p value);

    virtual i64_t  insertId();

    virtual void_t reset();
    virtual void_t ubind();
    virtual void_t init();

    virtual int_t count();
    virtual int_t execute();
};

typedef Sqlite3Statement  Sqlite3Statement_t;
typedef Sqlite3Statement* Sqlite3Statement_p;

#endif /* !SQLITE3_BACKEND_HPP_ */
