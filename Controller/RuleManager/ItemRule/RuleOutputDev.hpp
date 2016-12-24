/*
 * .hpp
 *
 *  Created on: Nov 17, 2016
 *      Author: phind
 */

#ifndef RULEOUTPUTDEV_HPP_
#define RULEOUTPUTDEV_HPP_

#include "json.h"
#include "String.hpp"
#include "typedefs.h"
#include <string>

class RuleOutputDev {
public:

	RuleOutputDev(int_t timer, String data) {
		m_timer = timer;
		Json::Reader reader;
		Json::Value jsonValue = 0;
		reader.parse(data.element, jsonValue, false);
		jsonValue.removeMember("timer");
		m_data = String(jsonValue.toStyledString().c_str());
	}

	~RuleOutputDev() {
	}

	const String& GetData() const {
		return m_data;
	}

	int_t GetTimer() const {
		return m_timer;
	}

private:
	int_t m_timer;
	String m_data;
};

typedef RuleOutputDev  RuleOutputDev_t;
typedef RuleOutputDev* RuleOutputDev_p;

#endif /* RULEOUTPUTDEV_HPP_ */
