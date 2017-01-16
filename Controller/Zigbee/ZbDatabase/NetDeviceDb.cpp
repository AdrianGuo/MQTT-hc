/*
 * NetDeviceDb.cpp
 *
 *  Created on: Dec 20, 2016
 *      Author: taho
 */

#include <NetDeviceDb.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
NetDeviceDb::NetDeviceDb() :
    DeviceID (0, "DeviceID"),
    MAC ("", "MAC"),
    Type (0, "Type"),
    NetID (0, "NetID") {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
NetDeviceDb::~NetDeviceDb() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
NetDeviceDb::GetTableName() {
    return "NETDEVICE";
}
