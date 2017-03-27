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

#ifndef JSON_RULE_ENA_HPP_
#define JSON_RULE_ENA_HPP_

#include "Typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleEna: public JsonMessageBase {
public:
	JsonRuleEna() {
		Refresh();
	}
	virtual ~JsonRuleEna() {
	}
	static String GetStrCmd() {
		return "rule=ena";
	}
	virtual void_t Refresh() {
		m_ruleID = -1;
		m_act = -1;
	}
	virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

	int_t getRuleId() const {
		return m_ruleID;
	}

	int_t GetAct() const {
		return m_act;
	}

	JsonCommand_p CreateJsonCommand(int_t ruleID);

private:
	int_t m_ruleID;
	int_t m_act;
	bool_t ParseJsonValue(Json::Value& jsonValue);
};

typedef JsonRuleEna  JsonRuleEna_t;
typedef JsonRuleEna* JsonRuleEna_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleEna::ParseJsonCommand(JsonCommand_p pJsonCommand) {
	return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t JsonRuleEna::ParseJsonValue(Json::Value& jsonValue) {
	if (!(jsonValue.isMember("ruleid") && jsonValue.isMember("act"))) {
		return FALSE;
	}
	m_ruleID = std::stoi(jsonValue["ruleid"].asCString());
	m_act = std::stoi(jsonValue["act"].asCString());
	return TRUE;
}

#endif /* !JSON_RULE_ENA_HPP_ */
