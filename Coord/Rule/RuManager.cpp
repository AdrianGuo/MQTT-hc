/*
 * RuManager.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: phind
 */

#include "RuManager.hpp"

RuManager::RuManager() {

}

RuManager::~RuManager() {
}

/**
 * @func   SetFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuManager::SetFunctor(CtrllerFunctor_p pRecvFunctor) {
	if (pRecvFunctor != NULL) {
		m_pCtrllerFunctor = pRecvFunctor;
		return TRUE;
	}
	return FALSE;
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
RuManager::PushJsonCommand(
    JsonCommand_p pJsonCommand
) {
	if ((m_pCtrllerFunctor != NULL) && (pJsonCommand != NULL)) {
		(*m_pCtrllerFunctor)(pJsonCommand);
	}
}

/**
 * @func   HandlerRuCmd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
RuManager::HandlerRuCmd(
    JsonCommand_p pJsonCommand
) {
	pJsonCommand->SetDesFlag(JsonCommand::Rule);
	PushJsonCommand(pJsonCommand);
}
