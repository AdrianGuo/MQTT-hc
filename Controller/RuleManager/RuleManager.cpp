/*
 * RuleManager.cpp
 *
 *  Created on: Nov 17, 2016
 *      Author: phind
 */

#include "Functor.hpp"
#include "ItemRule/RuleOutputDev.hpp"
#include "JsonCommand.hpp"
#include "LogPlus.hpp"
#include "RuleManager.hpp"
#include "String.hpp"
#include "JsonRuleActvRes.hpp"
#include <unistd.h>
#include <iostream>
#include <iterator>
#include <string>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleManager::RuleManager() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleManager::~RuleManager() {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
		if (m_vecRules[nIndex] != NULL) {
			delete m_vecRules[nIndex];
			m_vecRules[nIndex] = NULL;
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::Innit() {
	const Json::Value array = m_dbManager.GetAllRule();
	for (int_t index = 0; index < (int_t) array.size(); ++index) {
		Json::Reader reader;
		Json::Value jsonValue = 0;
		reader.parse(array[index].toStyledString(), jsonValue, false);
		Rule_p pRule = new Rule(jsonValue);
		m_vecRules.push_back(pRule);
		m_eventManager.Register(pRule);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::SetFunctor(CtrllerFunctor_p pRecvFunctor) {
	if (pRecvFunctor != NULL) {
		m_pCtrllerFunctor = pRecvFunctor;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::Process() {
	m_eventManager.Process();
	ProcessCheckRule();
	ProcessOutRule();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::ProcessEvent(Json::Value& jsonValue,
		EventManager::TypeRuleEvent typeEvent) {
	m_eventManager.ProcessEvent(jsonValue, typeEvent);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::AddRule(Json::Value& jsonValue) {
	Rule_p pRule = new Rule(jsonValue);
	if (GetIndexRule(pRule->GetId()) == -1) {
		m_dbManager.AddRule(pRule->GetId(),
				String(jsonValue.toStyledString().c_str()));
		m_vecRules.push_back(pRule);
		m_eventManager.Register(pRule);
		return TRUE;
	} else {
		delete pRule;
		pRule = NULL;
		return FALSE;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::EditRule(Json::Value& jsonValue) {
	Rule_p pRule = new Rule(jsonValue);
	int_t indexRule = GetIndexRule(pRule->GetId());
	if (indexRule != -1) {
		RmRegisterOutput(pRule->GetId());
		m_eventManager.RmRegister(pRule->GetId());
		RemoveRuleIndex(indexRule);

		m_dbManager.UpdateRule(pRule->GetId(),
				String(jsonValue.toStyledString().c_str()));
		m_vecRules.push_back(pRule);
		m_eventManager.Register(pRule);
		return TRUE;
	} else {
		delete pRule;
		pRule = NULL;
		return FALSE;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::DeleteRule(JsonRuleDel_t jsonValue) {
	int_t idRule = jsonValue.getRuleId();
	int_t indexRule = GetIndexRule(idRule);
	if (indexRule != -1) {
		RmRegisterOutput(idRule);
		m_eventManager.RmRegister(idRule);
		RemoveRuleIndex(indexRule);
		m_dbManager.DeleteRule(idRule);
	}
	return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::EnableRule(JsonRuleEna_t jsonValue) {
	int_t idRule = jsonValue.getRuleId();
	bool_t enable = FALSE;
	if (jsonValue.GetAct() == 0) {
		enable = TRUE;
	} else {
		if (jsonValue.GetAct() == 1) {
			enable = FALSE;
		} else {
			return FALSE;
		}
	}
	Rule_p pRule = GetRule(idRule);
	if (pRule != NULL) {
		if (pRule->IsEnable() != enable) {
			pRule->SetEnable(enable);
			Json::Value jsonInfor = m_dbManager.GetRule(idRule);
			jsonInfor["enable"] = std::to_string(jsonValue.GetAct());
			m_dbManager.UpdateRule(idRule,
					String(jsonInfor.toStyledString().c_str()));
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::ActiveRule(JsonRuleAct_t jsonValue) {
	int_t idRule = jsonValue.getRuleId();
	Rule_p pRule = GetRule(idRule);
	if (pRule != NULL) {
		if (pRule->GetType() != Rule::Scenes) {
			return FALSE;
		}
		RegisterOutput(pRule);
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleManager::GetInforRule(Json::Value& jsonValue) {
	if (jsonValue.isMember("ruleid")) {
		int_t idRule = std::atoi(jsonValue["ruleid"].asCString());
		Json::Value jsonInfor = m_dbManager.GetRule(idRule);
		if (jsonInfor != 0) {
			jsonValue.swap(jsonInfor);
			return TRUE;
		} else {
			return FALSE;
		}
	} else {
		return FALSE;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::PushJsonCommand(void_p pBuffer) {
	JsonCommand_p pJsonCommandResult = (JsonCommand_p) pBuffer;
	if (pJsonCommandResult != NULL) {
		LOG_DEBUG("%s%s", pJsonCommandResult->GetFullCommand().c_str(),
				pJsonCommandResult->GetJsonValue().c_str());
	}

	if ((m_pCtrllerFunctor != NULL) && (pBuffer != NULL)) {
//		m_pCtrllerFunctor->operator ()((JsonCommand_p) pBuffer);
		(*m_pCtrllerFunctor)((JsonCommand_p) pBuffer);
	}

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::ProcessCheckRule() {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
		if (m_vecRules[nIndex]->IsChecking()) {
			if (m_vecRules[nIndex]->IsActive()) {
				RegisterOutput(m_vecRules[nIndex]);
//				if (m_vecRules[nIndex]->GetType() == Rule::Auto) {
				JsonRuleActvRes_t jsonRuleActRes;
				JsonCommand_p pJsonCommand = jsonRuleActRes.CreateJsonCommand(
						m_vecRules[nIndex]->GetId(), 0);
				pJsonCommand->SetDesFlag(JsonCommand::NetWork);
				PushJsonCommand(pJsonCommand);
//				}
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
void_t RuleManager::ProcessOutRule() {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecItemsAct.size(); nIndex++) {
		Json::Value vecDevAct(Json::arrayValue);
		bool_t isDevAct = FALSE;
		if (m_vecItemsAct[nIndex].IsActive()) {
			if (m_vecItemsAct[nIndex].GetType() == RuleItemActive::Dev) {
				isDevAct = TRUE;
				Json::Reader reader;
				Json::Value jsonValue = 0;
				reader.parse(m_vecItemsAct[nIndex].GetData(), jsonValue,
						false);
				vecDevAct.append(jsonValue);
				m_vecItemsAct.erase(m_vecItemsAct.begin() + nIndex);
				nIndex--;
			} else {
				// TODO truong hop dau ra la canh ...
			}
		}
		if (isDevAct) {
			Json::Value dataResult;
			dataResult["dev"] = vecDevAct;
			JsonCommand_p pJsonCommandResult = new JsonCommand("dev=set",
					String(dataResult.toStyledString().c_str()),
					JsonCommand::Rule, JsonCommand::Coord);
			PushJsonCommand(pJsonCommandResult);
		}
	}

// TODO chưa kiểm tra đầu ra cảnh ...
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Rule_p RuleManager::GetRule(int_t id) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
		if (m_vecRules[nIndex]->GetId() == id) {
			return m_vecRules[nIndex];
		}
	}
	return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
int_t RuleManager::GetIndexRule(int_t id) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
//		LOG_DEBUG("GetIndexRule = %d", m_vecRules[nIndex]->GetId());
		if (m_vecRules[nIndex]->GetId() == id) {
			return nIndex;
		}
	}
	return -1;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::RemoveRuleId(int_t id) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
		if (m_vecRules[nIndex]->GetId() == id) {
			delete m_vecRules[nIndex];
			m_vecRules[nIndex] = NULL;
			m_vecRules.erase(m_vecRules.begin() + nIndex);
			break;
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::RemoveRuleIndex(int_t index) {
	if (index < (int_t) m_vecRules.size()) {
		delete m_vecRules[index];
		m_vecRules[index] = NULL;
		m_vecRules.erase(m_vecRules.begin() + index);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::RegisterOutput(Rule_p pRule) {
	RmRegisterOutput(pRule->GetId());
	std::vector<RuleOutputDev_p> vecOutputDevs = pRule->GetVecOutputDevs();
	for (int_t index = 0; index < (int_t) vecOutputDevs.size(); ++index) {
		RuleItemActive_t item(pRule->GetId(), vecOutputDevs[index]->GetTimer(),
				vecOutputDevs[index]->GetData(), RuleItemActive::Dev);
		m_vecItemsAct.push_back(item);
	}
// TODO chua xu ly cho dau ra la rule cảnh
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::RmRegisterOutput(int_t idRule) {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecItemsAct.size(); nIndex++) {
		if (m_vecItemsAct[nIndex].GetIdRule() == idRule) {
			m_vecItemsAct.erase(m_vecItemsAct.begin() + nIndex);
			nIndex--;
		}
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::debug() {
	for (int var = 0; var < 60; ++var) {
		LOG_DEBUG("%d", var);
		sleep(1);
		Process();
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleManager::displayListRule() {
	for (int_t nIndex = 0; nIndex < (int_t) m_vecRules.size(); nIndex++) {
		m_vecRules[nIndex]->DisplayInfor();
	}
}
