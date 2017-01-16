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
 * Last Changed:     Date: 2016-11-02 22:35:00 (Tue, 02 Oct 2016)
 *
 ******************************************************************************/

#ifndef CONFIG_IMPL_HPP_
#define CONFIG_IMPL_HPP_

#include "typedefs.h"
#include "ValueIntDb.hpp"
#include "String.hpp"
#include "Config.hpp"

template<class C>
struct ConfigTable : public ConfigDefault {
    typedef ValueIntDb IdType;
    static String InsteadIdField() { return "ID"; }
};

#endif /* !CONFIG_IMPL_HPP_ */
