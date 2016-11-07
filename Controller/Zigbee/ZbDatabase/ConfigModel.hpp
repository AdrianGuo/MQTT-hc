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
 * Last Changed:     Date: 2016-11-03 16:40:00 (Thu, 03 Oct 2016)
 *
 ******************************************************************************/

#ifndef CONFIG_MODEL_HPP_
#define CONFIG_MODEL_HPP_

#include "ConfigImpl.hpp"

class ZbControllerDb;
class ZbDeviceDb;

template<>
struct ConfigTable<ZbDeviceDb> : public ConfigDefault {
    static  String InsteadIdField() { return String(); } // Use this if use [Id]
};

template<>
struct ConfigTable<ZbControllerDb> : public ConfigDefault {
    static  String InsteadIdField() { return String(); } // Use this if use [Id]
};

#endif /* !CONFIG_MODEL_HPP_ */
