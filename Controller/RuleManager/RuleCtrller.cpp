/*
 * RuleCtrller.cpp
 *
 *  Created on: Nov 15, 2016
 *      Author: phind
 */

#include "EventRule/EventManager.hpp"
#include "JsonDevStt.hpp"
#include "JsonRuleActv.hpp"
#include "JsonRuleAdd.hpp"
#include "JsonRuleDel.hpp"
#include "JsonRuleEdit.hpp"
#include "JsonRuleEna.hpp"
#include "JsonRuleGet.hpp"
#include "JsonRuleInfor.hpp"
#include <pthread.h>
#include "RuleCtrller.hpp"
#include "RuleDebug.hpp"
#include <unistd.h>
#include <map>
#include <string>

/******************************************************************************/
/*                     PRIVATE TYPES and DEFINITIONS                          */
/******************************************************************************/

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/

/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/******************************************************************************/
/*                             EXPORTED METHODS                               */
/******************************************************************************/

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void TestRule() {

	RuleCtrller_t ruleCtrller;
	ruleCtrller.debug();

	debugRule("---> Finish Test Rule");
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleCtrller::RuleCtrller() {
	m_pRuleCtrllerThread = new LThread();
	m_pRuleCtrllerLocker = new Locker();
	m_RuleCtrllerThreadFunctor = makeFunctor((threadFunctor_p) NULL, *this,
			&RuleCtrller::DbCtrlllerThreadProc);
	m_pRuleCtrllerThread->RegThreadFunctor(&m_RuleCtrllerThreadFunctor);

	m_ruleManager.Innit();
	RegisterHandler();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleCtrller::~RuleCtrller() {
	if (m_pRuleCtrllerThread != NULL) {
		delete m_pRuleCtrllerThread;
		m_pRuleCtrllerThread = NULL;
	}

	if (m_pRuleCtrllerLocker != NULL) {
		delete m_pRuleCtrllerLocker;
		m_pRuleCtrllerLocker = NULL;
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::debug() {
	// TODO TEST BUG
	JsonCommand_p pJsonCommand = NULL;
	pJsonCommand =
//---------------------------------------------------------------------------//
			/*Add & Edit*/
//			new JsonCommand("rule=add",
//					"{\"ruleid\":\"5\",\"name\":\"rule test\",\"type\":\"0\",\"in\":{\"dev\":[{\"devid\":\"0\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"},\"cond\":\"0\"},{\"devid\":\"2\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"},\"cond\":\"0\"}], \"mode\":\"x\"},\"out\":{\"dev\":[{\"devid\":\"3\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"},\"timer\":\"1\"}], \"scenes\":{\"ruleid\":\"xx\"}},\"timeschedule\":{\"time\":{\"starttime\":\"0000\",\"endtime\":\"2400\"},\"rp\":{\"type\":\"0\",\"date\":\"1111111\",\"abouttime\":{\"year\":\"2016\",\"month\":\"08\",\"day\":\"08\",\"after\":\"6\"}}}, \"enable\":\"1\"}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
			/*Dev stt*/
			new JsonCommand("dev=stt",
					"{\"dev\":[{\"devid\":\"55096\",\"ord\":\"5\",\"net\":\"1\",\"type\":\"1\",\"val\":{\"level\":\"1\",\"state\":\"off\"}},{\"devid\":\"2\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"}}]}",
					JsonCommand::SrcDefault, JsonCommand::DesDefault);
	/*Delete*/
//			new JsonCommand("rule=del", "{\"lst\":[\"4\",\"5\"]}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
	/*Enable*/
//			new JsonCommand("rule=ena", "{\"ruleid\":\"4\", \"act\":\"0\"}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
	/*Active*/
//			new JsonCommand("rule=actv", "{\"ruleid\":\"2\"}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
	/*Get Infor*/
//			new JsonCommand("rule=get", "{\"ruleid\":\"5\"}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
//---------------------------------------------------------------------------//
	String strJsonCommandName = pJsonCommand->GetFullCommand();
	MapHandlerFunctor::const_iterator_t it = m_mapHandlerFunctor.find(
			strJsonCommandName);
	if (it != m_mapHandlerFunctor.end()) {
		m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
	}

//	JsonCommand_p pJsonCommand2 = NULL;
//	pJsonCommand2 =
//			new JsonCommand("dev=stt",
//					"{\"dev\":[{\"devid\":\"0\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"}},{\"devid\":\"2\",\"ord\":\"0\",\"net\":\"0\",\"type\":\"1\",\"val\":{\"level\":\"0\"}}]}",
//					JsonCommand::SrcDefault, JsonCommand::DesDefault);
//	String strJsonCommandName2 = pJsonCommand2->GetFullCommand();
//	it = m_mapHandlerFunctor.find(strJsonCommandName2);
//	if (it != m_mapHandlerFunctor.end()) {
//		m_mapHandlerFunctor[strJsonCommandName2](pJsonCommand2);
//	}
//	delete pJsonCommand2;

	m_ruleManager.debug();
	delete pJsonCommand;
	pJsonCommand = NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::Process() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::CtrllerRecvFunctor(CtrllerFunctor_p pRecvFunctor) {
	if (pRecvFunctor != NULL) {
		m_pCtrllerFunctor = pRecvFunctor;
		m_ruleManager.SetFunctor(pRecvFunctor);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::PushResJsonCommand(JsonCommand_p pJsonCommand,
		bool_t result) {
	pJsonCommand->SetFullCommand(
			pJsonCommand->GetFullCommand() + String("res"));
	pJsonCommand->SetDesFlag(JsonCommand::NetWork);
	if (result) {
		pJsonCommand->GetJsonOjbect()["ret"] = String("0").element;
	} else {
		pJsonCommand->GetJsonOjbect()["ret"] = String("1").element;
	}
	PushJsonCommand(pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::PushJsonCommand(void_p pInBuffer) {
	JsonCommand_p pJsonCommandResult = (JsonCommand_p) pInBuffer;
	if (pJsonCommandResult != NULL) {
		debugRule(
				pJsonCommandResult->GetFullCommand().element
						+ pJsonCommandResult->GetJsonValue().element)
	}
	if ((m_pCtrllerFunctor != NULL) && (pInBuffer != NULL)) {
//		m_pCtrllerFunctor->operator ()((JsonCommand_p) pInBuffer);
		(*m_pCtrllerFunctor)((JsonCommand_p) pInBuffer);
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::Start() {
	if (m_pRuleCtrllerThread != NULL) {
		m_pRuleCtrllerThread->Start();
	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::ProcessHandler(JsonCommand_p pJsonCommand) {
	m_pRuleCtrllerLocker->Lock();
	m_queRuleCtrllerJsonCommand.push(pJsonCommand);
	m_pRuleCtrllerLocker->UnLock();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::RegisterHandler(String strJsonCommand,
		HandlerRuleCmdFunctor_t funcTor) {
	m_mapHandlerFunctor[strJsonCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::RegisterHandler() {
	RegisterHandler(JsonRuleAdd::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdAdd));
	RegisterHandler(JsonRuleEdit::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdEdit));
	RegisterHandler(JsonRuleDel::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdDel));
	RegisterHandler(JsonRuleEna::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdEna));
	RegisterHandler(JsonRuleActv::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdActv));
	RegisterHandler(JsonRuleGet::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdGet));
	RegisterHandler(JsonRuleInfor::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerRuleCmdInfor));
	RegisterHandler(JsonDevStt::GetStrCmd(),
			makeFunctor((HandlerRuleRuleFunctor_p) NULL, *this,
					&RuleCtrller::HandlerDevCmdStt));
	// TODO next
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_p RuleCtrller::DbCtrlllerThreadProc(void_p pBuffer) {
	while (TRUE) {
		JsonCommand_p pJsonCommand = NULL;
		m_pRuleCtrllerLocker->Lock();
		if (!m_queRuleCtrllerJsonCommand.empty()) {
			pJsonCommand = m_queRuleCtrllerJsonCommand.front();
			m_queRuleCtrllerJsonCommand.pop();
		}
		m_pRuleCtrllerLocker->UnLock();

		if (pJsonCommand != NULL) {
			String strJsonCommandName = pJsonCommand->GetFullCommand();
			MapHandlerFunctor::const_iterator_t it = m_mapHandlerFunctor.find(
					strJsonCommandName);
			if (it != m_mapHandlerFunctor.end()) {
				m_mapHandlerFunctor[strJsonCommandName](pJsonCommand);
			}
//			debugRule(pJsonCommand->GetJsonValue().element);
		}

		m_ruleManager.Process();

		usleep(5000);
	}

	pthread_exit(NULL);
	return NULL;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdAdd(JsonCommand_p pJsonCommand) {
	Json::Value & jsonValue = pJsonCommand->GetJsonOjbect();
	bool_t result = m_ruleManager.AddRule(jsonValue);
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdEdit(JsonCommand_p pJsonCommand) {
	Json::Value & jsonValue = pJsonCommand->GetJsonOjbect();
	bool_t result = m_ruleManager.EditRule(jsonValue);
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdDel(JsonCommand_p pJsonCommand) {
	JsonRuleDel_t jsonValue;
	bool_t result = FALSE;
	if (jsonValue.ParseJsonCommand(pJsonCommand)) {
		result = m_ruleManager.DeleteRule(jsonValue);
	}
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdEna(JsonCommand_p pJsonCommand) {
	JsonRuleEna_t jsonValue;
	bool_t result = FALSE;
	if (jsonValue.ParseJsonCommand(pJsonCommand)) {
		result = m_ruleManager.EnableRule(jsonValue);
	}
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdActv(JsonCommand_p pJsonCommand) {
	JsonRuleAct_t jsonValue;
	bool_t result = FALSE;
	if (jsonValue.ParseJsonCommand(pJsonCommand)) {
		result = m_ruleManager.ActiveRule(jsonValue);
	}
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdGet(JsonCommand_p pJsonCommand) {
	Json::Value & jsonValue = pJsonCommand->GetJsonOjbect();
	bool_t result = m_ruleManager.GetInforRule(jsonValue);
	PushResJsonCommand(pJsonCommand, result);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerRuleCmdInfor(JsonCommand_p pJsonCommand) {
	// TODO
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleCtrller::HandlerDevCmdStt(JsonCommand_p pJsonCommand) {
	Json::Value & jsonValue = pJsonCommand->GetJsonOjbect();
	m_ruleManager.ProcessEvent(jsonValue, EventManager::Dev);
}
