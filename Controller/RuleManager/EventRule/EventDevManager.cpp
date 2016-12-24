/*
 * EventDevManager.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#include "EventRule/EventDevManager.hpp"
#include "ItemRule/RuleInputDev.hpp"
#include "String.hpp"
#include <cstdlib>
#include <string>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
EventDevManager::EventDevManager() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
EventDevManager::~EventDevManager() {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size(); nIndex++) {
		if (m_vecEvents[nIndex] != NULL) {
			delete m_vecEvents[nIndex];
			m_vecEvents[nIndex] = NULL;
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventDevManager::ProcessEvent(Json::Value& jsonValueEvent) {
	if (jsonValueEvent.isMember("dev")) {
		const Json::Value array = jsonValueEvent["dev"];
		for (int_t index = 0; index < (int_t) array.size(); ++index) {
			if (array[index].isMember("devid") && array[index].isMember("ord")
					&& array[index].isMember("net")
					&& array[index].isMember("type")
					&& array[index].isMember("val")) {
				int_t idpPhysic = std::atoi(array[index]["devid"].asCString());
				int_t ord = std::atoi(array[index]["ord"].asCString());
				int_t net = std::atoi(array[index]["net"].asCString());
				int_t idDev = net * 1000000 + idpPhysic * 1000 + ord;
				EventDev_p event = GetEventDev(idDev);
				if (event == NULL) {
					event = new EventDev(idDev,
							String(array[index].toStyledString().c_str()));
					m_vecEvents.push_back(event);
				} else {
					event->SetData(
							String(array[index].toStyledString().c_str()));
				}
			}
		}
	}

	//TODO chưa xử lý event đầu vào là cảnh ...
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventDevManager::RemoveEvent(int_t key) {
	// NOTHING
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventDevManager::RegisterEvent(Rule_p pRule) {
	std::vector<RuleInputDev_p> vecInputDevs = pRule->GetVecInputDevs();
	for (int_t index = 0; index < (int_t) vecInputDevs.size(); ++index) {
		EventDev_p event = GetEventDev(vecInputDevs[index]->GetKey());
		if (event == NULL) {
			event = new EventDev(vecInputDevs[index]->GetKey(), "no data");
			m_vecEvents.push_back(event);
		}
		event->AddRule(pRule);
	}

	//TODO chưa xử lý dang ky event đầu vào là cảnh ...
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventDevManager::RmRegisterEvent(int_t idRule) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size(); nIndex++) {
		m_vecEvents[nIndex]->RemoveRule(idRule);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
EventDev_p EventDevManager::GetEventDev(int_t key) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecEvents.size(); nIndex++) {
		if (m_vecEvents[nIndex]->GetKey() == key) {
			return m_vecEvents[nIndex];
		}
	}
	return NULL;
}
