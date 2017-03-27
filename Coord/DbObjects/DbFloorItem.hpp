/*
 * DbFloorItem.hpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#ifndef DBFLOORITEM_HPP_
#define DBFLOORITEM_HPP_

#include "Typedefs.h"
#include "DbItem.hpp"
#include "String.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"
#include "Collection.hpp"

class DbRoomItem;

class DbFloorItem: public DbItem {
public:
	DbFloorItem();
	virtual ~DbFloorItem();

	ValueIntDb_t FloorId;
	ValueStrDb_t Name;

	Collection<DbPtr<DbRoomItem>> Rooms;

	static String GetTableName();
	template<class A> void_t Table(A& action);
};

template<class A>
inline void_t
DbFloorItem::Table(A& action) {
	Id(action, FloorId);
	Column(action, Name, ValueDb::NotNull);
	HasMany(action, Rooms);
}

typedef DbFloorItem DbFloorItem_t;
typedef DbFloorItem* DbFloorItem_p;

#endif /* !DBFLOORITEM_HPP_ */
