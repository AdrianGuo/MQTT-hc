/*
 * DbDeviceItem.cpp
 *
 *  Created on: Nov 3, 2016
 *      Author: phind
 */

#include "String.hpp"
#include "DbInfo.hpp"
#include "DbDeviceItem.hpp"

/**
 * @func   DbDeviceItem
 * @brief  None
 * @param  None
 * @retval None
 */
DbDeviceItem::DbDeviceItem() :
    DbItem(DB_NAME_TABLE_DEVICE),
    DevId   (-1, "DeviceId"),
    Position(-1, "Position"),
    TypeDev (-1, "TypeDev"),
    TypeNet (-1, "TypeNet"),
    RoomId  (-1, "RoomId"),
    MacId   (String(), "Mac"),
    ParId   (-1, "ParId") {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
DbDeviceItem::~DbDeviceItem() {
}

/**
 * @func   GetTableName
 * @brief  None
 * @param  None
 * @retval None
 */
String DbDeviceItem::GetTableName() {
	return DB_NAME_TABLE_DEVICE;
}

