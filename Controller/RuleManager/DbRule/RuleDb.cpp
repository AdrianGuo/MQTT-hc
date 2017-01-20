/*
 * RuleDb.cpp
 *
 *  Created on: Jan 17, 2017
 *      Author: phind
 */

#include <DbRule/RuleDb.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDb::RuleDb() :
		RuleID(0, "RuleID"), RuleData("", "RuleData") {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDb::~RuleDb() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String RuleDb::GetTableName() {
	return "RULE";
}

