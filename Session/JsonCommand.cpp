#include "typedefs.h"
#include "JsonCommand.hpp"

/**
 * @func   JsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::JsonCommand(
    JsonCommand_p pJsonCommand
) : m_strFullCommand (pJsonCommand->GetFullCommand()),
    m_dwSrcFlag (pJsonCommand->GetSrcFlag()),
    m_dwDesFlag (pJsonCommand->GetDesFlag()),
    m_jsonValue (pJsonCommand->GetJsonOjbect()),
    m_boJsonAvailable (TRUE),
    m_dwClientId (0) {
}

/**
 * @func   JsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::JsonCommand(
    String strFullCommand,
    String strJson,
    u32_t dwSrcFlag,
    u32_t dwDesFlag
) : m_strFullCommand (strFullCommand),
    m_dwSrcFlag (dwSrcFlag),
    m_dwDesFlag (dwDesFlag),
    m_dwClientId (0){
    Json::Reader reader;
    m_boJsonAvailable = reader.parse(strJson, m_jsonValue, false);

}

/**
 * @func   ~JsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::~JsonCommand() {
    m_strFullCommand.clear();
    m_jsonValue.clear();
}


/**
 * @func   SetFullCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetFullCommand(
    String strFullCommand
) {
    m_strFullCommand = strFullCommand;
}

/**
 * @func   GetFullCommand
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetFullCommand() const {
    return m_strFullCommand;
}

/**
 * @func   SetJsonObject
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetJsonObject(
    String strJson
) {
    Json::Reader reader;
    m_boJsonAvailable = reader.parse(strJson, m_jsonValue, false);
}

/**
 * @func   SetJsonObject
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
 * @func   IsJsonAvailable
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
JsonCommand::IsJsonAvailable() {
    return m_boJsonAvailable;
}

/**
 * @func   GetJsonOjbect
 * @brief  None
 * @param  None
 * @retval None
 */
Json::Value&
JsonCommand::GetJsonOjbect() {
    return m_jsonValue;
}

/**
 * @func   GetJsonValue
 * @brief  None
 * @param  None
 * @retval None
 */
String
JsonCommand::GetJsonValue() {
    String strValue;
    strValue = m_jsonValue.toStyledString();
    return strValue;
}

/**
 * @func   SetSrcFlag
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
 * @func   GetSrcFlag
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
JsonCommand::GetSrcFlag() const {
    return m_dwSrcFlag;
}

/**
 * @func   SetDesFlag
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
 * @func   GetDesFlag
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
JsonCommand::GetDesFlag() const {
    return m_dwDesFlag;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
JsonCommand::SetClientId(
    u32_t dwClientID
) {
    m_dwClientId = dwClientID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
JsonCommand::GetClientId() const {
    return m_dwClientId;
}
