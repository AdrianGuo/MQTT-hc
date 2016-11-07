/*
 * ZwCtrllerDb.cpp
 *
 *  Created on: 23 Sep 2016
 *      Author: TrungTQ
 */

#include <ZbControllerDb.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbControllerDb::ZbControllerDb() :
    ControllerID (0, "ControllerID"),
    PanID (0, "PanID"),
    ExPanID ("", "ExPanID"),
    Channel (0, "Channel"),
    Firmware (0, "Firmware") {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbControllerDb::~ZbControllerDb() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
ZbControllerDb::GetTableName() {
    return "ZBCONTROLLER";
}
