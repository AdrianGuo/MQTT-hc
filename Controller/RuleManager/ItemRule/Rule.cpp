/*
 * Rule.cpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#include "LogPlus.hpp"
#include "ItemRule/Rule.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Rule::Rule() {
	m_id = 0;
	m_type = Rule::Auto;
	m_name = "";
	m_enable = FALSE;
	m_checking = FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Rule::Rule(Json::Value& jsonValue) {
	m_id = 0;
	m_type = Rule::Auto;
	m_name = "";
	m_enable = FALSE;
	m_checking = FALSE;
	LoadData(jsonValue);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Rule::~Rule() {
	ClearData();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Rule::UpdateStateDev(int_t key, String dataCurrent) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecInputDevs.size(); nIndex++) {
		if (m_vecInputDevs[nIndex]->GetKey() == key) {
			m_vecInputDevs[nIndex]->SetDataCurrent(dataCurrent);
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Rule::ClearData() {
	m_id = 0;
	m_type = Rule::Auto;
	m_name = "";
	m_enable = FALSE;
	m_checking = FALSE;
	m_timeSchedule.ClearData();
	for (int_t nIndex = 0; nIndex < (int_t) m_vecInputDevs.size(); nIndex++) {
		if (m_vecInputDevs[nIndex] != NULL) {
			delete m_vecInputDevs[nIndex];
			m_vecInputDevs[nIndex] = NULL;
		}
	}
	for (int_t nIndex = 0; nIndex < (int_t) m_vecOutputDevs.size(); nIndex++) {
		if (m_vecOutputDevs[nIndex] != NULL) {
			delete m_vecOutputDevs[nIndex];
			m_vecOutputDevs[nIndex] = NULL;
		}
	}
	m_vecInputDevs.clear();
	m_vecOutputDevs.clear();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t Rule::LoadData(Json::Value& jsonValue) {
	ClearData();
	if (jsonValue.isMember("ruleid")) {
		m_id = std::atoi(jsonValue["ruleid"].asCString());
	}
	if (jsonValue.isMember("name")) {
		m_name = String(jsonValue["name"].asCString());
	}
	if (jsonValue.isMember("type")) {
		if (std::atoi(jsonValue["type"].asCString()) == 0) {
			m_type = Rule::Auto;
		} else {
			m_type = Rule::Scenes;
		}
	}
	if (jsonValue.isMember("enable")) {
		if (std::atoi(jsonValue["enable"].asCString()) == 0) {
			m_enable = TRUE;
		}
	}
	if (jsonValue.isMember("timeschedule")) {
		m_timeSchedule.LoadData(jsonValue["timeschedule"]);
	}
	if (jsonValue.isMember("in")) {
		Json::Value input = jsonValue["in"];
		if (input.isMember("dev")) {
			const Json::Value array = input["dev"];
			for (int_t index = 0; index < (int_t) array.size(); ++index) {
				if (array[index].isMember("devid")
						&& array[index].isMember("ord")
						&& array[index].isMember("net")
						&& array[index].isMember("type")
						&& array[index].isMember("val")
						&& array[index].isMember("cond")) {
					int_t idpPhysic = std::atoi(
							array[index]["devid"].asCString());
					int_t ord = std::atoi(array[index]["ord"].asCString());
					int_t net = std::atoi(array[index]["net"].asCString());
					int_t key = net * 1000000 + idpPhysic * 1000 + ord;
					int_t type = std::atoi(array[index]["type"].asCString());
					int_t cond = std::atoi(array[index]["cond"].asCString());
					String data(array[index].toStyledString().c_str());

					RuleInputDev_p ruleInputDev = new RuleInputDev(key, type,
							cond, data);
					m_vecInputDevs.push_back(ruleInputDev);
				}
			}
		}
	}

	if (jsonValue.isMember("out")) {
		Json::Value output = jsonValue["out"];
		if (output.isMember("dev")) {
			const Json::Value array = output["dev"];
			for (int_t index = 0; index < (int_t) array.size(); ++index) {
				if (array[index].isMember("devid")
						&& array[index].isMember("ord")
						&& array[index].isMember("net")
						&& array[index].isMember("type")
						&& array[index].isMember("val")
						&& array[index].isMember("timer")) {
					int_t timer = std::atoi(array[index]["timer"].asCString());
					String data(array[index].toStyledString().c_str());

					RuleOutputDev_p ruleOutputDev = new RuleOutputDev(timer,
							data);
					m_vecOutputDevs.push_back(ruleOutputDev);
				}
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
void_t Rule::DisplayInfor() {
	LOG_DEBUG("Rule:\tid = %d\tname = %s\ttype = %d\tenable = %d", m_id,
			m_name.c_str(), m_type, m_enable);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t Rule::IsActive() {
	bool_t result = TRUE;
	if (m_enable) {
		// kiem tra input dev:
		for (int_t nIndex = 0; nIndex < (int_t) m_vecInputDevs.size();
				nIndex++) {
			if (!m_vecInputDevs[nIndex]->IsValid()) {
				result = FALSE;
				break;
			}
		}
		// // kiem tra time schedule
		if (!m_timeSchedule.IsValid()) {
			result = FALSE;
		}
	} else {
		result = FALSE;
	}
	m_checking = FALSE; // làm mới checking
	SetIsValidTimePoints(FALSE); // lam moi isValidTimePoints
	return result;
}
