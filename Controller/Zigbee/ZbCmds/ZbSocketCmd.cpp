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
#include <JsonZbStt.hpp>
#include <JsonZbResetRes.hpp>
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
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonZbStt>(JsonZbStt::GetStrCmd());
    m_pJsonRecvZigbeeSession->MapJsonMessage<JsonZbResetRes>(JsonZbResetRes::GetStrCmd());
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
//    DEBUG1("SendLstAdd");
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
ZbSocketCmd::SendZbStt(
    Device_t device,
    Json::Value val
) {
    JsonMessagePtr<JsonZbStt> jsonZbStt = m_pJsonRecvZigbeeSession->GetJsonMapping<JsonZbStt>();
    JsonCommand_p pJsonCommand = jsonZbStt->CreateJsonCommand(device, val);

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
