/*
 * RuleTimeSchedule.hpp
 *
 *  Created on: Dec 2, 2016
 *      Author: phind
 */

#ifndef RULETIMESCHEDULE_HPP_
#define RULETIMESCHEDULE_HPP_

#include "DateAndTime.hpp"
#include "json.h"
#include "LogPlus.hpp"
#include "typedefs.h"
#include <cstdlib>

class RuleTimeSchedule {
public:
	typedef enum {
		Timepoints, Timeinterval
	} TypeTime;
	typedef enum {
		Date, AboutTime
	} TypeRepeat;

	RuleTimeSchedule() {
		ClearData();
	}

	~RuleTimeSchedule() {
	}

	void_t ClearData() {
		m_typeTime = RuleTimeSchedule::Timepoints;
		m_typeRepeat = RuleTimeSchedule::Date;

		m_timepoints = -1;
		m_starttime = -1;
		m_endtime = -1;

		m_date = -1;
		m_year = -1;
		m_month = -1;
		m_day = -1;
		m_after = -1;

		isValidTimePoints = FALSE;
	}

	void_t LoadData(Json::Value& jsonValue) {
		ClearData();
		if (jsonValue.isMember("time")) {
			Json::Value jsonTime = jsonValue["time"];
			if (jsonTime.isMember("starttime")
					&& jsonTime.isMember("endtime")) {
				m_starttime = std::atoi(jsonTime["starttime"].asCString());
				m_endtime = std::atoi(jsonTime["endtime"].asCString());
				if (m_starttime == m_endtime) {
					m_typeTime = RuleTimeSchedule::Timepoints;
					m_timepoints = m_starttime;
				} else {
					m_typeTime = RuleTimeSchedule::Timeinterval;
				}
			}
		}
		if (jsonValue.isMember("rp")) {
			Json::Value jsonTime = jsonValue["rp"];
			if (jsonTime.isMember("type")) {
				int_t type = std::atoi(jsonTime["type"].asCString());
				if (type == 0) {
					// Type = Date
					m_typeRepeat = RuleTimeSchedule::Date;
					if (jsonTime.isMember("date")) {
						m_date = std::atoi(jsonTime["date"].asCString());
					}
				} else {
					if (type == 1) {
						// Type = AboutTime
						m_typeRepeat = RuleTimeSchedule::AboutTime;
						if (jsonTime.isMember("abouttime")) {
							m_year = std::atoi(
									jsonTime["abouttime"]["year"].asCString());
							m_month = std::atoi(
									jsonTime["abouttime"]["month"].asCString());
							m_day = std::atoi(
									jsonTime["abouttime"]["day"].asCString());
							m_after = std::atoi(
									jsonTime["abouttime"]["after"].asCString());
						}
					} else {
						// next ...
					}
				}
			}
		}
	}

	bool_t IsValid() {
		// "timeschedule" : "null" mặc định ok
		if (m_starttime == -1 && m_endtime == -1) {
			return TRUE;
		}

		if (m_typeTime == RuleTimeSchedule::Timepoints) {
			if (!isValidTimePoints) {
				return FALSE;
			} else {
				isValidTimePoints = FALSE;
			}
		} else {
			if (m_typeTime == RuleTimeSchedule::Timeinterval) {
				int_t timePointCurrent = (DateAndTime::GetSecTimePoints() / 100);
				if (!(timePointCurrent >= m_starttime
						&& timePointCurrent <= m_endtime)) {
					return FALSE;
				}
			}
		}

		if (m_typeRepeat == RuleTimeSchedule::Date) {
			if (!DateAndTime::checkDate(m_date) || (m_date == -1)) {
				return FALSE;
			}
		} else {
			if (m_typeRepeat == RuleTimeSchedule::AboutTime) {
				if (!DateAndTime::checkAboutTime(m_year, m_month, m_day,
						m_after)) {
					return FALSE;
				}
			}
		}
		return TRUE;
	}

// for test & debug
	void_t DisplayInfor() {
		LOG_DEBUG(
				"TimeSchedule:\n\tType Time = %d\n\tType Time Repeat = %d" "\n\tTimepoints = %d\n\tStartTime = %d\n\tEnd Time = %d\n\tDate = %d\n\tYear = %d\n\tMonth = %d\n\tDay = %d\n\tAfter = %d",
				m_typeTime, m_typeRepeat, m_timepoints, m_starttime, m_endtime,
				m_date, m_year, m_month, m_day, m_after);
	}

// Setters & Getters
	int_t GetTimepoints() const {
		return m_timepoints;
	}

	TypeTime GetTypeTime() const {
		return m_typeTime;
	}

	void_t SetIsValidTimePoints(bool_t isValidTimePoints) {
		this->isValidTimePoints = isValidTimePoints;
	}

private:
	TypeTime m_typeTime;
	TypeRepeat m_typeRepeat;

	int_t m_timepoints;
	int_t m_starttime;
	int_t m_endtime;

	int_t m_date;
	int_t m_year;
	int_t m_month;
	int_t m_day;
	int_t m_after;

	bool_t isValidTimePoints;
};

typedef RuleTimeSchedule RuleTimeSchedule_t;
typedef RuleTimeSchedule* RuleTimeSchedule_p;

#endif /* RULETIMESCHEDULE_HPP_ */
