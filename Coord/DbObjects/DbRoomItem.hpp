/*
 * DbRoomItem.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#ifndef DBROOMITEM_HPP_
#define DBROOMITEM_HPP_

#include "Typedefs.h"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "DbFloorItem.hpp"
#include "Collection.hpp"

class DbDeviceItem;

class DbRoomItem: public DbItem {
public:
	DbRoomItem();
	virtual ~DbRoomItem();

	ValueIntDb_t RoomId;
	ValueStrDb_t Name;
	ValueIntDb_t FloorId;

	Collection<DbPtr<DbDeviceItem>> Devices;
	DbPtr<DbFloorItem> Floor;

	static String GetTableName();
	template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
DbRoomItem::Table(A& action) {
	Id(action, RoomId);
	Column(action, Name, ValueDb::NotNull);
	Belong(action, Floor, FloorId, ValueDb::Constraint::OnDeleteCascade);
	HasMany(action, Devices);
}

typedef DbRoomItem  DbRoomItem_t;
typedef DbRoomItem* DbRoomItem_p;

#endif /* !DBROOMITEM_HPP_ */
