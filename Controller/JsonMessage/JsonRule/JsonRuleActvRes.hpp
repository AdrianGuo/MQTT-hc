/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-25 14:00:00 (Tue, 25 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_RULE_ACTVRES_HPP_
#define JSON_RULE_ACTVRES_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleActvRes {
public:
	JsonRuleActvRes() {
		Refresh();
	}
	virtual ~JsonRuleActvRes() {
	}
	static String GetStrCmd() {
		return "rule=actvres";
	}
	virtual void_t Refresh() {
		m_ruleID = -1;
		m_ret = -1;
	}
	virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

	int_t getRuleId() const {
		return m_ruleID;
	}

	JsonCommand_p CreateJsonCommand(int_t ruleID, int_t ret);

private:
	int_t m_ruleID;
	int_t m_ret;
	bool_t ParseJsonValue(Json::Value& jsonValue);
};

typedef JsonRuleActvRes JsonRuleActvRes_t;
typedef JsonRuleActvRes* JsonRuleActvRes_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleActvRes::ParseJsonCommand(JsonCommand_p pJsonCommand) {
	return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleActvRes::ParseJsonValue(Json::Value& jsonValue) {
	if (!jsonValue.isMember("ruleid")) {
		return FALSE;
	}
	m_ruleID = std::stoi(jsonValue["ruleid"].asCString());
	return TRUE;
}

/**
 * @func   CreateJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline JsonCommand_p JsonRuleActvRes::CreateJsonCommand(int_t ruleID,
		int_t ret) {
	JsonCommand_p pJsonCommand = new JsonCommand("rule=actvres");
	Json::Value jsonValue;

	jsonValue["ruleid"] = std::to_string(ruleID);
	jsonValue["ret"] = std::to_string(ret);
	pJsonCommand->SetJsonObject(jsonValue);

	return pJsonCommand;
}

#endif /* JSON_RULE_ACTVRES_HPP_ */
