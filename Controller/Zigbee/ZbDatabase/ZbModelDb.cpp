/*
 * ZwModelDb.cpp
 *
 *  Created on: 22 Sep 2016
 *      Author: TrungTQ
 */

#include "DbContextImpl.hpp"
#include <ZbModelDb.hpp>


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbModelDb::ZbModelDb(
    const_char_p cDbName
) : DbContext(cDbName) {
    MapDbTable<ZbControllerDb>(ZbControllerDb::GetTableName());
    MapDbTable<ZbDeviceDb>(ZbDeviceDb::GetTableName());
    CreateTables();
    ZbControllers = Find<ZbControllerDb>();
    ZbDevices = Find<ZbDeviceDb>();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbModelDb::~ZbModelDb() { }

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbModelDb_p
ZbModelDb::CreateModel(
    const_char_p cDbName
) {
    return new ZbModelDb(cDbName);
}
