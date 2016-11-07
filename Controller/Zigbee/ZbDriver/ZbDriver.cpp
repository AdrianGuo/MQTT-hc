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

    case ZbMessage::Command::RmvDevice: {
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("ParentID=?").Bind(0);
        for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            ZbZdoCmd::s_pInstance->LeaveRequest(pZbMessage, (*it));
        }
    }
        break;

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

//void_t ZbDriver::GetInitialState() {
//    for (Devices::const_iterator it = ZbDevices.begin(); it != ZbDevices.end();
//            it++) {
//        ZbZCLGlobCmd::s_pInstance->ReadAttributeRequest(*it, (*it)->ClusterID,
//                (*it)->AttributeID);
//    }
//}

//void_t ZbDriver::HandleZbAdd(ZbMsg_p pZbMsg, ZbPacket_p pZbPacket) {
//    u8_t time;
//    Json::Value& json = pZbMsg->GetJsonObject();
//    const_char_p act = json["act"].asCString();
//    if (strcmp(act, "0") == 0)
//        time = 0xFE;
//    else
//        time = 0x00;
//    ZbBasicCmd::s_pInstance->JoinNwkAllow(pZbPacket, time);
//}
//
//void_t ZbDriver::HandleDevSet(ZbMsg_p pZbMsg, ZbPacket_p pZbPacket) {
//    Json::Value& json = pZbMsg->GetJsonObject();
//    const Json::Value& devs = json["dev"];
//
//    for (Json::ValueConstIterator it = devs.begin(); it != devs.end(); ++it) {
//        const Json::Value& dev = *it;
//        int_t devID = atoi(dev["devID"].asCString());
//        int_t val = atoi(dev["val"].asCString());
//
//        Devices pDevices =
//                pZbModel->Find<ZbDeviceDb>().Where("DeviceID=?").Bind(devID);
//        if (pDevices.size() == 1) {
//            Devices::const_iterator iter = pDevices.begin();
//            ZbZCLCmd::s_pInstance->SetDevice(pZbPacket, *iter, (u8_t) val);
//        }
//    }
//}
//
//void_t ZbDriver::HandleZbReset(ZbMsg_p pZbMsg, ZbPacket_p pZbPacket) {
//    ZbDeviceManager_p pDevices = ZbDriver::s_pInstance->ZbDevices;
//    ZbControllerManager_p pControllers = ZbDriver::s_pInstance->ZbControllers;
//
//    while (pDevices->GetCountRecords() > 0) {
//        ZbPacket_p pTemp = new ZbPacket(pZbMsg->GetSeqMsg(), pZbMsg->GetCmdID(),
//                pZbMsg->Length());
//        ZbZDOCmd::s_pInstance->LeaveRequest(pTemp, pDevices->GetDeviceByIndex(0));
//        delete pTemp;
//    }
//
//    while (pControllers->GetCountRecords() > 0) {
//        pControllers->DeleteRecord(pControllers->GetControllerByIndex(0));
//    }
//
//    ZbSocketCmd::s_pInstance->Reset(0);
//    ZbBasicCmd::s_pInstance->NwkInit();
//
//    pDevices = NULL;
//    delete pDevices;
//    pControllers = NULL;
//    delete pControllers;
//}
//
//void_t ZbDriver::HandleZbDel(ZbMsg_p pZbMsg, ZbPacket_p pZbPacket) {
//    Json::Value& json = pZbMsg->GetJsonObject();
//    const_char_p act = json["act"].asCString();
//
//    if (strcmp(act, "0") == 0)
//        _act = true;
//    else if (strcmp(act, "1") == 0)
//        _act = false;
//
//    ZbDeviceManager_p pDevices = ZbDriver::s_pInstance->ZbDevices;
//    while (pDevices->GetCountRecords() > 0 && _act) {
//        ZbZDOCmd::s_pInstance->LeaveRequest(pZbPacket,
//                pDevices->GetDeviceByIndex(0));
//    }
//    pDevices = NULL;
//    delete pDevices;
//}

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
    Devices_t devices = s_pZbModel->Find<ZbDeviceDb>().Where("ParentID=?").Bind(0);
    for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        Device_t temp = (*it);
        temp.Modify()->GenerateDeviceInfo();
        if(temp->RealType > 0) {
            ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(temp, DeviceInfo::DI_State);
        }
    }

}
