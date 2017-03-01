/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: SqlStatement.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-13 09:15:00 (Thu, 13 Oct 2016)
 *
 ******************************************************************************/

#ifndef SQLSTATEMENT_HPP_
#define SQLSTATEMENT_HPP_

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/ValueIntDb.hpp"
#include "Libraries/LibDb/ValueStrDb.hpp"

class SqlStatement {
public:
    SqlStatement();
    virtual ~SqlStatement();

    virtual int_t bind(int_t column, i8_t  value) = 0;
    virtual int_t bind(int_t column, i16_t value) = 0;
    virtual int_t bind(int_t column, i32_t value) = 0;
    virtual int_t bind(int_t column, i64_t value) = 0;
    virtual int_t bind(int_t column, flo_t value) = 0;
    virtual int_t bind(int_t column, dob_t value) = 0;
    virtual int_t bind(int_t column, const_char_p value) = 0;
    virtual int_t bind(int_t column, void_p value, int_t n) = 0;
    virtual int_t bind(int_t column, const String& value) = 0;
    virtual int_t bind(int_t column) = 0;

    virtual int_t bind(int_t column, ValueIntDb value) = 0;
    virtual int_t bind(int_t column, ValueStrDb value) = 0;

    virtual int_t bind(const_char_p name, i8_t  value) = 0;
    virtual int_t bind(const_char_p name, i16_t value) = 0;
    virtual int_t bind(const_char_p name, i32_t value) = 0;
    virtual int_t bind(const_char_p name, i64_t value) = 0;
    virtual int_t bind(const_char_p name, flo_t value) = 0;
    virtual int_t bind(const_char_p name, dob_t value) = 0;
    virtual int_t bind(const_char_p name, const_char_p value) = 0;
    virtual int_t bind(const_char_p name, void_p value, int_t n) = 0;
    virtual int_t bind(const_char_p name, const String& value) = 0;
    virtual int_t bind(const_char_p name) = 0;

    virtual bool_t get(int_t column, i8_p  value) = 0;
    virtual bool_t get(int_t column, i16_p value) = 0;
    virtual bool_t get(int_t column, i32_p value) = 0;
    virtual bool_t get(int_t column, i64_p value) = 0;
    virtual bool_t get(int_t column, flo_p value) = 0;
    virtual bool_t get(int_t column, dob_p value) = 0;
    virtual bool_t get(int_t column, String* value) = 0;
    virtual bool_t get(int_t column, void_p value) = 0;

    virtual bool_t get(int_t column, ValueIntDb_p value) = 0;

    virtual i64_t  insertId() = 0;

    virtual void_t reset() = 0;
    virtual void_t ubind() = 0;
    virtual void_t init() = 0;

    virtual int_t count() = 0;
    virtual int_t execute() = 0;

    virtual String sql() const = 0;

    bool_t istriggered() const;
    void_t triggered(bool_t boTriggered = TRUE);
    void_t done();
};

typedef SqlStatement  SqlStatement_t;
typedef SqlStatement* SqlStatement_p;

/**
 * @func   SqlStatement
 * @brief  None
 * @param  None
 * @retval None
 */
inline
SqlStatement::SqlStatement() {

}

/**
 * @func   ~SqlStatement
 * @brief  None
 * @param  None
 * @retval None
 */
inline
SqlStatement::~SqlStatement() {

}

/**
 * @func   done
 * @brief  None
 * @param  None
 * @retval None
 */
inline void_t
SqlStatement::done() {
    reset();
    ubind();
}

#endif /* SQLSTATEMENT_HPP_ */
