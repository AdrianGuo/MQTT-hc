/*
 * RuleDb.hpp
 *
 *  Created on: Jan 17, 2017
 *      Author: phind
 */

#ifndef CONTROLLER_RULEMANAGER_DBRULE_RULEDB_HPP_
#define CONTROLLER_RULEMANAGER_DBRULE_RULEDB_HPP_

#include "typedefs.h"
#include "DbItem.hpp"
#include "DbPtr.hpp"
#include "Config.hpp"
#include "ValueIntDb.hpp"
#include "ValueStrDb.hpp"

class RuleDb {
public:
	RuleDb();
	virtual ~RuleDb();

	ValueIntDb_t RuleID;
	ValueStrDb_t RuleData;

	static String GetTableName();
	template<class A> void_t Table(A& action);
};

template<class A>
inline void_t RuleDb::Table(A& action) {
	Column(action, RuleID);
	Column(action, RuleData);
}

typedef RuleDb RuleDb_t;
typedef RuleDb* RuleDb_p;

#endif /* CONTROLLER_RULEMANAGER_DBRULE_RULEDB_HPP_ */
