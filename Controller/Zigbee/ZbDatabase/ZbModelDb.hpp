/*
 * ZbModelDb.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#ifndef ZBMODELDB_HPP_
#define ZBMODELDB_HPP_

#include "typedefs.h"
#include "ZbControllerDb.hpp"
#include "ZbDeviceDb.hpp"
#include "NetDeviceDb.hpp"
#include "DbContext.hpp"

typedef DbPtr<ZbDeviceDb>                   Device_t;
typedef Collection<DbPtr<ZbDeviceDb>>       Devices_t;
typedef Collection<DbPtr<ZbControllerDb>>   Controllers_t;
typedef DbPtr<ZbControllerDb>               Controller_t;
typedef DbPtr<NetDeviceDb>                  NetDevice_t;
typedef Collection<DbPtr<NetDeviceDb>>      NetDevices_t;

class ZbModelDb : public DbContext {
private:
    ZbModelDb(const_char_p cDbName);
public:
    static ZbModelDb* CreateModel(const_char_p cDbName);
    virtual ~ZbModelDb();
    Controllers_t   ZbControllers;
    Devices_t       ZbDevices;
    NetDevices_t    NetDevices;

};

typedef ZbModelDb  ZbModelDb_t;
typedef ZbModelDb* ZbModelDb_p;

#endif /* !ZBMODELDB_HPP_ */
