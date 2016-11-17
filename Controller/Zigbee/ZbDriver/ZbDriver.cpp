/*
 * ZbDriver.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <debug.hpp>
#include <ZbSerialAPI.hpp>
#include <Functor.hpp>
#include <zcl_ha.hpp>
#include <zcl_lumi.hpp>
#include <ZbSocketCmd.hpp>
#include <ZbDeviceDb.hpp>
#include <JsonZbSet.hpp>
#include <JsonZbGet.hpp>
#include <JsonManualRemove.hpp>
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
ZbDriver::ZbDriver(SZbSerial_p pSZbSerial) {
    m_pSZbSerial = pSZbSerial;

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
ZbDriver* ZbDriver::GetInstance(SZbSerial_p pSZbSerial) {
    if (s_pInstance == NULL)
        s_pInstance = new ZbDriver(pSZbSerial);
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbDriver::~ZbDriver() {
    if (m_pSZbSerial != NULL) {
        delete m_pSZbSerial;
        m_pSZbSerial = NULL;
    }
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

        case ZbMessage::Command::RmvDevice:
        case ZbMessage::Command::ResetReq: {
            ZbSocketCmd::GetInstance()->SendResetRes(1);
            Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
//            DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
//            if(mapDeviceNwk.size() > 0) {
//                for(DeviceLogic_t::iterator_t it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
//                    ZbZdoCmd::GetInstance()->LeaveRequest(it->first);
//                }
//            } else {
//                Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();//.Where("DeviceID !=?").Bind("IR-CMD");
                for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                    ZbZdoCmd::GetInstance()->LeaveRequest((*it)->Network.GetValue());
                }
//            }

        }
            break;

        case ZbMessage::Command::ManualRmv: {
            JsonManualRemove_p pJsonManualRemove = (JsonManualRemove_p) pZbMessage->GetJsonMessageObject();
            int_t nwk = pJsonManualRemove->Return();
            ZbZdoCmd::GetInstance()->LeaveRequest((u16_t) nwk);
        }
            break;

        case ZbMessage::Command::SetDevice: {
            JsonZbSet_p pJsonZbSet = (JsonZbSet_p) pZbMessage->GetJsonMessageObject();
            Vector<ZbSet_t> vZbSet = pJsonZbSet->Return();
            for(int_t i = 0; i < (int_t) vZbSet.size(); i++) {

                Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(vZbSet[i].devid).Bind(vZbSet[i].ord);
                if(device.Modify() == NULL) { continue; }
                switch (device->RealType) {
                    case LUMI_DEVICE_SWITCH:
                    case LUMI_DEVICE_INPUT:
                        ForwardSetValueToDevice(pZbMessage, device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_DIMMER:
                    case LUMI_DEVICE_CURTAIN:
                        ForwardSetValueToDimmer(pZbMessage, device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_FAN:
                        ForwardSetValueToFan(pZbMessage, device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_IR:
                        ForwardSetValueToIr(pZbMessage, device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_RGB:
                        ForwardSetValueToRGB(device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_DAIKIN:
                        ForwardSetValueToDaikin(device, vZbSet[i].val);
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

        }
            break;

        case ZbMessage::Command::GetDevice: {
            JsonZbGet_p pJsonZbGet = (JsonZbGet_p) pZbMessage->GetJsonMessageObject();
            Vector<ZbGet_t> vZbGet = pJsonZbGet->Return();
            for(int_t i = 0; i < (int_t) vZbGet.size(); i++) {
                Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(vZbGet[i].devid).Bind(vZbGet[i].ord);
                if(device.Modify() == NULL) { continue; }

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
            DEBUG1("COMMAND NOT FOUND!")
            break;
    }

    delete pZbPacket;
}

void_t ZbDriver::SendDriverFunctor() {
    if (m_pSZbSerial != NULL) {
        m_pSZbSerial->RecvFunctor(&m_driverSendFunctor);
    }
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
    if (m_pSZbSerial != NULL) {
        DEBUG1("START SUCCESS");
        return m_pSZbSerial->Start();
    }
    DEBUG1("START FAIL");
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDriver::Connect() {
    if (m_pSZbSerial != NULL) {
        return m_pSZbSerial->Connect();
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbDriver::Close() {
    if (m_pSZbSerial != NULL) {
        return m_pSZbSerial->Close();
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbDriver::InitDriver() {
    Controllers_t controllers = s_pZbModel->Find<ZbControllerDb>();
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>();
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);
        if(temp.Modify()->IsInterested()) {
        temp.Modify()->GenerateDeviceInfo();

        Json::Value jsonVal;
        jsonVal["devid"] = std::to_string(temp->DeviceID.GetValue());
        jsonVal["ord"] = std::to_string(temp->Endpoint.GetValue());
        JsonCommand_p pJsonCommand = new JsonCommand(String("dev"), String("get"));
        pJsonCommand->SetJsonObject(jsonVal);
        JsonZbGet_p pJsonZbGet = new JsonZbGet();
        pJsonZbGet->ParseJsonCommand(pJsonCommand);
        ZbMessage_p pZbMessage = new ZbMessage(pJsonZbGet, ZbMessage::Command::GetDevice);
        pZbMessage->SetCmdID(ZCL_CMD_REQ);
        ProcSendMessage(pZbMessage);
        pZbMessage = NULL;
        delete pJsonCommand;
        delete pJsonZbGet;
        }
    }

}
