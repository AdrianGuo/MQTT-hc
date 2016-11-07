/*
 * ValueDwordDb.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#ifndef VALUE_INTDB_HPP_
#define VALUE_INTDB_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "ValueDb.hpp"

class ValueIntDb : public ValueDb {
private:
    int_t m_iValue;
public:
    ValueIntDb(int_t iValue = 0, String strColumnName = "");

    virtual ~ValueIntDb();

    int_t  GetValue() const;
    void_t SetValue(int_t iValue);

    ValueIntDb& operator= (const ValueIntDb& rhs);
    ValueIntDb& operator= (int_t iValue);

    bool_t operator== (const ValueIntDb& rhs) const;
    bool_t operator== (int_t iValue);

    bool_t operator!= (const ValueIntDb& rhs) const;
    bool_t operator!= (int_t iValue);

    bool_t operator>  (const ValueIntDb& rhs) const;
    bool_t operator>  (int_t iValue);

    bool_t operator>= (const ValueIntDb& rhs) const;
    bool_t operator>= (int_t iValue);

    bool_t operator< (const ValueIntDb& rhs) const;
    bool_t operator< (int_t iValue);

    bool_t operator<= (const ValueIntDb& rhs) const;
    bool_t operator<= (int_t iValue);

    ValueIntDb& operator++ ();
    ValueIntDb& operator++ (int_t);

    operator int_t() { return m_iValue; }
    operator int_p() { return &m_iValue; }
};


typedef ValueIntDb  ValueIntDb_t;
typedef ValueIntDb* ValueIntDb_p;

#endif /* !VALUE_INTDB_HPP_ */
