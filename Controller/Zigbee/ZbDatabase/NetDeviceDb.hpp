/*
 * NetDeviceDb.hpp
 *
 *  Created on: Dec 20, 2016
 *      Author: taho
 */

#ifndef CONTROLLER_ZIGBEE_ZBDATABASE_NETDEVICEDB_HPP_
#define CONTROLLER_ZIGBEE_ZBDATABASE_NETDEVICEDB_HPP_

#include "Typedefs.h"
#include "DbItem.hpp"
#include "DbPtr.hpp"
#include "Config.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"

class NetDeviceDb {
public:
    NetDeviceDb();
    virtual ~NetDeviceDb();

    ValueIntDb_t DeviceID;
    ValueStrDb_t MAC;
    ValueIntDb_t Type;
    ValueIntDb_t NetID;

    static String GetTableName();
    template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
NetDeviceDb::Table(
    A& action
) {
    Column(action, DeviceID);
    Column(action, MAC);
    Column(action, Type);
    Column(action, NetID);
}

typedef NetDeviceDb  NetDeviceDb_t;
typedef NetDeviceDb* NetDeviceDb_p;

#endif /* CONTROLLER_ZIGBEE_ZBDATABASE_NETDEVICEDB_HPP_ */
