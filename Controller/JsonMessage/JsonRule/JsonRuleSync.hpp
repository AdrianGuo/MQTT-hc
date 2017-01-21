/*
 * JsonRuleSync.hpp
 *
 *  Created on: Jan 21, 2017
 *      Author: phind
 */

#ifndef CONTROLLER_JSONMESSAGE_JSONRULE_JSONRULESYNC_HPP_
#define CONTROLLER_JSONMESSAGE_JSONRULE_JSONRULESYNC_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"

class JsonRuleSync: public JsonMessageBase {
public:
	JsonRuleSync() {
	}
	virtual ~JsonRuleSync() {
	}
	static String GetStrCmd() {
		return "rule=sync";
	}
};

typedef JsonRuleSync JsonRuleSync_t;
typedef JsonRuleSync* JsonRuleSync_p;

#endif /* CONTROLLER_JSONMESSAGE_JSONRULE_JSONRULESYNC_HPP_ */
