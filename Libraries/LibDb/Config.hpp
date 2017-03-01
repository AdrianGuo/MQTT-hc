/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Config.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-18 10:30:00 (Tue, 18 Oct 2016)
 *
 ******************************************************************************/

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include "Libraries/typedefs.h"
#include "Libraries/Value/Value.hpp"
#include "Libraries/LibDb/ValueIntDb.hpp"

struct ConfigDefault {
    typedef ValueIntDb IdType;
    static String InsteadIdField() { return "ID"; }
};

#endif /* !CONFIG_HPP_ */
