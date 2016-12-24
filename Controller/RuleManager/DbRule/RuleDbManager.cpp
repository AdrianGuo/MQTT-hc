/*
 * RuleDbManager.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: phind
 */

#include "DbRule/RuleDbManager.hpp"
#include "json.h"
#include <sqlite3.h>
#include <cstdio>
#include <cstring>
#include <string>
#include "LogPlus.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDbManager::RuleDbManager(const char_t* dbName) {
	strcpy(this->dbName, dbName);
	sprintf(this->dbPath, "%s", this->dbName);
	CreateDb();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
RuleDbManager::~RuleDbManager() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value RuleDbManager::GetAllRule() {
	sqlite3 *db;
	sqlite3_stmt *statement;
	int rc = 0;
	Json::Value vec(Json::arrayValue);

	if (sqlite3_open(this->dbPath, &db) == SQLITE_OK) {
		std::string cmd = "SELECT * FROM RULE";
		if (sqlite3_prepare_v2(db, cmd.c_str(), -1, &statement, 0) == SQLITE_OK) {
			while (1) {
				rc = sqlite3_step(statement);
				if (rc == SQLITE_DONE)
					break;
				if (rc != SQLITE_ROW) {
					rc = -1;
					break;
				}
				std::string data(
						(const char*) sqlite3_column_text(statement, 1));
				Json::Reader reader;
				Json::Value jsonValue = 0;
				reader.parse(data, jsonValue, false);
				vec.append(jsonValue);
			}
			sqlite3_finalize(statement);
		}
		sqlite3_close(db);
	}
	return vec;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value RuleDbManager::GetRule(int_t id) {
	sqlite3 *db;
	sqlite3_stmt *statement;
	int rc = 0;
	Json::Value jsonRule = 0;
	std::string idString = std::to_string(id);

	if (sqlite3_open(this->dbPath, &db) == SQLITE_OK) {
		std::string cmd = "SELECT * FROM RULE WHERE ID=" + idString;
		if (sqlite3_prepare_v2(db, cmd.c_str(), -1, &statement, 0) == SQLITE_OK) {
			rc = sqlite3_step(statement);
			if (rc == SQLITE_DONE) {
				sqlite3_close(db);
				return jsonRule;
			}
			if (rc != SQLITE_ROW) {
				rc = -1;
				sqlite3_close(db);
				return jsonRule;
			}
			std::string data((const char*) sqlite3_column_text(statement, 1));
			Json::Reader reader;
			reader.parse(data, jsonRule, false);
			sqlite3_finalize(statement);
		}
		sqlite3_close(db);
	}
	return jsonRule;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::AddRule(int_t id, String data) {
	std::string idString = std::to_string(id);
	std::string cmd = "INSERT INTO 'RULE' VALUES(" + idString + ",'"
			+ data.element + "');";
	return ExecDbCmd(String(cmd.c_str()));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::UpdateRule(int_t id, String data) {
	std::string idString = std::to_string(id);
	std::string cmd = "UPDATE RULE SET DATA='" + data.element + "' WHERE ID="
			+ idString;
	return ExecDbCmd(String(cmd.c_str()));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::DeleteRule(int_t id) {
	std::string idString = std::to_string(id);
	std::string cmd = "DELETE FROM RULE WHERE ID=" + idString;
	return ExecDbCmd(String(cmd.c_str()));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t RuleDbManager::CreateDb() {
	sqlite3 *db;
	char sql[500];
	if (sqlite3_open(this->dbPath, &db) == SQLITE_OK) {
		// Set PRAGMA
		sqlite3_exec(db, "PRAGMA encoding = \"UTF-8\"", 0, 0, 0);
		sqlite3_exec(db, "PRAGMA foreign_keys = ON", 0, 0, 0);

		sprintf(sql, "CREATE TABLE IF NOT EXISTS `RULE` ("
				"`ID` INTEGER PRIMARY KEY AUTOINCREMENT,"
				"`DATA` TEXT NOT NULL"
				");");
		sqlite3_exec(db, sql, 0, 0, 0);

		// Close slqlite connection;
		sqlite3_close(db);

	}
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t RuleDbManager::ExecDbCmd(String cmd) {
	sqlite3 *db;
	if (sqlite3_open(this->dbPath, &db) == SQLITE_OK) {
		// Set PRAGMA
		sqlite3_exec(db, "PRAGMA encoding = \"UTF-8\"", 0, 0, 0);
		sqlite3_exec(db, "PRAGMA foreign_keys = ON", 0, 0, 0);
//		LOG_DEBUG("ExecCmd = %s", cmd.element);
		int_t reslutExecCmd = sqlite3_exec(db, cmd.element.c_str(), 0, 0, 0);
		// Close slqlite connection;
		sqlite3_close(db);
		if (reslutExecCmd == 0)
			return TRUE;
	}
	return FALSE;
}
