/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueIntDb.hpp
 *
 * Author: TrungTQb
 *
 * Last Changed By:  Author: TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-07 16:10:00 (Fri, 07 Oct 2016)
 *
 ******************************************************************************/
#ifndef VALUE_INTDB_HPP_
#define VALUE_INTDB_HPP_

#include "Libraries/typedefs.h"
#include "Libraries/LibDb/ValueDb.hpp"

class ValueIntDb : public ValueDb {
private:
    int_t m_iValue;
protected:
    void_t Swap(ValueIntDb& other);
public:
    ValueIntDb(int_t iValue = -1,
               String strColumnName = String());
    ValueIntDb(const ValueIntDb& copied);

    virtual ~ValueIntDb();
    virtual void_t SetValueDefault();

    int_t  GetValue() const;
    void_t SetValue(int_t iValue);

    ValueIntDb& operator= (const ValueIntDb& copied);
    ValueIntDb& operator= (int_t iValue);
    ValueIntDb& operator= (u8_t  byValue);
    ValueIntDb& operator= (u16_t wValue);
    ValueIntDb& operator= (u32_t dwValue);

    bool_t operator== (const ValueIntDb& rhs) const;
    bool_t operator!= (const ValueIntDb& rhs) const;
    bool_t operator>  (const ValueIntDb& rhs) const;
    bool_t operator>= (const ValueIntDb& rhs) const;
    bool_t operator<  (const ValueIntDb& rhs) const;
    bool_t operator<= (const ValueIntDb& rhs) const;

    bool_t operator== (const int_t iValue) const;
    bool_t operator!= (const int_t iValue) const;
    bool_t operator>  (const int_t iValue) const;
    bool_t operator>= (const int_t iValue) const;
    bool_t operator<  (const int_t iValue) const;
    bool_t operator<= (const int_t iValue) const;

    ValueIntDb& operator++ ();
    ValueIntDb& operator++ (int_t);

    operator int_t() const { return m_iValue; }
    operator int_p() { return &m_iValue; }
};


typedef ValueIntDb  ValueIntDb_t;
typedef ValueIntDb* ValueIntDb_p;

#endif /* !VALUE_INTDB_HPP_ */
