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
#include <JsonIrLearn.hpp>
#include <JsonIrSet.hpp>
#include <JsonIrDel.hpp>
#include <JsonIrEna.hpp>
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
            DeviceLogic_t mapDeviceNwk = ZbZdoCmd::GetInstance()->GetDeviceLogic();
            if(mapDeviceNwk.size() > 0) {
                for(DeviceLogic_t::iterator_t it = mapDeviceNwk.begin(); it != mapDeviceNwk.end(); it++) {
                    ZbZdoCmd::GetInstance()->LeaveRequest(it->first);
                }
            } else {
                Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("ParentID=?").Bind(0);
                for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                    ZbZdoCmd::GetInstance()->LeaveRequest((*it)->Network.GetValue());
                }
            }

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

                    case LUMI_DEVICE_DOOR:
                    case LUMI_DEVICE_PIR:
                    case LUMI_DEVICE_TEMPERATURE:
                    case LUMI_DEVICE_HUMIDITY:
                    case LUMI_DEVICE_ILLUMINANCE:
                        // Read-Only devices
                        break;

                    case LUMI_DEVICE_RGB:
                        ForwardSetValueToRGB(device, vZbSet[i].val);
                        break;

                    case LUMI_DEVICE_DAIKIN:
                        ForwardSetValueToDaikin(device, vZbSet[i].val);
                        break;

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

                        break;

                    default:
                        break;
                }
            }

        }
            break;

        case ZbMessage::Command::IrLearn: {
            JsonIrLearn_p pJsonIrLearn = (JsonIrLearn_p) pZbMessage->GetJsonMessageObject();
            IrLearn_t sIrLearn = pJsonIrLearn->Return();
            Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(sIrLearn.devid).Bind(sIrLearn.ord);
            if(device.Modify() == NULL) { break; }

            if (sIrLearn.act == 0) {
                if(device->State == IRValue::IR_Idle) {
                    ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Learn);
                } else {
                    ZbSocketCmd::GetInstance()->SendIrRes(device, 8);
                }
            } else if(sIrLearn.act == 1) {
                ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Stop);
            }
        }
            break;

        case ZbMessage::Command::IrSet: {
            JsonIrSet_p pJsonIrSet = (JsonIrSet_p) pZbMessage->GetJsonMessageObject();
            IrSet_t sIrSet = pJsonIrSet->Return();
            Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(sIrSet.devid).Bind(sIrSet.ord);
            if(device.Modify() == NULL) { break; }

            Device_t ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Model=?").Bind(sIrSet.irid).Bind(String("IR-CMD"));
            if(ircmd.Modify() != NULL) {
                if(ircmd->Type.GetValue() == 1)
                    ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Active, sIrSet.irid);
                else
                    ZbSocketCmd::GetInstance()->SendIrRes(device, 5, sIrSet.irid);

            } else {
                ZbSocketCmd::GetInstance()->SendIrRes(device, 5, sIrSet.irid);
            }
        }
            break;

        case ZbMessage::Command::IrDel: {
            JsonIrDel_p pJsonIrDel = (JsonIrDel_p) pZbMessage->GetJsonMessageObject();
            IrDel_t sIrDel = pJsonIrDel->Return();
            Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(sIrDel.devid).Bind(sIrDel.ord);
            if(device.Modify() == NULL) { break; }
            ZbZclCmd::GetInstance()->SetIR(pZbMessage, device, IrCommand::IRCMD_Delete);
        }
            break;

        case ZbMessage::Command::IrEna: {
            JsonIrEna_p pJsonIrEna = (JsonIrEna_p) pZbMessage->GetJsonMessageObject();
            IrEna_t sIrEna = pJsonIrEna->Return();
            Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(sIrEna.devid).Bind(sIrEna.ord);
            if(device.Modify() == NULL) { break; }

            Device_t ircmd = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Model=?").Bind(sIrEna.irid).Bind(String("IR-CMD"));
            if(ircmd.Modify() != NULL) {
                ircmd.Modify()->Type = 0;
                s_pZbModel->Add(ircmd);
                s_pZbModel->UpdateChanges();
            } else {
                ZbSocketCmd::GetInstance()->SendIrRes(device, 5, sIrEna.irid);
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
    Devices_t devices = s_pZbModel->Find<ZbDeviceDb>();
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);
        if(temp->Model.GetValue() != String("IR-CMD")) {
            if(temp->RealType > 0) {
                ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(temp, DeviceInfo::DI_State);
                //Info req from server???
            }
        }
        temp.Modify()->GenerateDeviceInfo();
    }

}
