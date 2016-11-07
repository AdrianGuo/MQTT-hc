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
 * Last Changed:     Date: 2016-10-18 10:30:00 (Tue, 18 Oct 2016)
 *
 ******************************************************************************/

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "Value.hpp"
#include "ValueIntDb.hpp"

struct ConfigDefault {
    typedef ValueIntDb IdType;
    static String InsteadIdField() { return "ID"; }
};

#endif /* !CONFIG_HPP_ */
