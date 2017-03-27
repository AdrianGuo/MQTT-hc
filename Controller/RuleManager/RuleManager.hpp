/*
 * RuleManager.hpp
 *
 *  Created on: Nov 17, 2016
 *      Author: phind
 */

#ifndef RULEMANAGER_HPP_
#define RULEMANAGER_HPP_

#include "DbRule/RuleDbManager.hpp"
#include "EventRule/EventManager.hpp"
#include "ICtrller.hpp"
#include "ItemRule/Rule.hpp"
#include "ItemRule/RuleItemActive.hpp"
#include "json.h"
#include "Typedefs.h"
#include <vector>
#include "JsonRuleDel.hpp"
#include "JsonRuleActv.hpp"
#include "JsonRuleEna.hpp"

#define RULE_NAME_DB 	"rule.db"

class RuleManager {
public:
	RuleManager();
	virtual ~RuleManager();

	void_t Innit();
	void_t SetFunctor(CtrllerFunctor_p pRecvFunctor);
	void_t Process();

	void_t ProcessEvent(Json::Value& jsonValue,
			EventManager::TypeRuleEvent typeEvent);

	bool_t AddRule(Json::Value& jsonValue);
	bool_t GetInforRule(Json::Value& jsonValue);
	bool_t GetInforAllRule(Json::Value& jsonValue);

	bool_t DeleteRule(JsonRuleDel_t jsonValue);
	bool_t EnableRule(JsonRuleEna_t jsonValue);
	bool_t ActiveRule(JsonRuleAct_t jsonValue);

	// for test & debug
	void_t debug();
	void_t displayListRule();

private:
	CtrllerFunctor_p m_pCtrllerFunctor = NULL;
	void_t PushJsonCommand(void_p pBuffer);

	void_t ProcessCheckRule();
	void_t ProcessOutRule();

	void_t RegisterOutput(Rule_p pRule);
	void_t RmRegisterOutput(int_t idRule);

	Rule_p GetRule(int_t id);
	int_t GetIndexRule(int_t id);

	void_t RemoveRuleId(int_t id);
	void_t RemoveRuleIndex(int_t index);

	std::vector<Rule_p> m_vecRules;
	EventManager_t m_eventManager;
	RuleDbManager_t m_dbManager = RuleDbManager(RULE_NAME_DB);
	std::vector<RuleItemActive_t> m_vecItemsAct;
};

typedef RuleManager RuleManager_t;
typedef RuleManager* RuleManager_p;
#endif /* RULEMANAGER_HPP_ */
