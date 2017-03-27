/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbCtrller.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:09:35
 *
 ******************************************************************************/
#ifndef ZWDB_CTRLLER_HPP_
#define ZWDB_CTRLLER_HPP_

#include "Typedefs.h"
#include "DbItem.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "Collection.hpp"
#include "ZwDbConfigModel.hpp"

class ZwDbDeviceItem;

class ZwDbCtrllerItem : public DbItem {
public:
    ZwDbCtrllerItem();
    virtual ~ZwDbCtrllerItem();

    ValueIntDb_t HomeId;
    ValueStrDb_t HexHom;
    ValueIntDb_t NodeId;
    ValueIntDb_t ManuId;
    ValueIntDb_t ProdId;
    ValueIntDb_t ChipType;
    ValueIntDb_t ChipVersion;
    ValueStrDb_t LibVersion;

    Collection<DbPtr<ZwDbDeviceItem>> Devices;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
ZwDbCtrllerItem::Table(A& action) {
    Column(action, HomeId);
    Column(action, HexHom);
    Column(action, NodeId);
    Column(action, ManuId);
    Column(action, ProdId);
    Column(action, ChipType);
    Column(action, ChipVersion);
    Column(action, LibVersion);
    HasMany(action, Devices);
}

typedef ZwDbCtrllerItem  ZwDbCtrller_t;
typedef ZwDbCtrllerItem* ZwDbCtrller_p;

#endif /* !ZWDB_CTRLLER_HPP_ */
