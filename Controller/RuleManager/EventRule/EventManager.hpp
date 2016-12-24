/*
 * EventManager.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#ifndef EVENTMANAGER_HPP_
#define EVENTMANAGER_HPP_

#include "EventRule/EventDevManager.hpp"
#include "EventRule/EventTimeManager.hpp"
#include "ItemRule/Rule.hpp"
#include "json.h"
#include "typedefs.h"

class EventManager {
public:
	typedef enum {
		Dev, Mode
	} TypeRuleEvent;

	EventManager();
	virtual ~EventManager();

	void_t Process();

	void_t ProcessEvent(Json::Value& jsonValue, TypeRuleEvent typeEvent);
	void_t Register(Rule_p pRule);
	void_t RmRegister(int_t idRule);

	String GetDataDev(int_t key);

private:
	EventDevManager_t m_eventDevManager;
	EventTimeManager_t m_eventTimeManager;
};

typedef EventManager EventManager_t;
typedef EventManager* EventManager_p;

#endif /* EVENTMANAGER_HPP_ */
