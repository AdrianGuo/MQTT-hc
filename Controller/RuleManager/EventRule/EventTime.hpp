/*
 * EventTime.hpp
 *
 *  Created on: Dec 2, 2016
 *      Author: phind
 */

#ifndef EVENTTIME_HPP_
#define EVENTTIME_HPP_

#include "ItemRule/Rule.hpp"
#include "ItemRule/RuleTimeSchedule.hpp"
#include "typedefs.h"

class EventTime {
public:

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	EventTime(int_t timepoints, Rule_p pRule) {
		m_timepoints = timepoints;
		m_pRule = pRule;
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	~EventTime() {
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void SetEvent() {
		if (m_pRule != NULL) {
			m_pRule->SetIsValidTimePoints(TRUE);
			m_pRule->SetChecking(TRUE);
		}
	}

	// Setters & Getters
	int_t GetTimepoints() const {
		return m_timepoints;
	}

	int_t GetIdRule() {
		if (m_pRule != NULL) {
			return m_pRule->GetId();
		} else {
			return -1;
		}
	}

private:
	int_t m_timepoints;
	Rule_p m_pRule = NULL;
};

typedef EventTime EventTime_t;
typedef EventTime* EventTime_p;

#endif /* EVENTTIME_HPP_ */
