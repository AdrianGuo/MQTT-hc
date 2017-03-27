/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DbChecker.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 6 Mar 2017 16:59:40
 *
 ******************************************************************************/
#ifndef DBCHECKER_HPP_
#define DBCHECKER_HPP_

#include "../Typedefs.h"

template<class Class, typename Value>
struct Check {
    static bool_t Compare(Class& obj, Value value, ValueDb::Compare Cp = ValueDb::Compare::Equal) { return FALSE; }
};

#endif /* !DBCHECKER_HPP_ */
