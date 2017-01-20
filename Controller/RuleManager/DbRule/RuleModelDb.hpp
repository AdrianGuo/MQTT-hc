/*
 * RuleModelDb.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: phind
 */

#ifndef CONTROLLER_RULEMANAGER_DBRULE_RULEMODELDB_HPP_
#define CONTROLLER_RULEMANAGER_DBRULE_RULEMODELDB_HPP_

#include "typedefs.h"
#include "DbRule/RuleDb.hpp"
#include "DbContext.hpp"
#include "Collection.hpp"

typedef DbPtr<RuleDb> RuleItemDb_t;
typedef Collection<DbPtr<RuleDb>> RulesItemDb_t;

class RuleModelDb: public DbContext {
private:
	RuleModelDb(const_char_p cDbName);
public:
	static RuleModelDb* CreateModel(const_char_p cDbName);
	virtual ~RuleModelDb();

	RulesItemDb_t Rules;
};

typedef RuleModelDb RuleModelDb_t;
typedef RuleModelDb* RuleModelDb_p;

#endif /* CONTROLLER_RULEMANAGER_DBRULE_RULEMODELDB_HPP_ */
