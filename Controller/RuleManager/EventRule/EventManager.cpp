/*
 * EventManager.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#include "EventRule/EventManager.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
EventManager::EventManager() {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
EventManager::~EventManager() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventManager::Process() {
	m_eventTimeManager.ProcessChecking();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventManager::Register(Rule_p pRule) {
	m_eventDevManager.RegisterEvent(pRule);
	m_eventTimeManager.RegisterEvent(pRule);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventManager::ProcessEvent(Json::Value& jsonValue,
		TypeRuleEvent typeEvent) {
	if (typeEvent == TypeRuleEvent::Dev) {
		m_eventDevManager.ProcessEvent(jsonValue);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t EventManager::RmRegister(int_t idRule) {
	m_eventDevManager.RmRegisterEvent(idRule);
	m_eventTimeManager.RmRegisterEvent(idRule);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String EventManager::GetDataDev(int_t key) {
	EventDev_p pEventDev = m_eventDevManager.GetEventDev(key);
	if (pEventDev != NULL) {
		return pEventDev->GetData();
	} else {
		return String("no dev");
	}
}
