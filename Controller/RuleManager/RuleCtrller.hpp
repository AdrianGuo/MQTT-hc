/*
 * RuleCtrller.hpp
 *
 *  Created on: Nov 15, 2016
 *      Author: phind
 */

#ifndef RULECTRLLER_HPP_
#define RULECTRLLER_HPP_

#include <Functor.hpp>
#include <ICtrller.hpp>
#include <JsonCommand.hpp>
#include <Locker.hpp>
#include <LThread.hpp>
#include <Map.hpp>
#include <Queue.hpp>
#include <RuleManager.hpp>
#include <String.hpp>
#include <Typedefs.h>

typedef Functor1_t(JsonCommand_p)HandlerRuleCmdFunctor_t;
typedef HandlerRuleCmdFunctor_t* HandlerRuleRuleFunctor_p;

class RuleCtrller: public ICtrller {
private:
	Queue<JsonCommand_p> m_queRuleCtrllerJsonCommand;

	LThread_p m_pRuleCtrllerThread;
	Locker_p m_pRuleCtrllerLocker;
	CtrllerFunctor_p m_pCtrllerFunctor = NULL;
	threadFunctor_t m_RuleCtrllerThreadFunctor;

	RuleManager_t m_ruleManager;

	typedef Map<String, HandlerRuleCmdFunctor_t> MapHandlerFunctor;
	MapHandlerFunctor m_mapHandlerFunctor;

	void_t RegisterHandler(String strJsonCommand,
			HandlerRuleCmdFunctor_t funcTor);
	void_t RegisterHandler();

	void_t HandlerRuleCmdAddOrEdit(JsonCommand_p pJsonCommand);
	void_t HandlerDevCmdStt(JsonCommand_p pJsonCommand);
	void_t HandlerRuleCmdGet(JsonCommand_p pJsonCommand);

	void_t HandlerRuleCmdDel(JsonCommand_p pJsonCommand);
	void_t HandlerRuleCmdEna(JsonCommand_p pJsonCommand);
	void_t HandlerRuleCmdActv(JsonCommand_p pJsonCommand);

	void_t HandlerRuleCmdInfor(JsonCommand_p pJsonCommand);
	void_t HandlerRuleCmdSync(JsonCommand_p pJsonCommand);
	// TODO next

	void_t PushResJsonCommand(JsonCommand_p pJsonCommand, bool_t result);

	void_p DbCtrlllerThreadProc(void_p pBuffer);
	void_t PushJsonCommand(void_p pBuffer);
public:
	RuleCtrller();
	virtual ~RuleCtrller();

	virtual void_t Process();
	void_t CtrllerRecvFunctor(CtrllerFunctor_p pRecvFunctor);

	void_t Start();
	void_t ProcessHandler(JsonCommand_p pJsonCommand);

	// for test & debug
	void_t debug();
};

// For TEST
void TestRule();

typedef RuleCtrller RuleCtrller_t;
typedef RuleCtrller* RuleCtrller_p;
#endif /* RULECTRLLER_HPP_ */
