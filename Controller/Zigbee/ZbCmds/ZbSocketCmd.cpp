/*
 * ZbSocketCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <typedefs.h>
#include <stdarg.h>
#include <zcl_ha.hpp>
#include <JsonZbLstAdd.hpp>
#include <JsonZbLstDel.hpp>
#include <JsonDevRep.hpp>
#include <JsonZbResetRes.hpp>
#include <JsonRGBRes.hpp>
#include <JsonRGBEnableRes.hpp>

#include <ZbSocketCmd.hpp>

ZbSocketCmd* ZbSocketCmd::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbSocketCmd::ZbSocketCmd() {
    m_pJsonRecvZigbeeSession = JsonRecvZigbeeSession::CreateSession();

    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonZbLstAdd>(JsonZbLstAdd::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonZbLstDel>(JsonZbLstDel::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonDevRep>(JsonDevRep::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonZbResetRes>(JsonZbResetRes::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonRGBRes>(JsonRGBRes::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonRGBEnableRes>(JsonRGBEnableRes::GetStrCmd());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbSocketCmd_p
ZbSocketCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbSocketCmd();
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendJsonMessage(
    EvAction avAction,
    JsonCommand_p pJsonCommand
) {
    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zigbee);
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    if (ZbDriver::s_pInstance->m_pZbCtrllerFunctor != NULL)
        ZbDriver::s_pInstance->m_pZbCtrllerFunctor->operator ()(avAction,pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendLstAdd(
    Devices_t devices
) {
    JsonMessagePtr<JsonZbLstAdd> jsonZbLstAdd = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonZbLstAdd>();
    JsonCommand_p pJsonCommand = jsonZbLstAdd->CreateJsonCommand(devices);

    SendJsonMessage(EvAction::None, pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendLstDel(
    Devices_t devices
) {
    JsonMessagePtr<JsonZbLstDel> jsonZbLstDel = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonZbLstDel>();
    JsonCommand_p pJsonCommand = jsonZbLstDel->CreateJsonCommand(devices);

    SendJsonMessage(EvAction::None, pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendDevRep(
    int_t deviceID,
    int_t idwValue
) {
    JsonMessagePtr<JsonDevRep> jsonDevRep = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonDevRep>();
    JsonCommand_p pJsonCommand = jsonDevRep->CreateJsonCommand(deviceID, idwValue);

    SendJsonMessage(EvAction::None, pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendResetRes(
    u8_t byRet
) {
    JsonMessagePtr<JsonZbResetRes> jsonZbResetRes = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonZbResetRes>();
    JsonCommand_p pJsonCommand = jsonZbResetRes->CreateJsonCommand(byRet);

    SendJsonMessage(EvAction::None, pJsonCommand);
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendRGBRes(
    ZbDeviceDb_p device
){
    JsonMessagePtr<JsonRGBRes> jsonRGBRes = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonRGBRes>();
    JsonCommand_p pJsonCommand = jsonRGBRes->CreateJsonCommand(device);

    SendJsonMessage(EvAction::None, pJsonCommand);
}


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendRGBEnableRes(
    u8_t byValue
) {
    JsonMessagePtr<JsonRGBEnableRes> jsonRGBEnableRes = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonRGBEnableRes>();
    JsonCommand_p pJsonCommand = jsonRGBEnableRes->CreateJsonCommand(byValue);

    SendJsonMessage(EvAction::None, pJsonCommand);
}
