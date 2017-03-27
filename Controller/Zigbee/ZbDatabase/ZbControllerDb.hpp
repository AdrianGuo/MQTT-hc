/*
 * Controller.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#ifndef ZBCTROLLERDB_HPP_
#define ZBCTROLLERDB_HPP_

#include "Typedefs.h"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "ConfigModel.hpp"
#include "Collection.hpp"
#include "ValueStrDb.hpp"

class ZbDeviceDb;

class ZbControllerDb {
public:
    ZbControllerDb();
    virtual ~ZbControllerDb();

    ValueIntDb_t ControllerID;
    ValueIntDb_t PanID;
    ValueStrDb_t ExPanID;
    ValueIntDb_t Channel;
    ValueIntDb_t Firmware;

    Collection<DbPtr<ZbDeviceDb>> Device;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
ZbControllerDb::Table(
    A& action
) {
    Id(action, ControllerID);
    Column(action, PanID);
    Column(action, ExPanID);
    Column(action, Channel);
    Column(action, Firmware);
    HasMany(action, Device);
}

typedef ZbControllerDb  ZbCtrllerDb_t;
typedef ZbControllerDb* ZbCtrllerDb_p;

#endif /* !ZBCTROLLERDB_HPP_ */
