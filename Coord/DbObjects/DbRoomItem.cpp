/*
 * DbRoomItem.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#include "String.hpp"
#include "DbInfo.hpp"
#include "DbRoomItem.hpp"

/**
 * @func   DbRoomItem
 * @brief  None
 * @param  None
 * @retval None
 */
DbRoomItem::DbRoomItem() :
    DbItem(DB_NAME_TABLE_ROOM),
    RoomId(-1, "RoomId"),
    Name(String(), "Name"),
    FloorId(-1, "FloorId") {
}

/**
 * @func   ~DbRoomItem
 * @brief  None
 * @param  None
 * @retval None
 */
DbRoomItem::~DbRoomItem() {
}

/**
 * @func   GetTableName
 * @brief  None
 * @param  None
 * @retval None
 */
String
DbRoomItem::GetTableName() {
	return DB_NAME_TABLE_ROOM;
}

