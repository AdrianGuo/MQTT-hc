/*
 * ZbZDOCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <debug.hpp>
#include <LogPlus.hpp>
#include <zcl.hpp>
#include <zdo.hpp>
#include <zcl_lumi.hpp>
#include <zcl_ha.hpp>
#include <ZbHelper.hpp>
#include <ZbSocketCmd.hpp>

#include <ZbZdoCmd.hpp>

ZbZdoCmd* ZbZdoCmd::s_pInstance = NULL;
DeviceLogic_t ZbZdoCmd::s_mapEPInfor = {};

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
            LOG_WARN("ZDO COMMAND NOT FOUND!");
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

    LOG_INFO("Device %d announce.", wNwk);

    //Not request ActiveEndpoint to device that has been rejoined.
    u8_t byCheck = 0;
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("MAC=?").Bind(MAC);
    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND MAC=?").Bind(wNwk).Bind(MAC);
    if(devices.size() > 0) {
        if(device.Modify() != NULL) {
            //request state
            return;
        }
        for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            Device_t tempDevice = *it;
            byCheck++;
            if(byCheck == 1) {
                s_mapEPInfor[wNwk].byTotalEP   = s_mapEPInfor[tempDevice.Modify()->Network].byTotalEP;
                s_mapEPInfor[wNwk].byEPCount   = s_mapEPInfor[tempDevice.Modify()->Network].byEPCount;
                s_mapEPInfor[wNwk].byTypeCount   = s_mapEPInfor[tempDevice.Modify()->Network].byTypeCount;
                for(Map<u16_t, u16_t>::const_iterator_t it2 = s_mapEPInfor[tempDevice.Modify()->Network].mapType.begin();
                        it2 != s_mapEPInfor[tempDevice.Modify()->Network].mapType.end(); it2++) {
                    s_mapEPInfor[wNwk].mapType[it2->first] = it2->second;
                }
                s_mapEPInfor.erase(tempDevice.Modify()->Network);
            }

            tempDevice.Modify()->Network = wNwk;
            tempDevice.Modify()->DeviceID = wNwk;
            ZbDriver::s_pZbModel->Add(tempDevice);
//            ZbDriver::s_pZbModel->UpdateChanges();

            if(tempDevice.Modify()->IsInterested()) {
                //request state
            }
        }
        ZbDriver::s_pZbModel->UpdateChanges();

        ZbSocketCmd::GetInstance()->SendLstDel(devices);
        for(Map<u16_t, u16_t>::const_iterator_t it3 = s_mapEPInfor[wNwk].mapType.begin();
                it3 != s_mapEPInfor[wNwk].mapType.end(); it3++) {
            if(ZbDeviceDb::IsInterested(it3->second)) {
                Devices_t devices2 = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(wNwk).Bind(it3->second);
                ZbSocketCmd::GetInstance()->SendLstAdd(devices2);
            }
        }

    } else {
        m_mapTemps[wNwk]   = MAC;
        ActiveEndpointRequest(wNwk);
    }
    // ZB Device Type (router, end device - sleepable)!!!
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
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
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
        s_mapEPInfor[wNwk].byTotalEP   = byEndpointNo;
        s_mapEPInfor[wNwk].byEPCount   = 0;
        s_mapEPInfor[wNwk].byTypeCount = 0;
        for (int_t i = 0; i < byEndpointNo; i++)
            byEndpointList[i] = *pbyBuffer++;
    }
    switch (byStatus) {
    case ZDO_STATUS_SUCCESS: {
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
        Controller_t pController      = ZbDriver::s_pZbModel->Find<ZbControllerDb>();
        if (devices.size() == 0) {
            for (int j = 0; j < byEndpointNo; j++) {
                ZbDeviceDb_p pZbDevice = new ZbDeviceDb();
                pZbDevice->DeviceID  = wNwk; //*1000 + byEndpointList[j];
                pZbDevice->Network   = wNwk;
                pZbDevice->MAC       = m_mapTemps[wNwk];
                pZbDevice->Endpoint  = byEndpointList[j];
                pZbDevice->Controller   = pController; //!!! temporary !!!

                Device_t device = ZbDriver::s_pZbModel->Add(pZbDevice);
//                ZbDriver::s_pZbModel->UpdateChanges();
                pZbDevice = NULL;
                delete pZbDevice;

                NodeDescriptionRequest(device);
            }
        } else {
            for (int j = 0; j < byEndpointNo; j++) {
                Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("MAC=? AND Endpoint=?").Bind(m_mapTemps[wNwk]).Bind(byEndpointList[j]);
                if(device.Modify() != NULL) {
                    device.Modify()->DeviceID       = wNwk; //*1000 + byEndpointList[j];
                    device.Modify()->Network        = wNwk;
                    device.Modify()->MAC            = m_mapTemps[wNwk];
                    device.Modify()->Controller     = pController; //!!! temporary !!!

                    ZbDriver::s_pZbModel->Add(device);
//                    ZbDriver::s_pZbModel->UpdateChanges();
                    NodeDescriptionRequest(device);
                }
            }
        }
        ZbDriver::s_pZbModel->UpdateChanges();
        m_mapTemps.erase(wNwk);
    }
        break;
    case ZDO_STATUS_NO_DESCRIPTOR:
    case ZDO_STATUS_INV_REQUESTTYPE:
    case ZDO_STATUS_DEVICE_NOT_FOUND:
    default:
        LOG_WARN("NO DESCRIPTOR|INV REQUEST TYPE|DEVICE NOT FOUND!");
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
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
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
            LOG_WARN("NONFORMAT RSP!");
            break;
        }
        u8_t byEndpoint = *pbyBuffer++;
        pbyBuffer       += 2; //Application profile identifier
        u16_t wType     = BigWord(&pbyBuffer); //Application device identifier
        Device_t device  = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);
        if (device.Modify() == NULL) { break; }
        device.Modify()->Type = (int_t) wType;
        ZbDriver::s_pZbModel->Add(device);
//        ZbDriver::s_pZbModel->UpdateChanges();

        //Only request model & manufacturer info to one of same type devices (send when get the last endpoint).
        if(s_mapEPInfor[wNwk].byEPCount == 0) {
            s_mapEPInfor[wNwk].mapType[++s_mapEPInfor[wNwk].byTypeCount] = wType;
        } else {
            bool_t boCheck = TRUE;
            for(u8_t i = 1; i <= s_mapEPInfor[wNwk].byEPCount; i++) {
                if(s_mapEPInfor[wNwk].mapType[i] == wType) { boCheck = FALSE; break; }
            }
            if(boCheck == TRUE) {
                s_mapEPInfor[wNwk].mapType[++s_mapEPInfor[wNwk].byTypeCount] = wType;
            }
        }

        s_mapEPInfor[wNwk].byEPCount++;
        if (s_mapEPInfor[wNwk].byEPCount == s_mapEPInfor[wNwk].byTotalEP) {
            Device_t device;
            Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
            bool_t boCheck = FALSE;
            for(Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                if((*it).Modify()->IsInterested()) {
                    device = *it;
                    boCheck = TRUE;
                    break;
                }
            }
            if(!boCheck) { return; }
            Vector<DeviceInfo> vDI;
            vDI.push_back(DeviceInfo::DI_Model);
            vDI.push_back(DeviceInfo::DI_Manufacturer);
            ZbZclGlobalCmd::s_pInstance->ReadAttributeRequest(device, vDI);
        }


    }
        break;
    case ZDO_STATUS_INVALID_EP:
    case ZDO_STATUS_NOT_ACTIVE:
    case ZDO_STATUS_NO_DESCRIPTOR:
    case ZDO_STATUS_INV_REQUESTTYPE:
    case ZDO_STATUS_DEVICE_NOT_FOUND:
    default:
        LOG_WARN("INVALID EP|NOT ACTIVE|NO DESCRIPTOR|INV REQUESTTYPE|DEVICE NOT FOUND!");
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
    u16_t wNwk
){
    ZbPacket_p pZbPacket = new ZbPacket(15);
    pZbPacket->SetCmdID(ZDO_CMD_REQ);
    if(s_mapEPInfor.find(wNwk) != s_mapEPInfor.end()) {
        s_mapEPInfor.erase(wNwk);
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
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
    delete pZbPacket;

    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    if(devices.size() > 0) {
        ZbSocketCmd::GetInstance()->SendLstDel(devices);
        for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            (*it).Remove();
        }
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
        LOG_INFO("Device %d left." ,wNwk);
        if (s_mapEPInfor.find(wNwk) != s_mapEPInfor.end()) {
            s_mapEPInfor.erase(wNwk);
        }
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
        if(devices.size() > 0) {
            ZbSocketCmd::GetInstance()->SendLstDel(devices);
            for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                (*it).Remove();
            }
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
DeviceLogic_t
ZbZdoCmd::GetDeviceLogic() {
    return s_mapEPInfor;
}








