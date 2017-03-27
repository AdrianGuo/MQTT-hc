/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbModel.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:13:06
 *
 ******************************************************************************/
#ifndef ZWDB_MODEL_HPP_
#define ZWDB_MODEL_HPP_

#include "Typedefs.h"
#include "ZwDbCmdClassItem.hpp"
#include "ZwDbConfigModel.hpp"
#include "ZwDbCtrllerItem.hpp"
#include "ZwDbDeviceItem.hpp"

typedef DbPtr<ZwDbDeviceItem>   ZwDbDevice;
typedef DbPtr<ZwDbCtrllerItem>  ZwDbController;
typedef DbPtr<ZwDbCmdClassItem> ZwDbCommandClass;

typedef Collection<ZwDbDevice>       ZwDbDevices;
typedef Collection<ZwDbController>   ZwDbControllers;
typedef Collection<ZwDbCommandClass> ZwDbCommandClasses;

class ZwDbModel: public DbContext {
private:
    static ZwDbModel* m_pInstance;
    ZwDbModel(const_char_p cDbName);
public:
    static ZwDbModel* CreateModel(const_char_p cDbName);
    virtual ~ZwDbModel();

    ZwDbDevices Devices;
    ZwDbController Controller;
    ZwDbCommandClasses CmdClasses;
};

typedef ZwDbModel  ZwDbModel_t;
typedef ZwDbModel* ZwDbModel_p;


#endif /* !ZWDB_MODEL_HPP_ */
