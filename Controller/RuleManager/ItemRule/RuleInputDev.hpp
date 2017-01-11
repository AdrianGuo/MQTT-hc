/*
 * RuleInputDev.hpp
 *
 *  Created on: Nov 17, 2016
 *      Author: phind
 */

#ifndef RULEINPUTDEV_HPP_
#define RULEINPUTDEV_HPP_

#include "json.h"
#include "String.hpp"
#include "typedefs.h"
#include <iostream>
#include <string>

class RuleInputDev {

private:
	typedef enum {
		Equal = 0, 				// ==
		NotEqual = 1, 			// !=
		LessThan = 2, 			// <
		GreaterThan = 3, 		// >
		LessThanOrEqual = 4, 	//<=
		GreaterThanOrEqual = 5 	//>=
	} TypeCond;

public:
	RuleInputDev(int_t key, int_t type, int_t cond, String data) {
		m_key = key;
		m_cond = cond;
		m_type = type;
		Json::Reader reader;
		Json::Value jsonValue = 0;
		reader.parse(data, jsonValue, false);
		m_value = jsonValue["val"];
		m_valueCurrent = 0;
	}

	~RuleInputDev() {
	}
	// phuong thuc cho phep kiem tra tinh hop le cua input
	// chua xet den dieu kien cond
	bool_t IsValid() {
		// TODO Checking
		if (m_valueCurrent == 0)
			return FALSE;
		bool_t result = FALSE;
		switch (m_type) {

		case 1: // cong tac
		case 9: // binary input
		case 4: // rem
		{
			if (m_value.isMember("state") && m_valueCurrent.isMember("state")) {
				String stateSetup = String(m_value["state"].asCString());
				String stateCurrent = String(
						m_valueCurrent["state"].asCString());
				return CheckingVlaueState(stateSetup, stateCurrent);
			}
			break;
		}
		case 2: // dimmer
		case 3: // quat
		{
			if (m_value.isMember("level") && m_valueCurrent.isMember("level")
					&& m_value.isMember("state")
					&& m_valueCurrent.isMember("state")) {
				String stateSetup = String(m_value["state"].asCString());
				String stateCurrent = String(
						m_valueCurrent["state"].asCString());
				if (CheckingVlaueState(stateSetup, stateCurrent)) {
					if (m_cond == TypeCond::Equal)
						return TRUE;
					if (IsCheckingLevel(stateSetup)) {
						int_t levelSetup = std::atoi(
								m_value["level"].asCString());
						int_t levelCurrent = std::atoi(
								m_valueCurrent["level"].asCString());
						return CheckingValueLevel(levelSetup, levelCurrent);
					} else {
						return TRUE;
					}
				} else {
					return FALSE;
				}
			}
			break;
		}

		case 5:  // IR
		{
			if (m_value.isMember("act") && m_valueCurrent.isMember("act")
					&& m_value.isMember("irid")
					&& m_valueCurrent.isMember("irid")) {
				int_t actSetup = std::atoi(m_value["act"].asCString());
				int_t actCurrent = std::atoi(m_valueCurrent["act"].asCString());
				if (actSetup == actCurrent) {
					if (actSetup == 3 || actSetup == 4 || actSetup == 5) {
						int_t iridSetup = std::atoi(
								m_value["irid"].asCString());
						int_t iridCurrent = std::atoi(
								m_valueCurrent["irid"].asCString());
						if (iridSetup == iridCurrent) {
							return TRUE;
						} else {
							return FALSE;
						}
					} else {
						return FALSE;
					}
				} else {
					return FALSE;
				}
			}
			break;
		}

		case 6: // RGB
		{
			if (m_value.isMember("level") && m_valueCurrent.isMember("level")
					&& m_value.isMember("state")
					&& m_valueCurrent.isMember("state")) {
				String stateSetup = String(m_value["state"].asCString());
				String stateCurrent = String(
						m_valueCurrent["state"].asCString());
				if (CheckingVlaueState(stateSetup, stateCurrent)) {
					return TRUE;
				} else {
					return FALSE;
				}
			}
			break;
		}

		case 7:  // PIR dien
		{
			break;
		}

		case 8:  // Dieu hoa Daikin
		{
			return CheckingVlaueState(String(m_value.toStyledString().c_str()),
					String(m_valueCurrent.toStyledString().c_str()));
			break;
		}

		case 128:	// Cam bien chuyen dong
		case 129:	// Cam bien cua
		case 130:	// Cam bien nhiet do
		case 131:	// Cam bien do am
		case 132:	// Cam bien anh sang
		{
			if (m_value.isMember("level") && m_valueCurrent.isMember("level")) {
				int_t levelSetup = std::atoi(m_value["level"].asCString());
				int_t levelCurrent = std::atoi(
						m_valueCurrent["level"].asCString());
				return CheckingValueLevel(levelSetup, levelCurrent);
			}
			break;
		}
		}

		return result;
	}

	int_t GetKey() const {
		return m_key;
	}

	int_t GetCond() const {
		return m_cond;
	}

	void_t SetDataCurrent(String dataCurrent) { // Khong xu ly tong quat
		Json::Reader reader;
		Json::Value jsonValue = 0;
		if (reader.parse(dataCurrent, jsonValue, false)) {
			if (jsonValue.isMember("val")) {
				m_valueCurrent = jsonValue["val"];
			}
		} else {
			m_valueCurrent = 0;
		}
	}

private:
	int_t m_key;
	int_t m_type;
	int_t m_cond;
	Json::Value m_value;
	Json::Value m_valueCurrent;

	bool_t CheckingValueLevel(int_t levelSetup, int_t levelCurrent) {
		if (levelSetup == -1)
			return TRUE;
		switch (m_cond) {
		case Equal: {
			if (levelSetup == levelCurrent)
				return TRUE;
			break;
		}
		case NotEqual: {
			if (levelSetup != levelCurrent)
				return TRUE;
			break;
		}
		case LessThan: {
			if (levelSetup < levelCurrent)
				return TRUE;
			break;
		}
		case GreaterThan: {
			if (levelSetup > levelCurrent)
				return TRUE;
			break;
		}
		case LessThanOrEqual: {
			if (levelSetup <= levelCurrent)
				return TRUE;
			break;
		}
		case GreaterThanOrEqual: {
			if (levelSetup >= levelCurrent)
				return TRUE;
			break;
		}
		default:
			break;
		}
		return FALSE;
	}

	bool_t CheckingVlaueState(String stateSetup, String stateCurrent) {
		if (stateSetup == stateCurrent) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	bool_t IsCheckingLevel(String state) {
		if (state == String("off")) {
			return FALSE;
		} else {
			return TRUE;
//			if(state == String("off")){
//
//			}else{
//
//			}
		}
	}
};

typedef RuleInputDev RuleInputDev_t;
typedef RuleInputDev* RuleInputDev_p;

#endif /* RULEINPUTDEV_HPP_ */
