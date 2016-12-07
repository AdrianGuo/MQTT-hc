/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbDevice.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:11:29
 *
 ******************************************************************************/
#ifndef ZWDB_DEVICE_HPP_
#define ZWDB_DEVICE_HPP_

#include "typedefs.h"
#include "DbItem.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "ZwDbConfigModel.hpp"
#include "ZwDbCtrllerItem.hpp"
#include "Collection.hpp"

class ZwDbCmdClassItem;

class ZwDbDeviceItem : public DbItem {
public:
    ZwDbDeviceItem();
    virtual ~ZwDbDeviceItem();

    ValueIntDb_t NodeId;
    ValueIntDb_t Position;
    ValueIntDb_t DevType;
    ValueIntDb_t ParId;
    ValueIntDb_t HomeId;

    ValueIntDb_t Security;
    ValueIntDb_t Basic;
    ValueIntDb_t Generic;
    ValueIntDb_t Specific;
    ValueIntDb_t Capability;
    ValueIntDb_t Version;
    ValueIntDb_t ManuId;
    ValueIntDb_t ProdId;
    ValueIntDb_t RoleType;
    ValueIntDb_t NodeType;
    ValueIntDb_t IconType;

    ValueStrDb_t Manufac;
    ValueStrDb_t Product;


    DbPtr<ZwDbDeviceItem>  Device;
    DbPtr<ZwDbCtrllerItem> Controller;

    Collection<DbPtr<ZwDbDeviceItem>> Devices;
    Collection<DbPtr<ZwDbCmdClassItem>> CmdClasses;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
ZwDbDeviceItem::Table(A& action) {
    Column(action, NodeId);
    Column(action, Position);
    Column(action, DevType);
    Column(action, Basic);
    Column(action, Generic);
    Column(action, Specific);
    Column(action, Capability);
    Belong(action, Controller, HomeId, ValueDb::Constraint::OnDeleteCascade);
    Belong(action, Device, ParId, ValueDb::Constraint::OnDeleteCascade);
    Column(action, ManuId);
    Column(action, ProdId);
    Column(action, RoleType);
    Column(action, NodeType);
    Column(action, IconType);
    Column(action, Manufac);
    Column(action, Product);
    HasMany(action, Devices);
    HasMany(action, CmdClasses);
}

typedef ZwDbDeviceItem  ZwDbDevice_t;
typedef ZwDbDeviceItem* ZwDbDevice_p;

#endif /* !ZWDB_DEVICE_HPP_ */
