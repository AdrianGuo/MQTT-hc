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

#ifndef JSON_RULE_DEL_HPP_
#define JSON_RULE_DEL_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

class JsonRuleDel: public JsonMessageBase {
public:
	JsonRuleDel() {
		Refresh();
	}
	virtual ~JsonRuleDel() {
	}
	static String GetStrCmd() {
		return "rule=del";
	}
	virtual void_t Refresh() {
		m_ruleID = -1;
	}
	virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

	int_t getRuleId() const {
		return m_ruleID;
	}

	JsonCommand_p CreateJsonCommand(int_t ruleID);

private:
	int_t m_ruleID;
	bool_t ParseJsonValue(Json::Value& jsonValue);
};

typedef JsonRuleDel JsonRuleDel_t;
typedef JsonRuleDel* JsonRuleDel_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleDel::ParseJsonCommand(JsonCommand_p pJsonCommand) {
	return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleDel::ParseJsonValue(Json::Value& jsonValue) {
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
inline JsonCommand_p JsonRuleDel::CreateJsonCommand(int_t ruleID) {
	JsonCommand_p pJsonCommand = new JsonCommand("rule=del");
	Json::Value jsonValue;

	jsonValue["ruleid"] = std::to_string(ruleID);
	pJsonCommand->SetJsonObject(jsonValue);

	return pJsonCommand;
}

#endif /* JSON_RULE_DEL_HPP_ */
