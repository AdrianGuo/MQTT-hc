/*
 * EventDev.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#ifndef EVENTDEV_HPP_
#define EVENTDEV_HPP_

#include "ItemRule/Rule.hpp"
#include "String.hpp"
#include "typedefs.h"
#include <iterator>
#include <vector>

class EventDev {
public:

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	EventDev(int_t key, String data) {
		m_key = key;
		m_data = data;
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	~EventDev() {
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	const String& GetData() const {
		return m_data;
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void SetData(const String& data) {
		if (this->m_data == data) {
			return;
		}
		this->m_data = data;
		for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
			if (m_vecRules[nIndex] != NULL) {
				m_vecRules[nIndex]->UpdateStateDev(m_key, data);
				m_vecRules[nIndex]->SetChecking(TRUE);
			}
		}
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	int_t GetKey() const {
		return m_key;
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void_t AddRule(Rule_p pRule) {
		pRule->UpdateStateDev(m_key, m_data);
		m_vecRules.push_back(pRule);
	}

	/**
	 * @func
	 * @brief  None
	 * @param  None
	 * @retval None
	 */
	void_t RemoveRule(int_t idRule) {
		for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
			if (m_vecRules[nIndex] != NULL) {
				if (m_vecRules[nIndex]->GetId() == idRule) {
					m_vecRules.erase(m_vecRules.begin() + nIndex);
					nIndex--;
				}
			} else {
				m_vecRules.erase(m_vecRules.begin() + nIndex);
				nIndex--;
			}
		}
	}

private:
	int_t m_key;
	String m_data;
	std::vector<Rule_p> m_vecRules;
};

typedef EventDev EventDev_t;
typedef EventDev* EventDev_p;

#endif /* EVENTDEV_HPP_ */
