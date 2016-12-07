/*
 * DbFloorItem.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#include "DbFloorItem.hpp"
#include "DbInfo.hpp"
#include "String.hpp"

/**
 * @func   DbFloorItem
 * @brief  None
 * @param  None
 * @retval None
 */
DbFloorItem::DbFloorItem() :
    DbItem(DB_NAME_TABLE_FLOOR),
    FloorId(-1, "FloorId"),
    Name(String(), "Name") {
}

/**
 * @func   ~DbFloorItem
 * @brief  None
 * @param  None
 * @retval None
 */
DbFloorItem::~DbFloorItem() {
}

/**
 * @func   GetTableName
 * @brief  None
 * @param  None
 * @retval None
 */
String
DbFloorItem::GetTableName() {
	return DB_NAME_TABLE_FLOOR;
}

