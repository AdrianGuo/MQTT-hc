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

    case ZbMessage::Command::SetDevice: {
        JsonZbSet_p pJsonZbSet = (JsonZbSet_p) pZbMessage->GetJsonMessageObject();
        Vector<ZbSet_t> vZbSet = pJsonZbSet->Return();
        for(int_t i = 0; i < vZbSet.size(); i++) {
            Device_t device = s_pZbModel->Find<ZbDeviceDb>().Where("DeviceID=? AND Enpoint=?").Bind(vZbSet[i].devid).Bind(vZbSet[i].order);
            if(device.Modify() == NULL) { continue; }

        }

    }
        break;

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
