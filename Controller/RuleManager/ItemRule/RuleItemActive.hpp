/*
 * WaitTimer.hpp
 *
 *  Created on: Nov 21, 2016
 *      Author: phind
 */

#ifndef RULEITEMACTIVE_HPP_
#define RULEITEMACTIVE_HPP_

#include <sys/time.h>
#include "String.hpp"
#include "typedefs.h"

class RuleItemActive {
public:
	typedef enum {
		Dev, Scenes
	} TypeRuleItemActive;

	RuleItemActive(int_t idRule, u64_t nSecond, String data,
			TypeRuleItemActive type) {
		m_nStartTime = GetTickCountRule();
		m_nDuration = nSecond * 1000;
		m_idRule = idRule;
		m_data = data;
		m_type = type;
	}

	~RuleItemActive() {
	}

	bool_t IsActive() {
		return GetRemainTime();
	}

	const String& GetData() const {
		return m_data;
	}

	TypeRuleItemActive GetType() const {
		return m_type;
	}

	int_t GetIdRule() const {
		return m_idRule;
	}

private:
	bool_t GetRemainTime() {
		u64_t nElapsed = GetTickCountRule() - m_nStartTime;
		if (nElapsed >= m_nDuration) {
			return TRUE;
		} else
			return FALSE;
	}

	u32_t GetTickCountRule() {
		struct timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	int_t m_idRule;
	TypeRuleItemActive m_type;
	String m_data;
	u64_t m_nStartTime;
	u64_t m_nDuration;
};

typedef RuleItemActive RuleItemActive_t;
typedef RuleItemActive* RuleItemActive_p;

#endif /* RULEITEMACTIVE_HPP_ */
