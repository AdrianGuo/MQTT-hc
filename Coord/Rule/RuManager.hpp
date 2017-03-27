/*
 * RuManager.hpp
 *
 *  Created on: Dec 19, 2016
 *      Author: phind
 */

#ifndef RUMANAGER_HPP_
#define RUMANAGER_HPP_

#include "Typedefs.h"
#include "Functor.hpp"
#include "ICtrller.hpp"
#include "JsonCommand.hpp"

typedef Functor1_t(JsonCommand_p)HandlerRuCmdFunctor_t;
typedef HandlerRuCmdFunctor_t* HandlerRuCmdFunctor_p;

class RuManager {
public:
	RuManager();
	virtual ~RuManager();
	bool_t SetFunctor(CtrllerFunctor_p pRecvFunctor);
	void_t PushJsonCommand(JsonCommand_p pJsonCommand);
	void_t HandlerRuCmd(JsonCommand_p pJsonCommand);
private:
	CtrllerFunctor_p m_pCtrllerFunctor = NULL;
};

typedef RuManager  RuManager_t;
typedef RuManager* RuManager_p;

#endif /* RUMANAGER_HPP_ */
