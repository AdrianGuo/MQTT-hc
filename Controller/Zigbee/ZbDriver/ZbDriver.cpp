/*
 * ZbDriver.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <unistd.h>
#include "debug.hpp"
#include "LogPlus.hpp"
#include "ZbSerialAPI.hpp"
#include "Functor.hpp"
#include "zcl_ha.hpp"
#include "zcl_lumi.hpp"
#include "ZbSocketCmd.hpp"
#include "ZbDeviceDb.hpp"
#include "ZbConvertValueTo.hpp"
#include "DeviceInfo.hpp"
#include "JsonDevAdd.hpp"
#include "JsonDevDel.hpp"
#include "JsonDevSet.hpp"
#include "JsonDevGet.hpp"
#include "JsonDevInfo.hpp"
#include "JsonDevReset.hpp"
#include "JsonDevRestart.hpp"
#include "IO.hpp"

#include "LogPlus.hpp"

#include "ZbDriver.hpp"
#include "SMQTT.hpp"

#define REQUEST_INTERVAL_INPUT      (5)
#define REQUEST_INTERVAL            (6*60)

ZbDriver* ZbDriver::s_pInstance = NULL;
ZbModelDb_p ZbDriver::s_pZbModel = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDriver::ZbDriver(
    const_char_p chPortname
) : m_SZbSerial(chPortname) {
	m_pLocker 		  = new Locker();
    m_pZbBasicCmd     = ZbBasicCmd::GetInstance();
    m_pZbZdoCmd       = ZbZdoCmd::GetInstance();
    m_pZbZclCmd       = ZbZclCmd::GetInstance();
    m_pZbZclGlobalCmd = ZbZclGlobalCmd::GetInstance();

    m_pZbCtrllerFunctor = NULL;
    m_SerialRecvFunctor = makeFunctor((DriverFunctor_p) NULL, *this, &ZbDriver::ProcDevRecvMsg);
    SendDriverFunctor();

    s_pZbModel = ZbModelDb::CreateModel("zigbee.db");

    m_pTimer = RTimer::getTimerInstance();
    m_RequestFunctor = makeFunctor((TimerFunctor_p) NULL, *this, &ZbDriver::HandleRequest);
    m_iRequest = -1;
    m_idwCheckTime = 0;

    m_pJsonRecvSession = JsonRecvZigbeeSession::CreateSession();
    m_pJsonRecvSession->MapJsonMessage<JsonDevAdd>(JsonDevAdd::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevDel>(JsonDevDel::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevSet>(JsonDevSet::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevGet>(JsonDevGet::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevInfo>(JsonDevInfo::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevReset>(JsonDevReset::GetStrCmd());
    m_pJsonRecvSession->MapJsonMessage<JsonDevRestart>(JsonDevRestart::GetStrCmd());

    RegisterProcess(JsonDevAdd::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdAdd));
    RegisterProcess(JsonDevDel::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdDel));
    RegisterProcess(JsonDevSet::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdSet));
    RegisterProcess(JsonDevGet::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdGet));
    RegisterProcess(JsonDevInfo::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdInfo));
    RegisterProcess(JsonDevReset::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdReset));
    RegisterProcess(JsonDevRestart::GetStrCmd(), makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdRestart));
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDriver* ZbDriver::GetInstance(const_char_p chPortname) {
    if (s_pInstance == NULL) {
        s_pInstance = new ZbDriver(chPortname);
    }
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDriver::~ZbDriver() {
    delete m_pZbBasicCmd;
    delete m_pZbZdoCmd;
    delete m_pZbZclCmd;
    delete m_pZbZclGlobalCmd;
    delete m_pZbCtrllerFunctor;
    delete s_pZbModel;
    delete m_pLocker;;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::RegisterProcess(
    String strCommand,
    ProcZbCmdFunctor_t funcTor
) {
    m_mapProcFunctor[strCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcSerRecvMsg(
    JsonCommand_p pJsonCommand
) {
    String strCommand = pJsonCommand->GetFullCommand();
    MapProcFunctor::const_iterator it = m_mapProcFunctor.find(strCommand);
    if (it != m_mapProcFunctor.end()) {
        m_mapProcFunctor[strCommand](pJsonCommand);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcDevRecvMsg(
    void_p pInBuffer
) {
    ZbPacket_p pZbPacket = (ZbPacket_p) pInBuffer;
    switch (pZbPacket->GetCmdID()) {
        case NWK_JOIN_INFO_RSP:
            m_pZbBasicCmd->JoinNwkInfoRsp(pZbPacket);
            break;

        case NWK_INFO_RSP:
            m_pZbBasicCmd->NwkInfoRsp(pZbPacket);
            break;

        case ZCL_CMD_RSP:
            m_pZbZclCmd->ProcRecvMessage(pInBuffer);
            break;

        case ZCL_GLOBAL_CMD_RSP:
            m_pZbZclGlobalCmd->ProcRecvMessage(pInBuffer);
            break;

        case ZDO_CMD_RSP:
            m_pZbZdoCmd->ProcRecvMessage(pInBuffer);
            break;

        default:
            LOG_WARN("COMMAND NOT FOUND!");
            break;
    }

    delete pZbPacket;
}

void_t ZbDriver::SendDriverFunctor() {
    m_SZbSerial.RecvFunctor(&m_SerialRecvFunctor);
}

void_t ZbDriver::ZbDriverRecvFunctor(ZbCtrllerFunctor_p pZbCtrllerFunctor) {
    if (pZbCtrllerFunctor != NULL)
        m_pZbCtrllerFunctor = pZbCtrllerFunctor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDriver::Start() {
    return m_SZbSerial.Start();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDriver::Connect() {
    return m_SZbSerial.Connect();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDriver::Close() {
    return m_SZbSerial.Close();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::SendZbPacket(
    ZbPacket_p pZbPacket
) {
    if(pZbPacket != NULL) {
    	m_pLocker->Lock();
        m_SZbSerial.PushZbPacket(pZbPacket);
        m_pLocker->UnLock();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::SendJsonMessage(
    EvAct evAction,
    void_p pBufffer
) {
    m_pZbCtrllerFunctor->operator ()(evAction, pBufffer);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Queue<JsonCommand_p>&
ZbDriver::GetSerRecvMsgQueue() {
    return m_queRecvMsg;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdAdd(
    JsonCommand_p pJsonCommand
) {
	JsonMessagePtr<JsonDevAdd> jsonDevAdd = m_pJsonRecvSession->GetJsonMapping<JsonDevAdd>();
	if (!jsonDevAdd->ParseJsonCommand(pJsonCommand)) return;

	i8_t act = jsonDevAdd->Act();
    if(act == 0) {
    	Notify(Allowed);
        m_pZbBasicCmd->JoinNwkAllow((u8_t) 0XFF);
    } else if(act == 1) {
    	Notify(Backup);
    	m_pZbBasicCmd->JoinNwkAllow((u8_t) 0X00);
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdReset(
    JsonCommand_p pJsonCommand
) {
//    ZbSocketCmd::GetInstance()->SendResetRes(0);
    DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
    if(mapDeviceNwk.size() > 0) {
        for(DeviceLogic_t::iterator it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
            ZbZdoCmd::GetInstance()->LeaveRequest(it->first);
        }
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdDel(
    JsonCommand_p pJsonCommand
) {
    ZbSocketCmd::GetInstance()->SendResetRes(0);
    DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
    if(mapDeviceNwk.size() > 0) {
        for(DeviceLogic_t::iterator it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
            ZbZdoCmd::GetInstance()->LeaveRequest(it->first);
        }
    }
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdSet(
    JsonCommand_p pJsonCommand
) {
	Notify(AppSig);
	JsonMessagePtr<JsonDevSet> jsonDevSet = m_pJsonRecvSession->GetJsonMapping<JsonDevSet>();
	if (!jsonDevSet->ParseJsonCommand(pJsonCommand)) return;
    Vector<JsonDevSet::Device_t> vecLstDev = jsonDevSet->LstDev();
    for(int_t i = 0; i < (int_t) vecLstDev.size(); i++) {
        Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Endpoint=?").Bind(vecLstDev[i].devid).Bind(vecLstDev[i].order);
        if(device.get() == NULL) { continue; }
        device.Modify()->OwnersReq.push(pJsonCommand->GetClientId());
        switch (device->RealType) {
            case LUMI_DEVICE_SWITCH:
            case LUMI_DEVICE_INPUT:
                ForwardSetValueToDevice(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_DIMMER:
                ForwardSetValueToDimmer(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_CURTAIN:
                ForwardSetValueToCurtain(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_FAN:
                ForwardSetValueToFan(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_IR:
                ForwardSetValueToIr(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_RGB:
                ForwardSetValueToRGB(device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_DAIKIN:
                ForwardSetValueToDaikin(device, vecLstDev[i].value);
                break;

            // Read-Only devices
            case LUMI_DEVICE_DOOR:
            case LUMI_DEVICE_PIR:
            case LUMI_DEVICE_TEMPERATURE:
            case LUMI_DEVICE_HUMIDITY:
            case LUMI_DEVICE_ILLUMINANCE:
            default:
                break;
        }
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdGet(
    JsonCommand_p pJsonCommand
) {
	JsonMessagePtr<JsonDevGet> jsonDevGet = m_pJsonRecvSession->GetJsonMapping<JsonDevGet>();
	if (!jsonDevGet->ParseJsonCommand(pJsonCommand)) return;
    Vector<JsonDevGet::Device_t> vecLstDev = jsonDevGet->LstDev();
    for(int_t i = 0; i < (int_t) vecLstDev.size(); i++) {
        Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Endpoint=?").Bind(vecLstDev[i].devid).Bind(vecLstDev[i].order);
        if(device.get() == NULL) { continue; }
        device.Modify()->OwnersReq.push(pJsonCommand->GetClientId());
        switch (device->RealType) {
            case LUMI_DEVICE_SWITCH:
            case LUMI_DEVICE_INPUT:
            case LUMI_DEVICE_DIMMER:
            case LUMI_DEVICE_CURTAIN:
            case LUMI_DEVICE_FAN:
            case LUMI_DEVICE_DOOR:
            case LUMI_DEVICE_PIR:
            case LUMI_DEVICE_TEMPERATURE:
            case LUMI_DEVICE_HUMIDITY:
            case LUMI_DEVICE_ILLUMINANCE:
            case LUMI_DEVICE_POWER:
                ForwardGetRequestToDevice(device);
                break;

            case LUMI_DEVICE_RGB:
            case LUMI_DEVICE_DAIKIN:
                ForwardGetRequestsToDevice(device);
                break;

            case LUMI_DEVICE_IR:
                m_pZbZclCmd->SetIR(device, IrCommand::IRCMD_State);
                break;

            default:
                break;
        }
    }
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdInfo(
    JsonCommand_p pJsonCommand
) {

}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdRestart(
    JsonCommand_p pJsonCommand
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::Init(
    bool stateReq
) {
    if(stateReq) {
        m_pZbBasicCmd->NwkInfoReq();
//        while(m_pZbBasicCmd->IsNetworkAvail() != TRUE) {
//            LOG_DEBUG("Waiting zb's response...");
//            sleep(1);
//        }
    }
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);
        u16_t wNwk = temp->Network.GetValue();
        //Load endpoints map
        ZbZdoCmd::s_mapEPInfo[wNwk].MAC = temp->MAC.GetValue();
        ZbZdoCmd::s_mapEPInfo[wNwk].byTotalEP++;
        ZbZdoCmd::s_mapEPInfo[wNwk].IsAERequested = TRUE;
        ZbZdoCmd::s_mapEPInfo[wNwk].HasModelInfo = TRUE;
        ZbZdoCmd::s_mapEPInfo[wNwk].HasManufInfo = TRUE;
        ZbZdoCmd::s_mapEPInfo[wNwk].IsDone = TRUE;
        ZbZdoCmd::s_mapEPInfo[wNwk].vEPList.push_back(temp->Endpoint.GetValue());
        ZbZdoCmd::s_mapEPInfo[wNwk].mapType[temp->Endpoint.GetValue()] = temp->Type.GetValue();

        //Generate device info and request state.
        if(temp.Modify()->IsInterested()) {
            temp.Modify()->GenerateDeviceInfo();

//            if (stateReq && temp->RealType != LUMI_DEVICE_IR) {
//                Json::Value jsonVal, jsonDev;
//                jsonDev["devid"] = std::to_string(temp->DeviceID.GetValue());
//                jsonDev["ord"] = std::to_string(temp->Endpoint.GetValue());
//                jsonDev["net"] = std::to_string(1);
//                jsonDev["type"] = std::to_string(temp->RealType);
//                jsonVal["dev"].append(jsonDev);
//                JsonCommand_p pJsonCommand = new JsonCommand(String("dev"), String("get"));
//                pJsonCommand->SetJsonObject(jsonVal);
//                JsonDevGet_p pJsonDevGet = new JsonDevGet();
//                pJsonDevGet->ParseJsonCommand(pJsonCommand);
//                ZbMessage_p pZbMessage = new ZbMessage(pJsonDevGet, ZbMessage::Command::GetDevice);
//                pZbMessage->SetCmdID(ZCL_CMD_REQ);
//                ProcSendMessage(pZbMessage);
//                pZbMessage = NULL;
//                delete pJsonCommand;
//                delete pJsonDevGet;
//            }

        }
    }
    //Start keepalive timer
    m_iRequest = m_pTimer->StartTimer(RTimer::Repeat::Forever, REQUEST_INTERVAL_INPUT, &m_RequestFunctor, NULL);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::HandleRequest(
   void_p pbyBuffer
) {
    LOG_DEBUG("Affirm alive state of devices!");
    m_idwCheckTime += REQUEST_INTERVAL_INPUT;
    if (m_idwCheckTime > REQUEST_INTERVAL)
        m_idwCheckTime = 0;

    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
    Devices_t::const_iterator it;
    for(it = devices.begin(); it != devices.end(); it++) {
        const Device_t& tmp = (*it);
        if(tmp.Modify()->RealType > 0) {
            if ((tmp.Modify()->RealType == LUMI_DEVICE_INPUT) || (m_idwCheckTime ==  0)) {
                if(tmp.Modify()->IsAlive == FALSE) {
                    SMQTT::s_pInstance->Publish(tmp.Modify()->Name.c_str(), -1);
                    LOG_WARN("device %s  not reply", tmp.Modify()->Name.c_str());
                } else {
                    tmp.Modify()->IsAlive = FALSE;
                }
            }
        }
    }
//   LOG_DEBUG("Check alive state of devices!");
    if (m_idwCheckTime % 30 == 0) {
        m_pZbZclGlobalCmd->Broadcast();
    } else {
        for(it = devices.begin(); it != devices.end(); it++) {
            try {
                const Device_t& tmp = (*it);
                if (tmp.Modify()->RealType == LUMI_DEVICE_INPUT) {
                    m_pZbZclGlobalCmd->ReadAttributeRequest(tmp, DI_ZCLVersion);
                }
            }
            catch (...) { LOG_WARN("Exception occurred"); }
        }
    }
}
