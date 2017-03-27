#ifndef DBMODELDB_HPP_
#define DBMODELDB_HPP_

#include "Typedefs.h"
#include "DbConfigModel.hpp"
#include "DbRoomItem.hpp"
#include "DbFloorItem.hpp"
#include "DbDeviceItem.hpp"

typedef enum {
    Zwave = 0,
    Zigbee,
    Wifi,
    Bluetooth
} NetWk;

typedef DbPtr<DbRoomItem>   DbRoom;
typedef DbPtr<DbFloorItem>  DbFloor;
typedef DbPtr<DbDeviceItem> DbDevice;

typedef Collection<DbRoom>    DbRooms;
typedef Collection<DbFloor>   DbFloors;
typedef Collection<DbDevice>  DbDevices;

class DbModel: public DbContext {
private:
    static DbModel* m_pInstance;
	DbModel(const_char_p cDbName);
public:
	static DbModel* CreateModel(const_char_p cDbName);
	virtual ~DbModel();

	DbRooms   Rooms;
	DbFloors  Floors;
	DbDevices Devices;
};

typedef DbModel  DbModel_t;
typedef DbModel* DbModel_p;

#endif /* !DBMODELDB_HPP_ */
