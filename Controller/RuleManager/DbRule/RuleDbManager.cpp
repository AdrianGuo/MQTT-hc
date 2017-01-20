/*
 * RuleDbManager.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: phind
 */

#include "DbRule/RuleDbManager.hpp"
#include "json.h"
#include <sqlite3.h>
#include <cstdio>
#include <cstring>
#include <string>
#include "LogPlus.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDbManager::RuleDbManager(const char_t* dbName) {
	m_pRuleModel = RuleModelDb::CreateModel(dbName);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDbManager::~RuleDbManager() {
	delete m_pRuleModel;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value RuleDbManager::GetAllRule() {
	Json::Value vec(Json::arrayValue);

	RulesItemDb_t rules = m_pRuleModel->Find<RuleDb>();
	for (RulesItemDb_t::const_iterator it = rules.begin(); it != rules.end();
			it++) {
		RuleItemDb_t temp = (*it);
		std::string data(temp.Modify()->RuleData.GetValue().c_str());
		Json::Reader reader;
		Json::Value jsonValue = 0;
		reader.parse(data, jsonValue, false);
		vec.append(jsonValue);
	}
	return vec;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value RuleDbManager::GetRule(int_t id) {
	Json::Value jsonRule = 0;
	RuleItemDb_t ruleItem = m_pRuleModel->Find<RuleDb>().Where("RuleID=?").Bind(
			id);
	if (ruleItem.get() != NULL) {
		std::string data(ruleItem.Modify()->RuleData.GetValue().c_str());
		Json::Reader reader;
		reader.parse(data, jsonRule, false);
	}
	return jsonRule;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::AddRule(int_t id, String data) {
	RuleItemDb_t ruleItem = m_pRuleModel->Find<RuleDb>().Where("RuleID=?").Bind(
			id);
	if (ruleItem.get() == NULL) {
		ruleItem = m_pRuleModel->Add(new RuleDb());
		ruleItem.Modify()->RuleID = id;
		ruleItem.Modify()->RuleData = data;
		m_pRuleModel->Add(ruleItem);
		m_pRuleModel->UpdateChanges();
		return TRUE;
	}
	return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::UpdateRule(int_t id, String data) {
	RuleItemDb_t ruleItem = m_pRuleModel->Find<RuleDb>().Where("RuleID=?").Bind(
			id);
	if (ruleItem.get() != NULL) {
		ruleItem.Modify()->RuleData = data;
		m_pRuleModel->Add(ruleItem);
		m_pRuleModel->UpdateChanges();
		return TRUE;
	}
	return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::DeleteRule(int_t id) {
	RuleItemDb_t ruleItem = m_pRuleModel->Find<RuleDb>().Where("RuleID=?").Bind(
			id);
	if (ruleItem.get() != NULL) {
		ruleItem.Remove();
		m_pRuleModel->UpdateChanges();
	}
	return TRUE;
}
