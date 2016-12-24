/*
 * EventDevManager.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#ifndef EVENTDEVMANAGER_HPP_
#define EVENTDEVMANAGER_HPP_

#include "EventRule/EventDev.hpp"
#include "ItemRule/Rule.hpp"
#include "json.h"
#include "typedefs.h"
#include <vector>

class EventDevManager {
public:
	EventDevManager();
	virtual ~EventDevManager();

	void_t ProcessEvent(Json::Value& jsonValueEvent);
	EventDev_p GetEventDev(int_t key);
	void_t RegisterEvent(Rule_p pRule);
	void_t RmRegisterEvent(int_t idRule);
private:
	void_t RemoveEvent(int_t key);
	std::vector<EventDev_p> m_vecEvents;
};

typedef EventDevManager EventDevManager_t;
typedef EventDevManager* EventDevManager_p;

#endif /* EVENTDEVMANAGER_HPP_ */
