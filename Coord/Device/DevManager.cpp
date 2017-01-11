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

#include "LogCommand.hpp"
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

    Vector<JsonDevSet::Device_t> lstDevSet = jsonDeviceSet->LstDev();
    Vector<JsonDevSet::Device_t> lstZwDevice;
    Vector<JsonDevSet::Device_t> lstZbDevice;

    for (u32_t i = 0; i < lstDevSet.size(); i++) {
        if (lstDevSet[i].netwk == NetWk::Zwave) { // Zwave Device
            JsonDevSet::Device_t zwDevice;
            zwDevice.devid = lstDevSet[i].devid;
            zwDevice.order = lstDevSet[i].order;
            zwDevice.type  = lstDevSet[i].type;
            zwDevice.value = lstDevSet[i].value;
            zwDevice.netwk = lstDevSet[i].netwk;
            lstZwDevice.push_back(zwDevice);
        } else if (lstDevSet[i].netwk == NetWk::Zigbee) { // Zigbee Device
            JsonDevSet::Device_t zbDevice;
            zbDevice.devid = lstDevSet[i].devid;
            zbDevice.order = lstDevSet[i].order;
            zbDevice.type  = lstDevSet[i].type;
            zbDevice.value = lstDevSet[i].value;
            zbDevice.netwk = lstDevSet[i].netwk;
            lstZbDevice.push_back(zbDevice);
        } else if (lstDevSet[i].netwk == NetWk::Wifi) { // Wifi Device

        } else if (lstDevSet[i].netwk == NetWk::Bluetooth) { // Bluetooth Device

        }
    }

    if (lstZwDevice.size() > 0) {
        JsonMessagePtr<JsonDevSet> jsonZwDeviceSet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevSet>();
        JsonCommand_p pZwJsonCommand =
        jsonZwDeviceSet->CreateJsonCommand(lstZwDevice);
        pZwJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
        PushJsonCommand(pZwJsonCommand);
    }

    if (lstZbDevice.size() > 0) {
        JsonMessagePtr<JsonDevSet> jsonZbDeviceSet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevSet>();
        JsonCommand_p pZbJsonCommand =
        jsonZbDeviceSet->CreateJsonCommand(lstZbDevice);
        pZbJsonCommand->SetDesFlag(JsonCommand::Flag::Zigbee);
        PushJsonCommand(pZbJsonCommand);
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
    JsonMessagePtr<JsonDevGet> jsonDeviceGet =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevGet>();

    if (!jsonDeviceGet->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    Vector<JsonDevGet::Device_t> lstDevGet = jsonDeviceGet->LstDev();
    Vector<JsonDevGet::Device_t> lstZwDevice;
    Vector<JsonDevGet::Device_t> lstZbDevice;

    for (u32_t i = 0; i < lstDevGet.size(); i++) {
        if (lstDevGet[i].netwk == NetWk::Zwave) { // Zwave Device
            JsonDevGet::Device_t zwDevice;
            zwDevice.devid = lstDevGet[i].devid;
            zwDevice.order = lstDevGet[i].order;
            zwDevice.netwk = lstDevGet[i].netwk;
            zwDevice.type  = lstDevGet[i].type;
            lstZwDevice.push_back(zwDevice);
        } else if (lstDevGet[i].netwk == 1) { // Zigbee Device
            JsonDevGet::Device_t zbDevice;
            zbDevice.devid = lstDevGet[i].devid;
            zbDevice.order = lstDevGet[i].order;
            zbDevice.netwk = lstDevGet[i].netwk;
            zbDevice.type  = lstDevGet[i].type;
            lstZbDevice.push_back(zbDevice);
        } else if (lstDevGet[i].netwk == 2) { // Wifi Device

        } else if (lstDevGet[i].netwk == 3) { // Bluetooth Device

        }
    }

    if (lstZwDevice.size() > 0) {
        JsonMessagePtr<JsonDevGet> jsonZwGet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevGet>();
        JsonCommand_p pZwJsonCommand =
        jsonZwGet->CreateJsonCommand(lstZwDevice);
        pZwJsonCommand->SetDesFlag(JsonCommand::Flag::Zwave);
        PushJsonCommand(pZwJsonCommand);
    }

    if (lstZbDevice.size() > 0) {
        JsonMessagePtr<JsonDevGet> jsonZbGet =
        m_pJsonSendDevSession->GetJsonMapping<JsonDevGet>();
        JsonCommand_p pZbJsonCommand =
        jsonZbGet->CreateJsonCommand(lstZbDevice);
        pZbJsonCommand->SetDesFlag(JsonCommand::Flag::Zigbee);
        PushJsonCommand(pZbJsonCommand);
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
    JsonMessagePtr<JsonDevStt> jsonDevStt =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevStt>();

    if (!jsonDevStt->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }
    // Send to Server
    JsonCommand_p pNwJsonCommand = new JsonCommand(pJsonCommand);
    pNwJsonCommand->SetDesFlag(JsonCommand::Flag::NetWork);
    PushJsonCommand(pNwJsonCommand);
    // Send to Rule module
    JsonCommand_p pRuJsonCommand = new JsonCommand(pJsonCommand);
    pRuJsonCommand->SetDesFlag(JsonCommand::Flag::Rule);
    PushJsonCommand(pRuJsonCommand);
    // Store Value
    Vector<JsonDevStt::Device_t> lstDevStt = jsonDevStt->LstDev();
    for (u32_t i = 0; i < lstDevStt.size(); i++) {
        String strKey("dev_");
        strKey += std::to_string(lstDevStt[i].devid).c_str();
        strKey += "type_";
        strKey += std::to_string(lstDevStt[i].type ).c_str();
        strKey += "ord_";
        strKey += std::to_string(lstDevStt[i].order).c_str();
        strKey += "net_";
        strKey += std::to_string(lstDevStt[i].netwk).c_str();

        String strVal(lstDevStt[i].value.toStyledString().c_str());
        strVal.erase(std::remove(strVal.begin(), strVal.end(), ENDLN), strVal.end()); //remove_char(ENDLN);
        strVal.erase(std::remove(strVal.begin(), strVal.end(), SPACE), strVal.end()); //remove_char(SPACE);
        m_StoreValue[strKey] = strVal;
    }
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
    JsonMessagePtr<JsonDevAdd> jsonDevAdd =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevAdd>();

    if (!jsonDevAdd->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    i8_t ibAction = jsonDevAdd->Act();

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
 * @brief  Process command dev=reset{...}
 * @param[in] JsonCommand Object
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
    JsonMessagePtr<JsonDevRestart> jsonDeviceRestart =
    m_pJsonRecvDevSession->GetJsonMapping<JsonDevRestart>();

    if (!jsonDeviceRestart->ParseJsonCommand(pJsonCommand)) {
        delete pJsonCommand;
        pJsonCommand = NULL;
        return;
    }

    delete pJsonCommand;
    pJsonCommand = NULL;

    JsonMessagePtr<JsonDevRestart> jsonRestart =
    m_pJsonSendDevSession->GetJsonMapping<JsonDevRestart>();

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
