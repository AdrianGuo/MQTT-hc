/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: DevManager.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 09:46:10
 *
 ******************************************************************************/
#include "JsonDevGet.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevStt.hpp"
#include "JsonDevOff.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevRestart.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevResetRes.hpp"
#include "JsonDevInfo.hpp"
#include "JsonDevInfoRes.hpp"
#include "JsonDevLst.hpp"
#include "JsonDevLstRes.hpp"
#include "JsonDevLstAdd.hpp"
#include "JsonDevLstAddRes.hpp"
#include "JsonDevLstDel.hpp"
#include "JsonDevLstDelRes.hpp"
#include "JsonDevSync.hpp"
#include "JsonDevSyncRes.hpp"
#include "JsonAuthReq.hpp"

#include "DevManager.hpp"

/**
 * @func   DevManager
 * @brief  None
 * @param  None
 * @retval None
 */
DevManager::DevManager(
) : m_pDbModelDb (DbModel::CreateModel("newhc.db")),
    m_pJsonRecvDevSession (JsonRecvDevSession::CreateSession()),
    m_pJsonSendDevSession (JsonSendDevSession::CreateSession()) {
    RegisterDevSession();
}

/**
 * @func   ~DevManager
 * @brief  None
 * @param  None
 * @retval None
 */
DevManager::~DevManager() {
    if (m_pJsonRecvDevSession != NULL) {
        delete m_pJsonRecvDevSession;
        m_pJsonRecvDevSession = NULL;
    }

    if (m_pJsonSendDevSession != NULL) {
        delete m_pJsonSendDevSession;
        m_pJsonSendDevSession = NULL;
    }
}

