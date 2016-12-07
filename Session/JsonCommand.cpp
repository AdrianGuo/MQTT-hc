#include "typedefs.h"
#include "debug.hpp"
#include "JsonCommand.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
//JsonCommand::JsonCommand() {
////    m_strCmdClass = "";
////    m_strCommand = "";
//    m_dwSrcFlag = SrcDefault;
//    m_dwDesFlag = DesDefault;
//    m_boJsonAvailable = FALSE;
//    m_jsonValue = 0;
//}

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
    m_boJsonAvailable = reader.parse(strJson.element, m_jsonValue, false);

}

/**
 * @func   ~JsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
JsonCommand::~JsonCommand() {
    m_strFullCommand.clear();
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
    m_boJsonAvailable = reader.parse(strJson.element, m_jsonValue, false);
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
    strValue.element = m_jsonValue.toStyledString();
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
