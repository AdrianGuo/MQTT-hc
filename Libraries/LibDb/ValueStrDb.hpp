/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueStrDb.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-29 16:35:00 (Sat, 29 Oct 2016)
 *
 ******************************************************************************/

#ifndef VALUE_STRDB_HPP_
#define VALUE_STRDB_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "ValueDb.hpp"

class ValueStrDb : public ValueDb {
private:
    String m_strValue;
public:
    ValueStrDb(String strValue = "",
               String strColumnName = "");

    virtual ~ValueStrDb();

    String GetValue() const;
    void_t SetValue(String strValue);
    virtual void_t SetValueDefault() { m_strValue = ""; }

    ValueStrDb& operator= (const ValueStrDb& rhs);
    ValueStrDb& operator= (const String strValue);
    ValueStrDb& operator= (const_char_p rhs);

    bool_t operator== (const ValueStrDb& rhs) const;
    bool_t operator== (const String strValue) const;

    bool_t operator!= (const ValueStrDb& rhs) const;
    bool_t operator!= (const String strValue) const;

    bool_t operator>  (const ValueStrDb& rhs) const;
    bool_t operator>  (const String strValue) const;

    bool_t operator>= (const ValueStrDb& rhs) const;
    bool_t operator>= (const String strValue) const;

    bool_t operator< (const ValueStrDb& rhs) const;
    bool_t operator< (const String strValue) const;

    bool_t operator<= (const ValueStrDb& rhs) const;
    bool_t operator<= (const String strValue) const;

    operator String()  { return m_strValue; }
    operator String*() { return &m_strValue; }
};

typedef ValueStrDb  ValueStrDb_t;
typedef ValueStrDb* valueStrDb_p;

#endif /* !VALUESTRDB_HPP_ */
