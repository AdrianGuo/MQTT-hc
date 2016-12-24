/*
 * EventTimeManager.hpp
 *
 *  Created on: Dec 2, 2016
 *      Author: phind
 */

#ifndef EVENTTIMEMANAGER_HPP_
#define EVENTTIMEMANAGER_HPP_

#include "DateAndTime.hpp"
#include "EventRule/EventTime.hpp"
#include "ItemRule/Rule.hpp"
#include "ItemRule/RuleTimeSchedule.hpp"
#include <stddef.h>
#include "typedefs.h"
#include <iterator>
#include <vector>

class EventTimeManager {
public:

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	EventTimeManager() {
		m_timePointCurrent = -1;
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	virtual ~EventTimeManager() {
		for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size(); nIndex++) {
			if (m_vecEvents[nIndex] != NULL) {
				delete m_vecEvents[nIndex];
				m_vecEvents[nIndex] = NULL;
			}
		}
	}

	/**
	 * @func Kiểm tra các sự kiện thời gian có thoải mãn hay không
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void_t ProcessChecking() {
		if (m_timePointCurrent != (DateAndTime::GetSecTimePoints() / 100)) {
			m_timePointCurrent = DateAndTime::GetSecTimePoints() / 100;
			for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size();
					nIndex++) {
				if (m_vecEvents[nIndex] != NULL) {
					if (m_vecEvents[nIndex]->GetTimepoints()
							== m_timePointCurrent) {
						m_vecEvents[nIndex]->SetEvent();
					}
				} else {
					m_vecEvents.erase(m_vecEvents.begin() + nIndex);
					nIndex--;
				}
			}
		}
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void_t RegisterEvent(Rule_p pRule) {
		if (pRule->GetTimeSchedule().GetTypeTime()
				== RuleTimeSchedule::Timepoints) {
			if (pRule->GetTimeSchedule().GetTimepoints() != -1) {
				EventTime_p event = new EventTime(
						pRule->GetTimeSchedule().GetTimepoints(), pRule);
				m_vecEvents.push_back(event);
			}
		}
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void_t RmRegisterEvent(int_t idRule) {
		for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size(); nIndex++) {
			if (m_vecEvents[nIndex] != NULL) {
				if (m_vecEvents[nIndex]->GetIdRule() == idRule) {
					m_vecEvents.erase(m_vecEvents.begin() + nIndex);
					nIndex--;
				}
			} else {
				m_vecEvents.erase(m_vecEvents.begin() + nIndex);
				nIndex--;
			}
		}
	}
private:
	void_t RemoveEvent(int_t key);
	std::vector<EventTime_p> m_vecEvents;

	int_t m_timePointCurrent;
};

typedef EventTimeManager EventTimeManager_t;
typedef EventTimeManager* EventTimeManager_p;

#endif /* EVENTTIMEMANAGER_HPP_ */
