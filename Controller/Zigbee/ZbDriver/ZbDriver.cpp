/*
 * ZbDriver.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <debug.hpp>
#include <LogPlus.hpp>
#include <ZbSerialAPI.hpp>
#include <Functor.hpp>
#include <zcl_ha.hpp>
#include <zcl_lumi.hpp>
#include <ZbSocketCmd.hpp>
#include <ZbDeviceDb.hpp>
#include <JsonDevSet.hpp>
#include <JsonDevGet.hpp>
#include <ZbConvertValueTo.hpp>
#include <DeviceInfo.hpp>

#include <ZbDriver.hpp>

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
Packet_p
ZbDriver::ProcSendMessage(
    void_p pInBuffer,
    u32_t dwMsecTimeout,
    bool_t boReturned
) {
    ZbMessage_p pZbMessage = (ZbMessage_p) pInBuffer;

    switch (pZbMessage->GetZbCommad()) {
        case ZbMessage::Command::AddDevice:
            ZbBasicCmd::s_pInstance->JoinNwkAllow(pZbMessage);
            break;

        case ZbMessage::Command::RemoveDevice:
        case ZbMessage::Command::ResetReq: {
            ZbSocketCmd::GetInstance()->SendResetRes(0);
            DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
            if(mapDeviceNwk.size() > 0) {
                for(DeviceLogic_t::iterator_t it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
                    ZbZdoCmd::GetInstance()->LeaveRequest(it->first);
                }
            }

        }
            break;

        case ZbMessage::Command::SetDevice: {
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
                    case LUMI_DEVICE_CURTAIN:
                        ForwardSetValueToDimmerCurtain(pZbMessage, device, vecLstDev[i].value);
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

                    case LUMI_DEVICE_DOOR:
                    case LUMI_DEVICE_PIR:
                    case LUMI_DEVICE_TEMPERATURE:
                    case LUMI_DEVICE_HUMIDITY:
                    case LUMI_DEVICE_ILLUMINANCE:
                        // Read-Only devices
                    default:
                        break;
                }
            }
            pJsonDevSet->Refresh();
        }
            break;

        case ZbMessage::Command::GetDevice: {
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
            break;

        case ZbMessage::Command::SendData:
        default:
            break;
    }

    delete pZbMessage;
    return NULL;
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
void_t
ZbDriver::Init() {
    m_pZbBasicCmd->NwkInfoReq();

    Controllers_t controllers = s_pZbModel->Find<ZbControllerDb>();
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);

        //Load endpoints map
        bool boCheck = false;
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTotalEP++;
        ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byEPCount =
                ZbZdoCmd::s_mapEPInfor[temp->Network.GetValue()].byTotalEP;
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
