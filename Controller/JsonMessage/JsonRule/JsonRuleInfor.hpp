/*
 * JsonRuleInfor.hpp
 *
 *  Created on: Dec 1, 2016
 *      Author: phind
 */

#ifndef JSON_RULE_INFOR_HPP_
#define JSON_RULE_INFOR_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleInfor: public JsonMessageBase {
public:
	JsonRuleInfor() {
	}
	virtual ~JsonRuleInfor() {
	}
	static String GetStrCmd() {
		return "rule=getres";
	}
};

typedef JsonRuleInfor  JsonRuleInfor_t;
typedef JsonRuleInfor* JsonRuleInfor_p;

#endif /* !JSON_RULE_INFOR_HPP_ */
