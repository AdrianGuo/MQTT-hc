/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueDbCore.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 15 Jan 2017 21:35:37
 *
 ******************************************************************************/
#ifndef VALUE_DBCORE_HPP_
#define VALUE_DBCORE_HPP_

#include "../Value/Value.hpp"

class ValueDbCore : public Value {
private:
    bool_t m_boChanged;

protected:
    void_t Swap(ValueDbCore& other);

public:
    ValueDbCore(Type_t valueType = type_none, const_char_p pName = "ValueDbCore");
    ValueDbCore(const ValueDbCore& copied);

    virtual ~ValueDbCore();
    ValueDbCore& operator= (const ValueDbCore& copied);

    virtual void_t SetChange();
    virtual void_t ResetChange();
    virtual bool_t IsChanged() const;
};

typedef ValueDbCore  ValueDbCore_t;
typedef ValueDbCore* ValueDbCore_p;

#endif /* !VALUE_DBCORE_HPP_ */