/**
 * @func   RegisterDevSession
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::RegisterDevSession() {
    // Recv
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevGet>(JsonDevGet::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevStt>(JsonDevStt::GetStrCmd()); // Recv From Zigbee/Zwave
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevOff>(JsonDevOff::GetStrCmd()); // Recv From Zigbee/Zwave
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevReset>(JsonDevReset::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevInfo>(JsonDevInfo::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevLstAdd>(JsonDevLstAdd::GetStrCmd()); // Recv From Zigbee/Zwave
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevLstDel>(JsonDevLstDel::GetStrCmd()); // Recv From Zigbee/Zwave
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevLstAddRes>(JsonDevLstAddRes::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevLstDelRes>(JsonDevLstDelRes::GetStrCmd()); // Recv From Server
    m_pJsonRecvDevSession->MapJsonMessage<JsonDevLst>(JsonDevLst::GetStrCmd()); // Recv From Server

    m_pJsonRecvDevSession->MapJsonMessage<JsonDevSyncRes>(JsonDevSyncRes::GetStrCmd());

    // Send
    m_pJsonSendDevSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd()); // Send To Zigbee/Zwave
    m_pJsonSendDevSession->MapJsonMessage<JsonDevGet>(JsonDevGet::GetStrCmd()); // Send To Zigbee/Zwave
    m_pJsonSendDevSession->MapJsonMessage<JsonDevStt>(JsonDevStt::GetStrCmd()); // Send To Server
    m_pJsonSendDevSession->MapJsonMessage<JsonDevOff>(JsonDevOff::GetStrCmd()); // Send To Server
    m_pJsonSendDevSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd()); // Send To Zigbee/Zwave
    m_pJsonSendDevSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd()); // Send To Zigbee/Zwave
    m_pJsonSendDevSession->MapJsonMessage<JsonDevReset>(JsonDevReset::GetStrCmd()); // Send To Zigbee/Zwave
//    m_pJsonSendDevSession->MapJsonMessage<JsonDevInfo>(JsonDevInfo::GetStrCmd()); // Send To Server
    m_pJsonSendDevSession->MapJsonMessage<JsonDevLstAdd>(JsonDevLstAdd::GetStrCmd()); // Send To Server
    m_pJsonSendDevSession->MapJsonMessage<JsonDevLstDel>(JsonDevLstDel::GetStrCmd()); // Send To Server
//    m_pJsonSendDevSession->MapJsonMessage<JsonDevLstAddRes>(JsonDevLstAddRes::GetStrCmd()); // Send To Server
//    m_pJsonSendDevSession->MapJsonMessage<JsonDevLstDelRes>(JsonDevLstDelRes::GetStrCmd()); // Send To Server
    m_pJsonSendDevSession->MapJsonMessage<JsonDevLst>(JsonDevLst::GetStrCmd()); // Send To Server
}

/**
 * @func   SetFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
DevManager::SetFunctor(
    CtrllerFunctor_p pRecvFunctor
) {
    if (pRecvFunctor != NULL) {
        m_pCtrllerFunctor = pRecvFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   PushJsonCommand
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::PushJsonCommand(
    JsonCommand_p pJsonCommand
) {
    if ((m_pCtrllerFunctor != NULL) && (pJsonCommand != NULL)) {
        (*m_pCtrllerFunctor)(pJsonCommand);
    }
}

/**
 * @func   HandlerDevCmdSet
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdSet(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevSet> jsonDeviceSet =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevSet>();

    if (!jsonDeviceSet->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    Vector<JsonDevSet::Device_t> lstSetDevice = jsonDeviceSet->LstDev();
    Vector<JsonDevSet::Device_t> lstSetZwDevice;
    Vector<JsonDevSet::Device_t> lstSetZbDevice;

    for (u32_t i = 0; i < lstSetDevice.size(); i++) {
        if (lstSetDevice[i].netwk == NetWk::Zwave) { // Zwave Device
            JsonDevSet::Device_t zwDevice;
            zwDevice.devid = lstSetDevice[i].devid;
            zwDevice.order = lstSetDevice[i].order;
            zwDevice.type  = lstSetDevice[i].type;
            zwDevice.value = lstSetDevice[i].value;
            zwDevice.netwk = lstSetDevice[i].netwk;
            lstSetZwDevice.push_back(zwDevice);
        } else if (lstSetDevice[i].netwk == NetWk::Zigbee ||
                lstSetDevice[i].netwk == NetWk::Wifi) { // Zigbee Device
            JsonDevSet::Device_t zbDevice;
            zbDevice.devid = lstSetDevice[i].devid;
            zbDevice.order = lstSetDevice[i].order;
            zbDevice.type  = lstSetDevice[i].type;
            zbDevice.value = lstSetDevice[i].value;
            zbDevice.netwk = lstSetDevice[i].netwk;
            lstSetZbDevice.push_back(zbDevice);
//        } else if (lstSetDevice[i].netwk == NetWk::Wifi) { // Wifi Device

        } else if (lstSetDevice[i].netwk == NetWk::Bluetooth) { // Bluetooth Device

        }
    }

    if (lstSetZwDevice.size() > 0) {
        JsonMessagePtr<JsonDevSet> jsonZwDeviceSet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevSet>();
        JsonCommand_p pJsonCommand =
        jsonZwDeviceSet->CreateJsonCommand(lstSetZwDevice);
        pJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
        PushJsonCommand(pJsonCommand);
    }

    if (lstSetZbDevice.size() > 0) {
        JsonMessagePtr<JsonDevSet> jsonZbDeviceSet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevSet>();
        JsonCommand_p pJsonCommand =
        jsonZbDeviceSet->CreateJsonCommand(lstSetZbDevice);
        pJsonCommand->SetDesFlag(JsonCommand::Flag::Zigbee);
        PushJsonCommand(pJsonCommand);
    }
}

/**
 * @func   HandlerDevCmdGet
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdGet(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevGet> jsonDeviceSet =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevGet>();

    if (!jsonDeviceSet->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    Vector<JsonDevGet::Device_t> lstSetDevice = jsonDeviceSet->LstDev();
    Vector<JsonDevGet::Device_t> lstGetZwDevice;
    Vector<JsonDevGet::Device_t> lstGetZbDevice;

    for (u32_t i = 0; i < lstSetDevice.size(); i++) {
        if (lstSetDevice[i].netwk == NetWk::Zwave) { // Zwave Device
            JsonDevGet::Device_t zwDevice;
            zwDevice.devid = lstSetDevice[i].devid;
            zwDevice.order = lstSetDevice[i].order;
            zwDevice.netwk = lstSetDevice[i].netwk;
            zwDevice.type  = lstSetDevice[i].type;
            lstGetZwDevice.push_back(zwDevice);
        } else if (lstSetDevice[i].netwk == 1 ||
                lstSetDevice[i].netwk == 2) { // Zigbee Device
            JsonDevGet::Device_t zbDevice;
            zbDevice.devid = lstSetDevice[i].devid;
            zbDevice.order = lstSetDevice[i].order;
            zbDevice.netwk = lstSetDevice[i].netwk;
            zbDevice.type  = lstSetDevice[i].type;
            lstGetZbDevice.push_back(zbDevice);
//        } else if (lstSetDevice[i].netwk == 2) { // Wifi Device

        } else if (lstSetDevice[i].netwk == 3) { // Bluetooth Device

        }
    }

    if (lstGetZwDevice.size() > 0) {
        JsonMessagePtr<JsonDevGet> jsonZwGet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevGet>();
        JsonCommand_p pJsonCommand =
        jsonZwGet->CreateJsonCommand(lstGetZwDevice);
        pJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
        PushJsonCommand(pJsonCommand);
    }

    if (lstGetZbDevice.size() > 0) {
        JsonMessagePtr<JsonDevGet> jsonZbGet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevGet>();
        JsonCommand_p pJsonCommand =
        jsonZbGet->CreateJsonCommand(lstGetZbDevice);
        pJsonCommand->SetDesFlag(JsonCommand::Flag::Zigbee);
        PushJsonCommand(pJsonCommand);
    }
}

/**
 * @func   HandlerDevCmdStt
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdStt(
    JsonCommand_p pJsonCommand
) {
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func   HandlerDevCmdAdd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevAdd> jsonDeviceAdd =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevAdd>();

    if (!jsonDeviceAdd->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    i8_t ibAction = jsonDeviceAdd->Act();

    JsonMessagePtr<JsonDevAdd> jsonAdd =
    m_pJsonSendDevSession->GetJsonMapping<JsonDevAdd>();

    JsonCommand_p pJsonCmdZwAdd = jsonAdd->CreateJsonCommand(ibAction);
    pJsonCmdZwAdd->SetDesFlag(JsonCommand::Flag::Zwave);
    PushJsonCommand(pJsonCmdZwAdd);

    JsonCommand_p pJsonCmdZbAdd = jsonAdd->CreateJsonCommand(ibAction);
    pJsonCmdZbAdd->SetDesFlag(JsonCommand::Flag::Zigbee);
    PushJsonCommand(pJsonCmdZbAdd);

}

/**
 * @func   HandlerDevCmdDel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevDel> jsonDeviceDel =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevDel>();

    if (!jsonDeviceDel->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    i8_t ibAct = jsonDeviceDel->Act();

    JsonMessagePtr<JsonDevDel> jsonDel =
    m_pJsonSendDevSession->GetJsonMapping<JsonDevDel>();

    JsonCommand_p pJsonCmdZwDel = jsonDel->CreateJsonCommand(ibAct);
    pJsonCmdZwDel->SetDesFlag(JsonCommand::Flag::Zwave);
    PushJsonCommand(pJsonCmdZwDel);

    JsonCommand_p pJsonCmdZbDel = jsonDel->CreateJsonCommand(ibAct);
    pJsonCmdZbDel->SetDesFlag(JsonCommand::Flag::Zigbee);
    PushJsonCommand(pJsonCmdZbDel);
}

/**
 * @func   HandlerDevCmdLstAdd
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdLstAdd(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevLstAdd> jsonDevLstAdd =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevLstAdd>();

    if (!jsonDevLstAdd->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    Vector<JsonDevLstAdd::Device_t> lstDevAdd = jsonDevLstAdd->LstDev();

    for (u32_t i = 0; i < lstDevAdd.size(); i++) {
        DbDevice deviceFind =
        m_pDbModelDb->Find<DbDeviceItem>().Where("DeviceId = ?").Bind(lstDevAdd[i].devid).
        Where( "TypeNet = ?").Bind(lstDevAdd[i].netwk).Where("Position = ?").Bind(lstDevAdd[i].order);

        if (deviceFind.get() == NULL) {
            DbDevice deviceAdd = m_pDbModelDb->Add(new DbDeviceItem());
            deviceAdd.Modify()->DevId = lstDevAdd[i].devid;
            deviceAdd.Modify()->MacId = lstDevAdd[i].mac;
            deviceAdd.Modify()->Position = lstDevAdd[i].order;
            deviceAdd.Modify()->TypeDev  = lstDevAdd[i].type;
            deviceAdd.Modify()->TypeNet  = lstDevAdd[i].netwk;
            if (lstDevAdd[i].order != 0) {
                DbDevice deviceRoot =
                m_pDbModelDb->Find<DbDeviceItem>().Where("DeviceId = ?").Bind(lstDevAdd[i].devid).
                Where( "TypeNet = ?").Bind(lstDevAdd[i].netwk).Where("Position = ?").Bind(0);
                if (deviceRoot.get() != NULL) {
                    deviceAdd.Modify()->Device = deviceRoot;
                }
            }
        }
    }
    m_pDbModelDb->UpdateChanges();
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func   HandlerDevCmdLstDel
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdLstDel(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevLstDel> jsonDevLstDel =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevLstDel>();

    if (!jsonDevLstDel->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    Vector<JsonDevLstDel::Device_t> lstDevDel = jsonDevLstDel->LstDev();

    for (u32_t i = 0; i < lstDevDel.size(); i++) {
        DbDevice deviceFind =
        m_pDbModelDb->Find<DbDeviceItem>().Where("DeviceId = ?").Bind(lstDevDel[i].devid).
                                           Where( "TypeNet = ?").Bind(lstDevDel[i].netwk).
                                           Where("Position = ?").Bind(lstDevDel[i].order);
        if (deviceFind.get() != NULL) {
            deviceFind.Remove();
        }
    }
    m_pDbModelDb->UpdateChanges();
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func   HandlerDevCmdReset
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdReset(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevReset> jsonDeviceReset =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevReset>();

    if (!jsonDeviceReset->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    JsonMessagePtr<JsonDevReset> jsonReset =
    m_pJsonSendDevSession->GetJsonMapping<JsonDevReset>();

    JsonCommand_p pJsonCmdZwReset = jsonReset->CreateJsonCommand();
    pJsonCmdZwReset->SetDesFlag(JsonCommand::Flag::Zwave);
    PushJsonCommand(pJsonCmdZwReset);

    JsonCommand_p pJsonCmdZbReset = jsonReset->CreateJsonCommand();
    pJsonCmdZbReset->SetDesFlag(JsonCommand::Flag::Zigbee);
    PushJsonCommand(pJsonCmdZbReset);
}

/**
 * @func   HandlerDevCmdResetRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdResetRes(
    JsonCommand_p pJsonCommand
) {
    pJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pJsonCommand);
}

/**
 * @func   HandlerDevCmdRestart
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdRestart(
    JsonCommand_p pJsonCommand
) {
    JsonMessagePtr<JsonDevRestart> jsonDeviceReset =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevRestart>();

    if (!jsonDeviceReset->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    JsonMessagePtr<JsonDevReset> jsonRestart =
    m_pJsonSendDevSession->GetJsonMapping<JsonDevReset>();

    JsonCommand_p pJsonCmdZwRestart = jsonRestart->CreateJsonCommand();
    pJsonCmdZwRestart->SetDesFlag(JsonCommand::Flag::Zwave);
    PushJsonCommand(pJsonCmdZwRestart);

    JsonCommand_p pJsonCmdZbRestart = jsonRestart->CreateJsonCommand();
    pJsonCmdZbRestart->SetDesFlag(JsonCommand::Flag::Zigbee);
    PushJsonCommand(pJsonCmdZbRestart);
}

/**
 * @func   HandlerDevCmdInfo
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdInfo(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerDevCmdLstRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdLstRes(
    JsonCommand_p pJsonCommand
) {

}

/**
 * @func   HandlerDevCmdLstAddRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdLstAddRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerDevCmdLstDelRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdLstDelRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerDevCmdSyncRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdSyncRes(
    JsonCommand_p pJsonCommand
) {
}

/**
 * @func   HandlerDevCmdSyncRes
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
DevManager::HandlerDevCmdAuthReq(
    JsonCommand_p pJsonCommand
) {
    pJsonCommand->SetDesFlag(JsonCommand::Flag::Zigbee);
    PushJsonCommand(pJsonCommand);
}
