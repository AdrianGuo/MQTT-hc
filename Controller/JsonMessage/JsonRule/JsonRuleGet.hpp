/*
 * JsonRuleGet.hpp
 *
 *  Created on: Dec 1, 2016
 *      Author: phind
 */

#ifndef JSON_RULE_GET_HPP_
#define JSON_RULE_GET_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleGet: public JsonMessageBase {
public:
	JsonRuleGet() {
		Refresh();
	}
	virtual ~JsonRuleGet() {
	}
	static String GetStrCmd() {
		return "rule=get";
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

typedef JsonRuleGet  JsonRuleGet_t;
typedef JsonRuleGet* JsonRuleGet_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRuleGet::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
	return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func   ParseJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRuleGet::ParseJsonValue(
    Json::Value& jsonValue
) {
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
inline JsonCommand_p
JsonRuleGet::CreateJsonCommand(
    int_t ruleID
) {
	JsonCommand_p pJsonCommand = new JsonCommand("rule=get");
	Json::Value jsonValue;

	jsonValue["ruleid"] = std::to_string(m_ruleID);
	pJsonCommand->SetJsonObject(jsonValue);

	return pJsonCommand;
}

#endif /* !JSON_RULE_GET_HPP_ */
