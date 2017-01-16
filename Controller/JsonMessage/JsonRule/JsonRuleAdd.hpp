/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-10-25 14:00:00 (Tue, 25 Oct 2016)
 *
 ******************************************************************************/

#ifndef JSON_RULE_ADD_HPP_
#define JSON_RULE_ADD_HPP_

#include "typedefs.h"
#include "json.h"
#include "JsonMessage.hpp"
#include "JsonCommand.hpp"
#include "Vector.hpp"

class JsonRuleAdd: public JsonMessageBase {
public:
	typedef struct {
		i32_t devid;
		u8_t ord;
		u8_t net;
		u8_t type;
		int_t cond;
		Json::Value value;
	} DeviceIn_t, *DeviceIn_p;
	typedef struct {
		i32_t devid;
		u8_t ord;
		u8_t net;
		u8_t type;
		int_t timer;
		Json::Value value;
	} DeviceOut_t, *DeviceOut_p;
	typedef struct {
		Json::Value time;
		Json::Value rp;
	} TimeSchedule_t, *TimeSchedule_p;
private:
	int_t m_id;
	int_t m_type;
	String m_name;
	bool_t m_enable;
	TimeSchedule_t m_timeSchedule;
	Vector<DeviceIn_t> m_vecLstDevIn;
	Vector<DeviceOut_t> m_vecLstDevOut;
	bool_t ParseJsonValue(Json::Value& jsonValue);
public:
	JsonRuleAdd() {
		Refresh();
	}
	virtual ~JsonRuleAdd() {
	}
	static String GetStrCmd() {
		return "rule=add";
	}
	virtual void_t Refresh() {
		m_id = -1;
		m_type = -1;
		m_name = "";
		m_enable = FALSE;
		m_timeSchedule.time = 0;
		m_timeSchedule.rp = 0;
		m_vecLstDevIn.clear();
		m_vecLstDevOut.clear();
	}
	virtual bool_t ParseJsonCommand(JsonCommand_p pJsonCommand);

	bool_t GetEnable() const {
		return m_enable;
	}

	int_t GetId() const {
		return m_id;
	}

	const String& GetName() const {
		return m_name;
	}

	const TimeSchedule_t& GetTimeSchedule() const {
		return m_timeSchedule;
	}

	int_t GetType() const {
		return m_type;
	}

	const Vector<DeviceIn_t>& GetVecLstDevIn() const {
		return m_vecLstDevIn;
	}

	const Vector<DeviceOut_t>& GetVecLstDevOut() const {
		return m_vecLstDevOut;
	}
};

typedef JsonRuleAdd  JsonRuleAdd_t;
typedef JsonRuleAdd* JsonRuleAdd_p;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRuleAdd::ParseJsonCommand(
    JsonCommand_p pJsonCommand
) {
	return ParseJsonValue(pJsonCommand->GetJsonOjbect());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline bool_t
JsonRuleAdd::ParseJsonValue(
    Json::Value& jsonValue
) {
	if (jsonValue.isMember("ruleid")) {
		m_id = std::atoi(jsonValue["ruleid"].asCString());
	} else {
		return FALSE;
	}
	if (jsonValue.isMember("name")) {
		m_name = String(jsonValue["name"].asCString());
	}
	if (jsonValue.isMember("type")) {
		m_type = std::atoi(jsonValue["type"].asCString());
	}
	if (jsonValue.isMember("enable")) {
		if (std::atoi(jsonValue["enable"].asCString()) == 0) {
			m_enable = TRUE;
		}
	}
	if (jsonValue.isMember("timeschedule")) {
		Json::Value timeScheduleJson = jsonValue["timeschedule"];
		if (timeScheduleJson.isMember("time")
				&& timeScheduleJson.isMember("rp")) {
			m_timeSchedule.rp = timeScheduleJson["rp"];
			m_timeSchedule.time = timeScheduleJson["time"];
		}
	}
	if (jsonValue.isMember("in")) {
		Json::Value input = jsonValue["in"];
		if (input.isMember("dev")) {
			const Json::Value array = input["dev"];
			for (int_t index = 0; index < (int_t) array.size(); ++index) {
				if (array[index].isMember("devid")
						&& array[index].isMember("ord")
						&& array[index].isMember("net")
						&& array[index].isMember("type")
						&& array[index].isMember("val")
						&& array[index].isMember("cond")) {

					DeviceIn_t device;

					device.devid = std::atoi(array[index]["devid"].asCString());
					device.ord = std::atoi(array[index]["ord"].asCString());
					device.net = std::atoi(array[index]["net"].asCString());
					device.type = std::atoi(array[index]["type"].asCString());
					device.cond = std::atoi(array[index]["cond"].asCString());
					device.value = array[index]["val"];
					m_vecLstDevIn.push_back(device);
				}
			}
		}
	}

	if (jsonValue.isMember("out")) {
		Json::Value output = jsonValue["out"];
		if (output.isMember("dev")) {
			const Json::Value array = output["dev"];
			for (int_t index = 0; index < (int_t) array.size(); ++index) {
				if (array[index].isMember("devid")
						&& array[index].isMember("ord")
						&& array[index].isMember("net")
						&& array[index].isMember("type")
						&& array[index].isMember("val")
						&& array[index].isMember("timer")) {
					DeviceOut_t device;

					device.devid = std::atoi(array[index]["devid"].asCString());
					device.ord = std::atoi(array[index]["ord"].asCString());
					device.net = std::atoi(array[index]["net"].asCString());
					device.type = std::atoi(array[index]["type"].asCString());
					device.timer = std::atoi(array[index]["timer"].asCString());
					device.value = array[index]["val"];
					m_vecLstDevOut.push_back(device);
				}
			}
		}
	}
	return TRUE;
}

#endif /* JSON_RULE_ADD_HPP_ */
