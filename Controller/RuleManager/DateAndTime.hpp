/*
 * DataAndTime.hpp
 *
 *  Created on: Dec 4, 2016
 *      Author: phind
 */

#ifndef DATEANDTIME_HPP_
#define DATEANDTIME_HPP_

#include <time.h>
#include <typedefs.h>
#include <math.h>
#include "RuleManager/RuleDebug.hpp"

class DateAndTime {
public:
	static int_t GetSecTimePoints() {
		time_t rawtime;
		struct tm* tm_info;
		time(&rawtime);
		tm_info = localtime(&rawtime);
		return (tm_info->tm_hour * 10000 + tm_info->tm_min * 100
				+ tm_info->tm_sec);
	}

	static bool_t checkDate(int_t recurrent) {
		time_t rawtime;
		tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		int balance = (recurrent / (int) pow(10, (6 - timeinfo->tm_wday))) % 2;
		if (balance == 0) {
			return FALSE;
		} else {
			return TRUE;
		}
	}

	static bool_t checkAboutTime(int_t year, int_t month, int_t day,
			int_t after) {
		time_t rawtime;
		tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);
		int_t ydayCurrent = timeinfo->tm_yday;

		timeinfo->tm_year = year - 1900;
		timeinfo->tm_mon = month - 1;
		timeinfo->tm_mday = day;
		mktime(timeinfo);
		int_t ydaySetup = timeinfo->tm_yday;

		if ((abs(ydayCurrent - ydaySetup)) % after == 0) {
			return TRUE;
		} else {
			return FALSE;
		}
	}

	static void_t debug() {
		time_t rawtime;
		tm * timeinfo;
		time(&rawtime);
		timeinfo = localtime(&rawtime);

		int_t ydayCurrent = timeinfo->tm_yday;

		timeinfo->tm_mon = 12 - 1;
		timeinfo->tm_mday = 23;
		timeinfo->tm_year = 2016 - 1900;
		mktime(timeinfo);

		int_t ydaySetup = timeinfo->tm_yday;

		if ((abs(ydayCurrent - ydaySetup)) % 20 == 0) {
			debugRule("TRUE");
		} else {
			debugRule("FALSE");
		}
	}
};

#endif /* DATEANDTIME_HPP_ */
