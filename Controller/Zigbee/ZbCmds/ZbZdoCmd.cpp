/*
 * ZbZDOCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <debug.hpp>
#include <zcl.hpp>
#include <zdo.hpp>
#include <zcl_lumi.hpp>
#include <zcl_ha.hpp>
#include <ZbHelper.hpp>
#include <ZbSocketCmd.hpp>

#include <ZbZdoCmd.hpp>

ZbZdoCmd* ZbZdoCmd::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZdoCmd::ZbZdoCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZdoCmd*
ZbZdoCmd::GetInstance() {
    if(s_pInstance == NULL)
        s_pInstance = new ZbZdoCmd();
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZdoCmd::~ZbZdoCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::ProcRecvMessage(
    void_p pInBuffer
) {
    ZbPacket_p pZbPacket = (ZbPacket_p) pInBuffer;
    u8_p pbyBuffer = pZbPacket->GetBuffer();
    u32_t idwLen = pZbPacket->Count() - 4;
    u16_t wNwk = LittleWord(&pbyBuffer);
    u16_t wZDOCmdID = LittleWord(&pbyBuffer);;
    switch (wZDOCmdID) {
        case ZDO_DEVICE_ANNCE:
            DeviceAnnounce(pbyBuffer, idwLen);
            break;

        case ZDO_ACTIVE_EP_RSP:
            ActiveEndpointResponse(pbyBuffer, idwLen);
            break;

        case ZDO_SIMPLE_DESC_RSP:
            NodeDescriptionResponse(pbyBuffer, idwLen);
            break;

        case ZDO_MGMT_LEAVE_RSP:
            LeaveResponse(wNwk, pbyBuffer, idwLen);
            break;

        default:
            DEBUG1("ZDO COMMAND NOT FOUND!");
            break;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::DeviceAnnounce(
        u8_p pbyBuffer, u32_t idwLen
) {
    pbyBuffer       += 2; //Ignore Cmd Payload Length
    u16_t wNwk      = BigWord(&pbyBuffer);
    String MAC      = HexToString(pbyBuffer, 8);
    pbyBuffer       += 8;
    m_mapTemps[wNwk]   = MAC;
    ActiveEndpointRequest(wNwk);
    // ZB Device Type (router, end device - sleepable)!!!
    DEBUG2("Device %d joined.", wNwk);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::ActiveEndpointRequest(
        u16_t wNwk
) {
    ZbPacket_p pZbPacket = new ZbPacket(8);
    pZbPacket->SetCmdID(ZDO_CMD_REQ);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(ZDO_ACTIVE_EP_REQ >> 8);
    pZbPacket->Push(ZDO_ACTIVE_EP_REQ & 0xFF);
    pZbPacket->Push(0x02);
    pZbPacket->Push(0x00);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(wNwk >> 8);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::ActiveEndpointResponse(
    u8_p pbyBuffer,
    u32_t idwLen
){
    pbyBuffer           += 2; //Ignore Cmd Payload Length
    u8_t byStatus       = *pbyBuffer++;
    u16_t wNwk          = BigWord(&pbyBuffer);;
    u8_t byEndpointNo   = *pbyBuffer++;
    u8_t byEndpointList[byEndpointNo];
    if (byEndpointNo > 0) {
        m_mapEPInfor[wNwk].byTotalEP   = byEndpointNo;
        m_mapEPInfor[wNwk].byEPCount   = 0;
        m_mapEPInfor[wNwk].byTypeCount = 0;
        for (int_t i = 0; i < byEndpointNo; i++)
            byEndpointList[i] = *pbyBuffer++;
    }
    switch (byStatus) {
    case ZDO_STATUS_SUCCESS: {
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("MAC=?").Bind(m_mapTemps[wNwk]);
        Controller_t pController      = ZbDriver::s_pZbModel->Find<ZbControllerDb>();
        if (devices.size() == 0) {
            for (int j = 0; j < byEndpointNo; j++) {
                ZbDeviceDb_p pZbDevice = new ZbDeviceDb();
                pZbDevice->DeviceID  = wNwk*1000 + byEndpointList[j];
                pZbDevice->Network   = wNwk;
                pZbDevice->MAC       = m_mapTemps[wNwk];
                pZbDevice->Endpoint  = byEndpointList[j];
                pZbDevice->Controller   = pController; //!!! temporary !!!

                Device_t device = ZbDriver::s_pZbModel->Add(pZbDevice);
                ZbDriver::s_pZbModel->UpdateChanges();
                pZbDevice = NULL;
                delete pZbDevice;

                NodeDescriptionRequest(device);
            }
        } else if (devices.size() > 0) {
            for (int j = 0; j < byEndpointNo; j++) {
                Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("MAC=? AND Endpoint=?").Bind(m_mapTemps[wNwk]).Bind(byEndpointList[j]);
                if(device.Modify() != NULL) {
                    device.Modify()->DeviceID       = wNwk*1000 + byEndpointList[j];
                    device.Modify()->Network        = wNwk;;
                    device.Modify()->Controller     = pController; //!!! temporary !!!

                    ZbDriver::s_pZbModel->Add(device);
                    ZbDriver::s_pZbModel->UpdateChanges();
                    NodeDescriptionRequest(device);
                }
            }
        }
        m_mapTemps.erase(wNwk);
    }
        break;
    case ZDO_STATUS_NO_DESCRIPTOR:
    case ZDO_STATUS_INV_REQUESTTYPE:
    case ZDO_STATUS_DEVICE_NOT_FOUND:
    default:
        DEBUG1("NO DESCRIPTOR|INV REQUEST TYPE|DEVICE NOT FOUND!");
        break;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::NodeDescriptionRequest(
    Device_t device
){
    ZbPacket_p pZbPacket = new ZbPacket(9);
    u16_t wNwk = device->Network.GetValue();
    pZbPacket->SetCmdID(ZDO_CMD_REQ);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(ZDO_SIMPLE_DESC_REQ >> 8);
    pZbPacket->Push(ZDO_SIMPLE_DESC_REQ & 0xFF);
    pZbPacket->Push(0x04);
    pZbPacket->Push(0x00);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(device->Endpoint.GetValue());
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::NodeDescriptionResponse(
    u8_p pbyBuffer,
    u32_t idwLen
){
    pbyBuffer       += 2; //Ignore Cmd Payload Length
    u8_t byStatus   = *pbyBuffer++;
    u16_t wNwk      = BigWord(&pbyBuffer);
    u8_t byLeng     = *pbyBuffer++;
    switch (byStatus) {
    case ZDO_STATUS_SUCCESS: {
        if (byLeng < 5) {
            DEBUG1("NONFORMAT RSP!");
            break;
        }
        u8_t byEndpoint = *pbyBuffer++;
        pbyBuffer       += 2; //Application profile identifier
        u16_t wType     = BigWord(&pbyBuffer); //Application device identifier
        Device_t device  = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);
        if (device.Modify() == NULL) { break; }
        device.Modify()->Type = (int_t) wType;
        ZbDriver::s_pZbModel->Add(device);
        ZbDriver::s_pZbModel->UpdateChanges();

        //Only request model & manufacturer info to one of same type devices (send when get the last endpoint).
        if(m_mapEPInfor[wNwk].byEPCount == 0) {
            m_mapEPInfor[wNwk].mapType[++m_mapEPInfor[wNwk].byTypeCount] = wType;
//            DEBUG2("Update Type: %d, %d at %d.", wType, m_mapEPInfor[wNwk].mapType[m_mapEPInfor[wNwk].byTypeCount], m_mapEPInfor[wNwk].byEPCount);
        } else {
            bool_t boCheck = TRUE;
            for(u8_t i = 1; i <= m_mapEPInfor[wNwk].byEPCount; i++) {
                if(m_mapEPInfor[wNwk].mapType[i] == wType) { boCheck = FALSE; break; }
            }
            if(boCheck == TRUE) {
                m_mapEPInfor[wNwk].mapType[++m_mapEPInfor[wNwk].byTypeCount] = wType;
//                DEBUG2("Update Type: %d, %d at %d.", wType, m_mapEPInfor[wNwk].mapType[m_mapEPInfor[wNwk].byTypeCount], m_mapEPInfor[wNwk].byEPCount);
            }
        }

        m_mapEPInfor[wNwk].byEPCount++;
        if (m_mapEPInfor[wNwk].byEPCount == m_mapEPInfor[wNwk].byTotalEP) {
            for(int_t i = 1; i <= m_mapEPInfor[wNwk].byTypeCount; i++) {
                Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(wNwk).Bind(m_mapEPInfor[wNwk].mapType[i]);
                if(device.Modify() == NULL) continue;
                if(device.Modify()->IsInterested() == TRUE) {
                    DEBUG2("Send req at count %d th for type %d.", m_mapEPInfor[wNwk].byEPCount, m_mapEPInfor[wNwk].mapType[i]);
                    Vector<DeviceInfo> vDI;
                    vDI.push_back(DeviceInfo::DI_Model);
                    vDI.push_back(DeviceInfo::DI_Manufacturer);
                    ZbZclGlobalCmd::s_pInstance->ReadAttributeRequest(device, vDI);
                }
            }
        }


    }
        break;
    case ZDO_STATUS_INVALID_EP:
    case ZDO_STATUS_NOT_ACTIVE:
    case ZDO_STATUS_NO_DESCRIPTOR:
    case ZDO_STATUS_INV_REQUESTTYPE:
    case ZDO_STATUS_DEVICE_NOT_FOUND:
    default:
        DEBUG1("INVALID EP|NOT ACTIVE|NO DESCRIPTOR|INV REQUESTTYPE|DEVICE NOT FOUND!");
        break;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::LeaveRequest(
    ZbPacket_p pZbPacket,
    Device_t device
){
    u16_t wNwk = device->Network.GetValue();
    if(m_mapEPInfor.find(wNwk) != m_mapEPInfor.end()) {
        m_mapEPInfor.erase(wNwk);
    }
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(ZDO_MGMT_LEAVE_REQ >> 8);
    pZbPacket->Push(ZDO_MGMT_LEAVE_REQ & 0xFF);
    pZbPacket->Push(0x0A);
    pZbPacket->Push(0x00);
    u8_t byMAC[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //MC will add MAC.
    pZbPacket->Push(byMAC, 8);
    pZbPacket->Push(0b00000011);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);

    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    ZbSocketCmd::GetInstance()->SendLstDel(devices);
    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
        if (m_mapEPInfor.find(wNwk) != m_mapEPInfor.end()) {
            m_mapEPInfor.erase(wNwk);
        }

        (*it).Remove();
        ZbDriver::s_pZbModel->UpdateChanges();
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::LeaveResponse(
    u16_t wNwk,
    u8_p pbyBuffer,
    u32_t idwLen
) {
    pbyBuffer   += 2; //Ignore Cmd Payload Length
    u8_t byStatus = *pbyBuffer;

    if (byStatus == ZDO_STATUS_SUCCESS) {
        DEBUG2("Device %d left." ,wNwk);
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
        ZbSocketCmd::GetInstance()->SendLstDel(devices);
        for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            if (m_mapEPInfor.find(wNwk) != m_mapEPInfor.end()) {
                m_mapEPInfor.erase(wNwk);
            }

            (*it).Remove();
            ZbDriver::s_pZbModel->UpdateChanges();
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
const DeviceLogic_p
ZbZdoCmd::GetDeviceLogic() {
    return &m_mapEPInfor;
}








