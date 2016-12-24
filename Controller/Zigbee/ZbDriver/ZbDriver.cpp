/*
 * ZbDriver.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

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
#include "JsonAuthReq.hpp"

#include "ZbDriver.hpp"

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

    m_pZbBasicCmd     = ZbBasicCmd::GetInstance();
    m_pZbZdoCmd       = ZbZdoCmd::GetInstance();
    m_pZbZclCmd       = ZbZclCmd::GetInstance();
    m_pZbZclGlobalCmd = ZbZclGlobalCmd::GetInstance();

    m_pZbCtrllerFunctor = NULL;
    m_driverSendFunctor = makeFunctor((DriverFunctor_p) NULL, *this,
            &ZbDriver::ProcRecvMessage);
    SendDriverFunctor();

    s_pZbModel = ZbModelDb::CreateModel("zigbee.db");

    RegisterProcess(ZbMessage::Command::AddDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdAdd));
    RegisterProcess(ZbMessage::Command::RemoveDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdDel));
    RegisterProcess(ZbMessage::Command::SetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdSet));
    RegisterProcess(ZbMessage::Command::GetDevice, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdGet));
    RegisterProcess(ZbMessage::Command::InfoReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdInfo));
    RegisterProcess(ZbMessage::Command::ResetReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdReset));
    RegisterProcess(ZbMessage::Command::RestartReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdRestart));
    RegisterProcess(ZbMessage::Command::AuthReq, makeFunctor((ProcZbCmdFunctor_p) NULL, *this, &ZbDriver::ProcCmdAuth));
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
    delete s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::RegisterProcess(
    ZbMessage::Command ZbCommand,
    ProcZbCmdFunctor_t funcTor
) {
    m_mapProcFunctor[ZbCommand] = funcTor;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcSendMessage(
    ZbMessage_p pZbMessage
) {
    ZbMessage::Command ZbCommand = pZbMessage->GetZbCommad();
    MapProcFunctor::const_iterator_t it = m_mapProcFunctor.find(ZbCommand);
    if (it != m_mapProcFunctor.end()) {
        m_mapProcFunctor[ZbCommand](pZbMessage);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcRecvMessage(
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
    m_SZbSerial.RecvFunctor(&m_driverSendFunctor);
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
    if(pZbPacket != NULL)
        m_SZbSerial.PushZbPacket(pZbPacket);
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
Queue<ZbMessage_p>&
ZbDriver::GetSendZbMsgQueue() {
    return m_queSendZbMsg;
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdAdd(
    ZbMessage_p pZbMessage
) {
    JsonDevAdd_p pJsonDevAdd = (JsonDevAdd_p) pZbMessage->GetJsonMessageObject();

    u8_t byTime = 0x00;
    if(pJsonDevAdd->Act() == 0) {
        byTime = 0xFE;
    } else if(pJsonDevAdd->Act() == 1) {
        byTime = 0x00;
    }
    pZbMessage->Push(byTime);
    ZbBasicCmd::s_pInstance->JoinNwkAllow(pZbMessage);
}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdReset(
    ZbMessage_p pZbMessage
) {
    ZbSocketCmd::GetInstance()->SendResetRes(0);
    DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
    if(mapDeviceNwk.size() > 0) {
        for(DeviceLogic_t::iterator_t it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
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
    ZbMessage_p pZbMessage
) {
    ZbSocketCmd::GetInstance()->SendResetRes(0);
    DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
    if(mapDeviceNwk.size() > 0) {
        for(DeviceLogic_t::iterator_t it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
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
    ZbMessage_p pZbMessage
) {
    JsonDevSet_p pJsonDevSet = (JsonDevSet_p) pZbMessage->GetJsonMessageObject();
    Vector<JsonDevSet::Device_t> vecLstDev = pJsonDevSet->LstDev();
    for(int_t i = 0; i < (int_t) vecLstDev.size(); i++) {
        Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Endpoint=?").Bind(vecLstDev[i].devid).Bind(vecLstDev[i].order);
        if(device.Modify() == NULL) { continue; }
        device.Modify()->OwnersReq.push(pZbMessage->GetClientId());
        switch (device->RealType) {
            case LUMI_DEVICE_SWITCH:
            case LUMI_DEVICE_INPUT:
                ForwardSetValueToDevice(pZbMessage, device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_DIMMER:
                ForwardSetValueToDimmer(pZbMessage, device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_CURTAIN:
                ForwardSetValueToCurtain(pZbMessage, device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_FAN:
                ForwardSetValueToFan(pZbMessage, device, vecLstDev[i].value);
                break;

            case LUMI_DEVICE_IR:
                ForwardSetValueToIr(pZbMessage, device, vecLstDev[i].value);
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
    pJsonDevSet->Refresh();
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdGet(
    ZbMessage_p pZbMessage
) {
    JsonDevGet_p pJsonDevGet = (JsonDevGet_p) pZbMessage->GetJsonMessageObject();
    Vector<JsonDevGet::Device_t> vecLstDev = pJsonDevGet->LstDev();
    for(int_t i = 0; i < (int_t) vecLstDev.size(); i++) {
        Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Endpoint=?").Bind(vecLstDev[i].devid).Bind(vecLstDev[i].order);
        if(device.Modify() == NULL) { continue; }
        device.Modify()->OwnersReq.push(pZbMessage->GetClientId());
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
                ForwardGetRequestToDevice(device);
                break;

            case LUMI_DEVICE_RGB:
            case LUMI_DEVICE_DAIKIN:
                ForwardGetRequestsToDevice(device);
                break;

            case LUMI_DEVICE_IR:
                pZbMessage->SetCmdID(ZCL_CMD_REQ);
                ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_State);
                break;

            default:
                break;
        }
    }
    pJsonDevGet->Refresh();
}


/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdInfo(
    ZbMessage_p pZbMessage
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
    ZbMessage_p pZbMessage
) {

}

/**
 * @func   None
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::ProcCmdAuth(
    ZbMessage_p pZbMessage
) {
    JsonAuthReq_p pJsonAuthReq = (JsonAuthReq_p) pZbMessage->GetJsonMessageObject();
    JsonAuthReq::Device_t dev = pJsonAuthReq->Dev();
    NetDevice_t netDevice = s_pZbModel->Find<NetDeviceDb>().Where("MAC=?").Bind(dev.mac);
    if(netDevice.Modify() == NULL) {
        netDevice = ZbDriver::s_pZbModel->Add(new NetDeviceDb());
        for(int_t i = 1; i < std::numeric_limits<int_t>::max(); i++) {
            NetDevice_t tmp = s_pZbModel->Find<NetDeviceDb>().Where("DeviceID=?").Bind(i);
            if(tmp.Modify() == NULL) {
                netDevice.Modify()->DeviceID = i;
                break;
            }
        }
        netDevice.Modify()->MAC = dev.mac;
        netDevice.Modify()->Type = dev.type;
        netDevice.Modify()->NetID = pZbMessage->GetClientId();

        Vector<JsonDevLstAdd::Device_t> vecLstDev;
        JsonDevLstAdd::Device_t lstDev;
        lstDev.devid = netDevice.Modify()->DeviceID.GetValue();
        lstDev.mac = dev.mac;
        lstDev.type  = dev.type;
        lstDev.order = 1;
        lstDev.netwk = NET_WIFI;
        vecLstDev.push_back(lstDev);
        ZbSocketCmd::GetInstance()->SendLstAdd(vecLstDev);
        ZbSocketCmd::GetInstance()->SendAuthRes(netDevice);
    } else {
        netDevice.Modify()->NetID = pZbMessage->GetClientId();
    }
    ZbDriver::s_pZbModel->Add(netDevice);
    ZbDriver::s_pZbModel->UpdateChanges();

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::Init(
    bool req
) {
    if(req) {
        m_pZbBasicCmd->NwkInfoReq();
        while(ZbBasicCmd::IsNetworkAvail == FALSE);
    }

    Controllers_t controllers = s_pZbModel->Find<ZbControllerDb>();
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);

        //Load endpoints map
        bool boCheck = false;
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTotalEP++;
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byEPCount =
                ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTotalEP;
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].MAC = temp->MAC.GetValue();
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].IsDone = TRUE;
        for(Map<u16_t, u16_t>::const_iterator_t it2 = ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].mapType.begin();
                it2 != ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].mapType.end(); it2++) {
            if(temp->Type.GetValue() == it2->second) {
                boCheck = true;
                break;
            }
        }
        if(!boCheck) {
            ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].mapType[ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTypeCount] = temp->Type.GetValue();
            ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTypeCount++;
        }


        //Generate device info and request state.
        if(temp.Modify()->IsInterested()) {
            temp.Modify()->GenerateDeviceInfo();

            if (req) {
                Json::Value jsonVal, jsonDev;
                jsonDev["devid"] = std::to_string(temp->DeviceID.GetValue());
                jsonDev["ord"] = std::to_string(temp->Endpoint.GetValue());
                jsonDev["net"] = std::to_string(1);
                jsonDev["type"] = std::to_string(temp->RealType);
                jsonVal["dev"].append(jsonDev);
                JsonCommand_p pJsonCommand = new JsonCommand(String("dev"), String("get"));
                pJsonCommand->SetJsonObject(jsonVal);
                JsonDevGet_p pJsonDevGet = new JsonDevGet();
                pJsonDevGet->ParseJsonCommand(pJsonCommand);
                ZbMessage_p pZbMessage = new ZbMessage(pJsonDevGet, ZbMessage::Command::GetDevice);
                pZbMessage->SetCmdID(ZCL_CMD_REQ);
                ProcSendMessage(pZbMessage);
                pZbMessage = NULL;
                delete pJsonCommand;
                delete pJsonDevGet;
            }

        }
    }
}
