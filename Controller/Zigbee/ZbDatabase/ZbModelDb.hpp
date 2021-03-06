/*
 * ZbModelDb.hpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#ifndef ZBMODELDB_HPP_
#define ZBMODELDB_HPP_

#include "Typedefs.h"
#include "ZbControllerDb.hpp"
#include "ZbDeviceDb.hpp"
#include "NetDeviceDb.hpp"
#include "BackupInfo.hpp"
#include "DbContext.hpp"

typedef DbPtr<ZbDeviceDb>                   Device_t;
typedef Collection<DbPtr<ZbDeviceDb>>       Devices_t;
typedef DbPtr<ZbControllerDb>               Controller_t;
typedef Collection<DbPtr<ZbControllerDb>>   Controllers_t;
typedef DbPtr<NetDeviceDb>                  NetDevice_t;
typedef Collection<DbPtr<NetDeviceDb>>      NetDevices_t;
typedef DbPtr<BackupInfoDb>                 BackupDev_t;
typedef Collection<DbPtr<BackupInfoDb>>     BackupDevs_t;

typedef Aggregation<DbPtr<ZbDeviceDb>>       ADevices_t;
typedef Aggregation<DbPtr<ZbControllerDb>>   AControllers_t;

class ZbModelDb : public DbContext {
private:
    ZbModelDb(const_char_p cDbName);
public:
    static ZbModelDb* CreateModel(const_char_p cDbName);
    virtual ~ZbModelDb();
    Controllers_t   ZbControllers;
    Devices_t       ZbDevices;
    NetDevices_t    NetDevices;
    BackupDevs_t    BackupDevs;
};

typedef ZbModelDb  ZbModelDb_t;
typedef ZbModelDb* ZbModelDb_p;

#endif /* !ZBMODELDB_HPP_ */
