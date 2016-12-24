/*
 * RuleDbManager.hpp
 *
 *  Created on: Nov 21, 2016
 *      Author: phind
 */

#ifndef RULEDBMANAGER_HPP_
#define RULEDBMANAGER_HPP_

#include "String.hpp"
#include "typedefs.h"

namespace Json {
class Value;
} /* namespace Json */

class RuleDbManager {
public:
	RuleDbManager(const char_t* dbName);
	virtual ~RuleDbManager();

	Json::Value GetAllRule();
	Json::Value GetRule(int_t id);
	bool_t AddRule(int_t id, String data);
	bool_t UpdateRule(int_t id, String data);
	bool_t DeleteRule(int_t id);

private:
	char_t dbPath[60];
	char_t dbName[40];
	void_t CreateDb();
	bool_t ExecDbCmd(String cmd);
};

typedef RuleDbManager RuleDbManager_t;
typedef RuleDbManager* RuleDbManager_p;

#endif /* RULEDBMANAGER_HPP_ */
