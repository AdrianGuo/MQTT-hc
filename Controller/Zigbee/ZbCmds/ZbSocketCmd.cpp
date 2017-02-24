/*
 * ZbSocketCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <typedefs.h>
#include <stdarg.h>
#include <zcl_ha.hpp>
#include <JsonDevLstAdd.hpp>
#include <JsonDevLstDel.hpp>
#include <JsonDevStt.hpp>
#include <JsonDevResetRes.hpp>
#include <JsonAuthRes.hpp>
#include <JsonDevAdd.hpp>
#include <LogPlus.hpp>
#include <ZbSocketCmd.hpp>

ZbSocketCmd* ZbSocketCmd::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbSocketCmd::ZbSocketCmd() {
    m_pJsonSendSession = JsonSendZigbeeSession::CreateSession();

    m_pJsonSendSession->MapJsonMessage<JsonDevLstAdd>(JsonDevLstAdd::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonDevLstDel>(JsonDevLstDel::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonDevStt>(JsonDevStt::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonDevResetRes>(JsonDevResetRes::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonAuthRes>(JsonAuthRes::GetStrCmd());
    m_pJsonSendSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd());
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbSocketCmd::~ZbSocketCmd() {
    delete m_pJsonSendSession;
    delete s_pInstance;
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
    EvAct EvAct,
    JsonCommand_p pJsonCommand,
    JsonCommand::Flag des
) {
    pJsonCommand->SetSrcFlag(JsonCommand::Flag::Zigbee);
    pJsonCommand->SetDesFlag(des);
    ZbDriver::GetInstance()->SendJsonMessage(EvAct,pJsonCommand);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendLstAdd(
    Devices_t devices,
    JsonCommand::Flag des
) {
    Vector<JsonDevLstAdd::Device_t> vecLstDev;
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        if((*it).Modify()->IsInterested()) {
            JsonDevLstAdd::Device_t temp;
            temp.devid = (*it)->DeviceID.GetValue();
            temp.netwk = NET_ZIGBEE;
            temp.type = (*it)->RealType;
            temp.order = (*it)->Endpoint.GetValue();
            temp.mac = (*it)->MAC.GetValue();
            vecLstDev.push_back(temp);
        }
    }
    SendLstAdd(vecLstDev, des);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendDevAdd(
    i8_t ibAct,
    JsonCommand::Flag des
) {
    JsonMessagePtr<JsonDevAdd> jsonDevAdd = m_pJsonSendSession->GetJsonMapping<JsonDevAdd>();
    JsonCommand_p pJsonCommand = jsonDevAdd->CreateJsonCommand(ibAct);

    SendJsonMessage(EvAct::EA_None, pJsonCommand, des);
}
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendLstAdd(
    Vector<JsonDevLstAdd::Device_t> vecLstDev,
    JsonCommand::Flag des
) {
    if(vecLstDev.size() > 0) {
        JsonMessagePtr<JsonDevLstAdd> jsonZbLstAdd = m_pJsonSendSession->GetJsonMapping<JsonDevLstAdd>();
        JsonCommand_p pJsonCommand = jsonZbLstAdd->CreateJsonCommand(vecLstDev);

        SendJsonMessage(EvAct::EA_None, pJsonCommand, des);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendLstDel(
    Devices_t devices,
    JsonCommand::Flag des
) {
    Vector<JsonDevLstDel::Device_t> vecLstDev;
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        if((*it).Modify()->IsInterested()) {
            JsonDevLstDel::Device_t temp;
            temp.devid = (*it)->DeviceID.GetValue();
            temp.netwk = NET_ZIGBEE;
            temp.order = (*it)->Endpoint.GetValue();
            vecLstDev.push_back(temp);
        }
    }
    if(vecLstDev.size() > 0) {
        JsonMessagePtr<JsonDevLstDel> jsonZbLstDel = m_pJsonSendSession->GetJsonMapping<JsonDevLstDel>();
        JsonCommand_p pJsonCommand = jsonZbLstDel->CreateJsonCommand(vecLstDev);

        SendJsonMessage(EvAct::EA_None, pJsonCommand, des);
    }
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
    Json::Value val,
    JsonCommand::Flag des
) {
    Vector<JsonDevStt::Device_t> vecLstDev;
    JsonDevStt::Device_t temp;
    temp.devid = device->DeviceID.GetValue();
    temp.order = device->Endpoint.GetValue();
    temp.type = device->RealType;
    temp.netwk = NET_ZIGBEE;
    temp.value = val;
    vecLstDev.push_back(temp);

    JsonMessagePtr<JsonDevStt> jsonZbStt = m_pJsonSendSession->GetJsonMapping<JsonDevStt>();
    JsonCommand_p pJsonCommand = jsonZbStt->CreateJsonCommand(vecLstDev);

    SendJsonMessage(EvAct::EA_None, pJsonCommand, des);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendResetRes(
    u8_t byRet,
    JsonCommand::Flag des
) {
    JsonDevResetRes::Device_t temp;
    temp.ret = byRet;
    temp.netwk = NET_ZIGBEE;

    JsonMessagePtr<JsonDevResetRes> jsonZbResetRes = m_pJsonSendSession->GetJsonMapping<JsonDevResetRes>();
    JsonCommand_p pJsonCommand = jsonZbResetRes->CreateJsonCommand(temp);

    SendJsonMessage(EvAct::EA_None, pJsonCommand, des);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbSocketCmd::SendAuthRes(
    NetDevice_t device
){
    JsonMessagePtr<JsonAuthRes> jsonAuthRes = m_pJsonSendSession->GetJsonMapping<JsonAuthRes>();
    JsonCommand_p pJsonCommand = jsonAuthRes->CreateJsonCommand(0, device->DeviceID.GetValue());
    pJsonCommand->SetClientId(device->NetID.GetValue());

    SendJsonMessage(EvAct::EA_None, pJsonCommand, JsonCommand::Flag::Client);
}
