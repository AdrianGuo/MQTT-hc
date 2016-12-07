/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbCmdClass.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:24:12
 *
 ******************************************************************************/
#ifndef ZWDB_CMDCLASS_HPP_
#define ZWDB_CMDCLASS_HPP_

#include "typedefs.h"
#include "DbItem.hpp"
#include "ValueStrDb.hpp"
#include "ValueIntDb.hpp"
#include "ZwDbDeviceItem.hpp"

class ZwDbCmdClassItem : public DbItem {
public:
    ZwDbCmdClassItem();
    virtual ~ZwDbCmdClassItem();

    ValueIntDb_t CmdId;
    ValueStrDb_t CmdHex;
    ValueStrDb_t CmdName;
    ValueIntDb_t DevId;

    DbPtr<ZwDbDeviceItem> Device;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
ZwDbCmdClassItem::Table(A& action) {
    Column(action, CmdId);
    Column(action, CmdHex);
    Column(action, CmdName);
    Belong(action, Device, DevId, ValueDb::Constraint::OnDeleteCascade);
}

typedef ZwDbCmdClassItem  ZwDbCmdClass_t;
typedef ZwDbCmdClassItem* ZwDbCmdClass_p;

#endif /* !ZWDB_CMDCLASS_HPP_ */
