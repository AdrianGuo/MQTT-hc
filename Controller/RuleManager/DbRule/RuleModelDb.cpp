/*
 * RuleModelDb.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: phind
 */

#include <DbRule/RuleModelDb.hpp>
#include "DbContextImpl.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleModelDb::RuleModelDb(const_char_p cDbName) :
		DbContext(cDbName) {
	MapDbTable<RuleDb>(RuleDb::GetTableName());
	CreateTables();

	Rules = Find<RuleDb>();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleModelDb::~RuleModelDb() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleModelDb_p RuleModelDb::CreateModel(const_char_p cDbName) {
	return new RuleModelDb(cDbName);
}
