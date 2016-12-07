/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbDevice.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:22:07
 *
 ******************************************************************************/
#include "ZwDbInfo.hpp"
#include "ZwDbDeviceItem.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbDeviceItem::ZwDbDeviceItem() :
    DbItem (ZWDEVICE),
    NodeId   (-1, "NodeId"),
    Position (-1, "Position"),
    DevType  (-1, "TypeDv"),
    ParId   (-1, "ParId"),
    HomeId  (-1, "HomeId"),
    Basic   (-1, "Basic"),
    Generic  (-1, "Generic"),
    Specific (-1, "Specific"),
    Capability  (-1, "Capability"),
    Version (-1, "Version"),
    ManuId  (-1, "ManuId"),
    ProdId  (-1, "ProdId"),
    RoleType (-1, "RoleType"),
    NodeType (-1, "NodeType"),
    IconType (-1, "IconType"),
    Manufac  (String(), "Manufac"),
    Product  (String(), "Product") {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbDeviceItem::~ZwDbDeviceItem() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDbDeviceItem::GetTableName() {
    return ZWDEVICE;
}
