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
	}
	virtual ~JsonRuleDel() {
	}
	static String GetStrCmd() {
		return "rule=del";
	}
	virtual void_t Refresh() {
		m_vecLstRule.clear();
	}
	virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

	const Vector<int_t>& GetVecLstRule() const {
		return m_vecLstRule;
	}

private:
	Vector<int_t> m_vecLstRule;
	bool_t ParseJsonValue(Json::Value& jsonValue);
};

typedef JsonRuleDel  JsonRuleDel_t;
typedef JsonRuleDel* JsonRuleDel_p;

/**
 * @func   ParseJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRuleDel::ParseJsonCommand(
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
JsonRuleDel::ParseJsonValue(
    Json::Value& jsonValue
) {
	if (!jsonValue.isMember("lst")) {
		return FALSE;
	}
	const Json::Value array = jsonValue["lst"];
	for (int_t index = 0; index < (int_t) array.size(); ++index) {
		int_t idRule = std::atoi(array[index].asCString());
		m_vecLstRule.push_back(idRule);
	}
	return TRUE;
}

#endif /* JSON_RULE_DEL_HPP_ */
