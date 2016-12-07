/*
 * DbDeviceItem.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#ifndef DBDEVICEITEM_HPP_
#define DBDEVICEITEM_HPP_

#include "typedefs.h"
#include "String.hpp"
#include "ValueIntDb.hpp"
#include "DbRoomItem.hpp"

class DbDeviceItem: public DbItem {
public:
	DbDeviceItem();
	virtual ~DbDeviceItem();

	ValueIntDb_t DevId;
	ValueIntDb_t Position;
	ValueIntDb_t TypeDev;
	ValueIntDb_t TypeNet;
	ValueIntDb_t RoomId;
	ValueStrDb_t MacId;
	ValueIntDb_t ParId;

	DbPtr<DbRoomItem> Room;
	DbPtr<DbDeviceItem> Device;

	Collection<DbPtr<DbDeviceItem>> Devices;

	static String GetTableName();
	template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
DbDeviceItem::Table(A& action) {
	Column(action, DevId,    ValueDb::NotNull);
	Column(action, Position, ValueDb::NotNull);
	Column(action, TypeDev,  ValueDb::NotNull);
	Column(action, TypeNet,  ValueDb::NotNull);
	Column(action, MacId,    ValueDb::NotNull);
	Belong(action, Room,   RoomId, ValueDb::Constraint::OnDeleteSetNull);
	Belong(action, Device, ParId,  ValueDb::Constraint::OnDeleteCascade);
	HasMany(action, Devices);
}

typedef DbDeviceItem  DbDeviceItem_t;
typedef DbDeviceItem* DbDeviceItem_p;

#endif /* !DBDEVICEITEM_HPP_ */
