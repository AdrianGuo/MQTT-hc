/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ZwDbCtrller.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 7 Nov 2016 20:21:56
 *
 ******************************************************************************/
#include "ZwDbCtrllerItem.hpp"
#include "ZwDbInfo.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbCtrllerItem::ZwDbCtrllerItem() :
    DbItem (ZWCTRLLER),
    HomeId (-1, "HomeId"),
    HexHom (String(), "HexHome"),
    NodeId (-1, "NodeId"),
    ManuId (-1, "ManuId"),
    ProdId (-1, "ProdId"),
    ChipType (-1, "ChipType"),
    ChipVersion (-1, "ChipVersion"),
    LibVersion (String(), "LibVersion") {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwDbCtrllerItem::~ZwDbCtrllerItem() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZwDbCtrllerItem::GetTableName() {
    return ZWCTRLLER;
}
