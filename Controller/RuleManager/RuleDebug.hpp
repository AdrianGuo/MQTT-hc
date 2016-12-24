/*
 * RuleDebug.hpp
 *
 *  Created on: Nov 23, 2016
 *      Author: phind
 */

#ifndef RULEDEBUG_HPP_
#define RULEDEBUG_HPP_

#include "debug.hpp"

#define DEBUG_RULE

#ifdef DEBUG_RULE
#define debugRule(x)      		DEBUG1(x)
#define errorRule(x)      		ERROR(x)
#define debugRule2(x, args ...) DEBUG2(x,args)
#else /* DEBUG_RULE */
#define debugRule(x)
#define errorRule(x)
#define debugRule2(x, args ...)
#endif /* DEBUG_RULE */

#endif /* RULEDEBUG_HPP_ */
