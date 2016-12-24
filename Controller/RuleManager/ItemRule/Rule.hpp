/*
 * Rule.hpp
 *
 *  Created on: Nov 16, 2016
 *      Author: phind
 */

#ifndef RULE_HPP_
#define RULE_HPP_

#include "ItemRule/RuleInputDev.hpp"
#include "ItemRule/RuleOutputDev.hpp"
#include "ItemRule/RuleTimeSchedule.hpp"
#include "json.h"
#include "String.hpp"
#include "typedefs.h"
#include <vector>

class Rule {
public:
	typedef enum {
		Auto, Scenes
	} TypeRule;

	Rule();
	Rule(Json::Value& jsonValue);
	virtual ~Rule();

	void_t DisplayInfor();

	// phương thức kiểm tra tất cả điều khiện đầu vào có thỏa mãn hay không.
	bool_t IsActive();

	// phương thức cho phép cập nhật trạng thái thiết bị. Dùng để EventManager gọi khi có sự kiện dev=stt
	void_t UpdateStateDev(int_t key, String dataCurrent);

	// Phương thức cho phép tải dữ liệu vào Rule
	void_t LoadData(Json::Value& jsonValue);

	// phương thức cho phép cập nhật trạng thái cho trường TimeSchedule. Dùng để EventManager gọi khi có sự kiện mốc thời gian
	void_t SetIsValidTimePoints(bool_t isValidTimePoints) {
		if (m_enable) {
			m_timeSchedule.SetIsValidTimePoints(isValidTimePoints);
		}
	}
	/**
	 * Getters & Setters
	 */
	bool_t IsChecking() const {
		return m_checking;
	}

	void_t SetChecking(bool_t checking) {
		if (m_enable) {
			m_checking = checking;
		}
	}

	int_t GetId() const {
		return m_id;
	}

	void_t SetId(int_t id) {
		m_id = id;
	}

	void_t SetEnable(bool_t enable) {
		m_enable = enable;
	}

	bool_t IsEnable() const {
		return m_enable;
	}

	const String& GetName() const {
		return m_name;
	}

	void_t SetName(const String& name) {
		m_name = name;
	}

	TypeRule GetType() const {
		return m_type;
	}

	const std::vector<RuleInputDev_p>& GetVecInputDevs() const {
		return m_vecInputDevs;
	}

	const std::vector<RuleOutputDev_p>& GetVecOutputDevs() const {
		return m_vecOutputDevs;
	}

	const RuleTimeSchedule_t& GetTimeSchedule() const {
		return m_timeSchedule;
	}

private:
	int_t m_id;
	TypeRule m_type;
	String m_name;
	bool_t m_enable;
	bool_t m_checking;
	RuleTimeSchedule_t m_timeSchedule;
	std::vector<RuleInputDev_p> m_vecInputDevs;
	std::vector<RuleOutputDev_p> m_vecOutputDevs;

	void_t ClearData();
};

typedef Rule Rule_t;
typedef Rule* Rule_p;

#endif /* RULE_HPP_ */
