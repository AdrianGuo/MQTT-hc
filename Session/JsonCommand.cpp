#include "typedefs.h"
#include "debug.hpp"
#include "JsonCommand.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::JsonCommand() {
    m_strCmdClass = "";
    m_strCommand = "";
    m_dwSrcFlag = SrcDefault;
    m_dwDesFlag = DesDefault;
    m_boJsonAvailable = FALSE;
    m_jsonValue = 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::JsonCommand(
    String strCmdClass,
    String strCommand,
    String strJson,
    u32_t  dwSrcFlag,
    u32_t  dwDesFlag
) {
    m_strCmdClass = strCmdClass;
    m_strCommand = strCommand;
    m_dwSrcFlag = dwSrcFlag;
    m_dwDesFlag = dwDesFlag;
    Json::Reader reader;
    m_boJsonAvailable = reader.parse(strJson.element, m_jsonValue, false);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::~JsonCommand() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetCmdClass(
    String strCmdClass
) {
    m_strCmdClass = strCmdClass;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetCmdClass() const {
    return m_strCmdClass;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetCommand(
    String strCmdClass
) {
    m_strCommand = strCmdClass;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetCommand() const {
    return m_strCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetFullCommand() const {
    return m_strCmdClass + "=" + m_strCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetJsonObject(
    String strJson
) {
    Json::Reader reader;
    m_boJsonAvailable = reader.parse(strJson.element, m_jsonValue, false);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetJsonObject(
    Json::Value& jsonValue
) {
    m_jsonValue = jsonValue;
    m_boJsonAvailable = TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
JsonCommand::IsJsonAvailable() {
    return m_boJsonAvailable;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value&
JsonCommand::GetJsonOjbect() {
    return m_jsonValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetJsonValue() {
    String strValue;
    strValue.element = m_jsonValue.toStyledString();
    return strValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetSrcFlag(
    u32_t dwSrcFlag
) {
    m_dwSrcFlag = dwSrcFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
JsonCommand::GetSrcFlag() const {
    return m_dwSrcFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetDesFlag(
    u32_t dwDesFlag
) {
    m_dwDesFlag = dwDesFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
JsonCommand::GetDesFlag() const {
    return m_dwDesFlag;
}
