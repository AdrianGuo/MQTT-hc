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
#include <zcl_general.hpp>
#include <ZbHelper.hpp>
#include <ZbSocketCmd.hpp>
#include <IO.hpp>
#include <ZbZdoCmd.hpp>

#define DEVICE_ANNOUNCE_TIME    (2)
#define ACTIVE_ENDPOINT_TIME    (10)
#define MAX_REQUEST_NO          (5)

ZbZdoCmd* ZbZdoCmd::s_pInstance = NULL;
DeviceLogic_t ZbZdoCmd::s_mapEPInfo = {};

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZdoCmd::ZbZdoCmd() {
    m_pTimer = RTimer::getTimerInstance();
    m_DAFunctor = makeFunctor((TimerFunctor_p) NULL, *this, &ZbZdoCmd::HandleDeviceAnnounce);
    m_iDAHandle = -1;
    m_AEFunctor = makeFunctor((TimerFunctor_p) NULL, *this, &ZbZdoCmd::HandleActiveEndpoint);
    m_iAEHandle = -1;

    m_pDA = NULL;
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
	if(m_pDA != NULL)
		delete m_pDA;
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
    u16_t wNwk = LittleWord(&pbyBuffer);
    u16_t wZDOCmdID = LittleWord(&pbyBuffer);
    u32_t idwLen = (*pbyBuffer++) - 1;
    ++pbyBuffer; //Ignore this byte
    switch (wZDOCmdID) {
        case ZDO_IEEE_ADDR_RSP:
            IEEEAddrResponse(pbyBuffer, idwLen);
            break;
        case ZDO_NODE_DESC_RSP:
            NodeDescResponse(pbyBuffer, idwLen);
            break;

        case ZDO_DEVICE_ANNCE:
            DeviceAnnounce(pbyBuffer, idwLen);
            break;

        case ZDO_MGMT_LEAVE_RSP:
            LeaveResponse(wNwk, pbyBuffer, idwLen);
            break;

        case ZDO_ACTIVE_EP_RSP:
            if (s_mapEPInfo[wNwk].IsDone == TRUE) { break; }
            ActiveEndpointResponse(pbyBuffer, idwLen);
        break;

        case ZDO_SIMPLE_DESC_RSP:
            if (s_mapEPInfo[wNwk].IsDone == TRUE) { break; }
            SimpleDescResponse(pbyBuffer, idwLen);
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
void_t ZbZdoCmd::IEEEAddrRequest(
    u16_t wNwk
) {
    LOG_INFO("IEEE Address Request %d.", wNwk);
    ZbPacket_p pZbPacket = new ZbPacket(10);
    pZbPacket->SetCmdID(ZDO_CMD_REQ);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(ZDO_IEEE_ADDR_REQ >> 8);
    pZbPacket->Push(ZDO_IEEE_ADDR_REQ & 0xFF);
    pZbPacket->Push(0x04);
    pZbPacket->Push(0x00);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(0x00);
    pZbPacket->Push(0x00);
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
ZbZdoCmd::IEEEAddrResponse(
    u8_p pbyBuffer,
    u32_t idwLen
) {
    pbyBuffer++;
    String MAC      = HexToString(pbyBuffer, 8);
    pbyBuffer       += 8;
    u16_t wNwk      = BigWord(&pbyBuffer, false);
    s_mapEPInfo[wNwk].MAC = MAC;
    ManualDeviceAnnounce(wNwk, MAC);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::DeviceAnnounce(
        u8_p pbyBuffer,
        u32_t idwLen
) {
	Notify(DevSig);

    u16_t wNwk      = BigWord(&pbyBuffer);
    String MAC      = HexToString(pbyBuffer, 8);
    pbyBuffer       += 8;
    LOG_INFO("Device %d announce.", wNwk);
    ManualDeviceAnnounce(wNwk, MAC);
    // ZB Device Type (router, end device - sleepable)!!!
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::ManualDeviceAnnounce(
    u16_t wNwk,
    String MAC
) {
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
                 s_mapEPInfo[wNwk].byTotalEP   = s_mapEPInfo[tempDevice.Modify()->Network].byTotalEP;
                 for(Map<u8_t, u16_t>::const_iterator it2 = s_mapEPInfo[tempDevice.Modify()->Network].mapType.begin();
                         it2 != s_mapEPInfo[tempDevice.Modify()->Network].mapType.end(); it2++) {
                     s_mapEPInfo[wNwk].mapType[it2->first] = it2->second;
                 }
                 s_mapEPInfo.erase(tempDevice.Modify()->Network);
             }

             tempDevice.Modify()->Network = wNwk;
             tempDevice.Modify()->DeviceID = wNwk;
             ZbDriver::s_pZbModel->Add(tempDevice);

             if(tempDevice.Modify()->IsInterested()) {
                 //request state
             }
         }
         ZbDriver::s_pZbModel->UpdateChanges();
         ZbSocketCmd::GetInstance()->SendLstDel(devices);

         for(Map<u8_t, u16_t>::const_iterator it3 = s_mapEPInfo[wNwk].mapType.begin();
                 it3 != s_mapEPInfo[wNwk].mapType.end(); it3++) {
             Map<u16_t,bool_t> mapCheck;
             if(ZbDeviceDb::IsInterested(it3->second)) {
                 if(mapCheck.find(it3->second) == mapCheck.end()) {
                     Devices_t devices2 = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(wNwk).Bind(it3->second);
                     ZbSocketCmd::GetInstance()->SendLstAdd(devices2);
                     mapCheck[it3->second] = TRUE;
                 }
             }
         }

     } else {
    	 s_mapEPInfo[wNwk].MAC = MAC;
         BackupDev_t BuDev = ZbDriver::s_pZbModel->Find<BackupInfoDb>().Where("MAC=?").Bind(MAC);
         if(BuDev.Modify() != NULL) {
             if(RestoreBuDevice(wNwk, BuDev)) return;
         }

         RequestMMInfo(wNwk);

         if(m_iDAHandle != -1) {
             m_pTimer->CancelTimer(m_iDAHandle);
         	if(m_pDA != NULL)
         		delete m_pDA;
         }
         u16_p mapEPInforSize = new u16_t();
         m_pDA = mapEPInforSize;
         *mapEPInforSize = s_mapEPInfo.size();
         m_iDAHandle = m_pTimer->StartTimer(RTimer::Repeat::OneTime, DEVICE_ANNOUNCE_TIME, &m_DAFunctor, mapEPInforSize);

         if(m_iDAHandle == -1) {
             LOG_DEBUG("Full of timer pool!");
             ActiveEndpointRequest(wNwk);
             if(mapEPInforSize != NULL)
                 delete mapEPInforSize;
         }
     }
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
    if (s_mapEPInfo[wNwk].IsDone == TRUE) { return; }
//    LOG_DEBUG("ActiveEndpointRequest");
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
//    LOG_DEBUG("ActiveEndpointResponse");
    u8_t byStatus       = *pbyBuffer++;
    u16_t wNwk          = BigWord(&pbyBuffer);
    u8_t byEndpointNo   = *pbyBuffer++;
    if(byEndpointNo != (idwLen - 4)) {
        LOG_WARN("EndpointNo is mismatched! %d eps in declared but %d eps in list", byEndpointNo, (idwLen - 4));
        byEndpointNo = idwLen - 4;
    }
    if (byEndpointNo > 0) {
        s_mapEPInfo[wNwk].byTotalEP   = byEndpointNo;
        s_mapEPInfo[wNwk].vEPList.clear();
        for (int_t i = 0; i < byEndpointNo; i++) {
            s_mapEPInfo[wNwk].vEPList.push_back(*pbyBuffer++);
        }
    } else {
        return;
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
                pZbDevice->MAC       = s_mapEPInfo[wNwk].MAC;
                pZbDevice->Endpoint  = s_mapEPInfo[wNwk].vEPList[j];
                pZbDevice->Controller   = pController; //!!! temporary !!!

                Device_t device = ZbDriver::s_pZbModel->Add(pZbDevice);
                pZbDevice = NULL;
                delete pZbDevice;

                SimpleDescRequest(device);
            }
        } else {
            for (u32_t j = 0; j < s_mapEPInfo[wNwk].vEPList.size(); j++) {
                Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("MAC=? AND Endpoint=?").Bind(s_mapEPInfo[wNwk].MAC).Bind(s_mapEPInfo[wNwk].vEPList[j]);
                if(device.Modify() != NULL) {
                    device.Modify()->DeviceID       = wNwk; //*1000 + byEndpointList[j];
                    device.Modify()->Network        = wNwk;
                    device.Modify()->MAC            = s_mapEPInfo[wNwk].MAC;
                    device.Modify()->Controller     = pController; //!!! temporary !!!

                    ZbDriver::s_pZbModel->Add(device);
                    SimpleDescRequest(device);
                }
            }
        }
        ZbDriver::s_pZbModel->UpdateChanges();
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
ZbZdoCmd::NodeDescRequest(
    u16_t wNwk
) {
    ZbPacket_p pZbPacket = new ZbPacket(8);
    pZbPacket->SetCmdID(ZDO_CMD_REQ);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(ZDO_NODE_DESC_REQ >> 8);
    pZbPacket->Push(ZDO_NODE_DESC_REQ & 0xFF);
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
ZbZdoCmd::NodeDescResponse(
    u8_p pbyBuffer,
    u32_t idwLen
) {

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::SimpleDescRequest(
    Device_t device
){
//    LOG_DEBUG("SimpleDescRequest");
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
ZbZdoCmd::SimpleDescResponse(
    u8_p pbyBuffer,
    u32_t idwLen
){
//    LOG_DEBUG("SimpleDescResponse");
    u8_t byStatus   = *pbyBuffer++;
    u16_t wNwk      = BigWord(&pbyBuffer);
    u8_t byLeng     = *pbyBuffer++;
    if (byStatus != ZDO_STATUS_SUCCESS || byLeng < 5) {
        LOG_WARN("INVALID EP|NOT ACTIVE|NO DESCRIPTOR|INV REQUESTTYPE|DEVICE NOT FOUND!");
        return;
    }

    u8_t byEndpoint = *pbyBuffer++;
    if (std::find(s_mapEPInfo[wNwk].vEPList.begin(), s_mapEPInfo[wNwk].vEPList.end(), byEndpoint) == s_mapEPInfo[wNwk].vEPList.end())
        return;
    pbyBuffer += 2; //Application profile identifier
    u16_t wType = BigWord(&pbyBuffer); //Application device identifier
    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);
    if (device.Modify() == NULL) { return; }
    if (wType == 0) {
        SimpleDescRequest(device);
        return;
    }
    device.Modify()->Type = (int_t) wType;
    ZbDriver::s_pZbModel->Add(device);
    ZbDriver::s_pZbModel->UpdateChanges();

    //Only request model & manufacturer info to one of same type devices (send when get the last endpoint).
    s_mapEPInfo[wNwk].mapType[byEndpoint] = wType;
    u8_t byEpNo = 0;
    for(Vector<u8_t>::const_iterator it = s_mapEPInfo[wNwk].vEPList.begin(); it != s_mapEPInfo[wNwk].vEPList.end(); it++) {
        if(s_mapEPInfo[wNwk].mapType.find(*it) != s_mapEPInfo[wNwk].mapType.end())
            byEpNo++;
    }
    if (byEpNo == s_mapEPInfo[wNwk].byTotalEP) {
        s_mapEPInfo[wNwk].IsDone = TRUE;
        Device_t device;
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
        u8_t byCheck = 0;
        for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            if ((*it).Modify()->IsInterested()) {
                device = *it;
                byCheck++;
            }
            if((*it).Modify()->Type == 0) {
                byCheck = 0;
                break;
            }
        }
        if (byCheck == 0) {
            s_mapEPInfo[wNwk].IsDone = FALSE;
            SimpleDescRequest(device);
            return;
        }
        Vector<DeviceInfo> vDI;
        vDI.push_back(DI_Model);
        vDI.push_back(DI_Manufacturer);
        ZbZclGlobalCmd::GetInstance()->ReadAttributeRequest(device, vDI);
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
    if(s_mapEPInfo.find(wNwk) != s_mapEPInfo.end()) {
        s_mapEPInfo.erase(wNwk);
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

    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    if(devices.size() > 0) {
        ZbSocketCmd::GetInstance()->SendLstDel(devices);
        for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
            (*it).Remove();
        }
        ZbDriver::s_pZbModel->UpdateChanges();
    }

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
ZbZdoCmd::LeaveResponse(
    u16_t wNwk,
    u8_p pbyBuffer,
    u32_t idwLen
) {
    u8_t byStatus = *pbyBuffer;

    if (byStatus == ZDO_STATUS_SUCCESS) {
        LOG_INFO("Device %d left." ,wNwk);
        if (s_mapEPInfo.find(wNwk) != s_mapEPInfo.end()) {
            s_mapEPInfo.erase(wNwk);
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
    return s_mapEPInfo;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::HandleDeviceAnnounce(
    void_p pBuffer
) {
    m_iDAHandle = -1;
    u16_t mapEPsize = *((u16_p) pBuffer);
    delete ((u16_p) pBuffer);
    m_pDA = NULL;
    LOG_DEBUG("Handle DeviceAnnounce %d", mapEPsize);
    if(s_mapEPInfo.size() <= mapEPsize) {
        for(DeviceLogic_t::const_iterator it = s_mapEPInfo.begin(); it != s_mapEPInfo.end(); it++) {
            if((it->second.byTotalEP == 0) &&
                    (s_mapEPInfo[it->first].IsAERequested == FALSE))
            {
                s_mapEPInfo[it->first].IsAERequested = TRUE;
                ActiveEndpointRequest(it->first);
                u16_p pwNwk = new u16_t();
                pwNwk[0] = it->first;
                s_mapEPInfo[it->first].byAEReqCount++;
                m_iAEHandle = m_pTimer->StartTimer(RTimer::Repeat::OneTime, ACTIVE_ENDPOINT_TIME, &m_AEFunctor, pwNwk);
            }
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::HandleActiveEndpoint(
    void_p pBuffer
) {
    m_iDAHandle = -1;
    u16_p tmp = (u16_p) pBuffer;
    u16_t wNwk = *tmp;
    LOG_DEBUG("Handle ActiveEndpoint %d", wNwk);
    if(s_mapEPInfo[wNwk].IsDone == TRUE) {
    	m_pTimer->CancelTimer(m_iAEHandle);
        delete tmp;
        return;
    } else {
        if(s_mapEPInfo[wNwk].byAEReqCount < MAX_REQUEST_NO) {
            ActiveEndpointRequest(wNwk);
            ++s_mapEPInfo[wNwk].byAEReqCount;
            m_iAEHandle = m_pTimer->StartTimer(RTimer::Repeat::OneTime, ACTIVE_ENDPOINT_TIME, &m_AEFunctor, tmp);
        } else {
            delete tmp;
            LeaveRequest(wNwk);
        }
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbZdoCmd::RestoreBuDevice(
     u16_t wNwk,
     BackupDev_t BuDev
) {
    Controller_t controller = ZbDriver::s_pZbModel->Find<ZbControllerDb>();
    if(BuDev->EndpointNo.GetValue() > 20) {
        BuDev.Remove();
        ZbDriver::s_pZbModel->UpdateChanges();
        return FALSE;
    }
    for(int_t i = 1; i <= BuDev->EndpointNo.GetValue(); i++) {
        Device_t tmp = ZbDriver::s_pZbModel->Add(new ZbDeviceDb());
        tmp.Modify()->DeviceID = wNwk;
        tmp.Modify()->Network = wNwk;
        tmp.Modify()->MAC = s_mapEPInfo[wNwk].MAC;
        if(BuDev.Modify()->EpOrd[i]->GetValue() == 0) {
            BuDev.Remove();
            ZbDriver::s_pZbModel->UpdateChanges();
            return FALSE;
        }
        tmp.Modify()->Endpoint = BuDev.Modify()->EpOrd[i]->GetValue() % 1000;
        tmp.Modify()->Type = (int_t) (BuDev.Modify()->EpOrd[i]->GetValue() / 1000);
        tmp.Modify()->Controller = controller;
        if(ZbDeviceDb::IsInterested(tmp.Modify()->Type.GetValue())) {
            tmp.Modify()->Model = BuDev.Modify()->Model;
            tmp.Modify()->Manufacturer = BuDev.Modify()->Manufacturer;
            tmp.Modify()->GenerateDeviceInfo();
            ZbZclGlobalCmd::RequestDevicesState(tmp);
        }

        s_mapEPInfo[wNwk].vEPList.push_back(tmp.Modify()->Endpoint.GetValue());
        s_mapEPInfo[wNwk].mapType[tmp.Modify()->Endpoint.GetValue()] = tmp.Modify()->Type.GetValue();
    }
    s_mapEPInfo[wNwk].byTotalEP = BuDev->EndpointNo.GetValue();
    s_mapEPInfo[wNwk].IsAERequested = TRUE;
    s_mapEPInfo[wNwk].HasModelInfo = TRUE;
    s_mapEPInfo[wNwk].HasManufInfo = TRUE;
    s_mapEPInfo[wNwk].IsDone = TRUE;

    ZbDriver::s_pZbModel->UpdateChanges();

    LOG_INFO("Device %s at %04X has joined.", BuDev.Modify()->Model.GetValue().c_str(), wNwk);
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    ZbSocketCmd::GetInstance()->SendLstAdd(devices);
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZdoCmd::RequestMMInfo(u16_t wNwkAdd) {
        ZbPacket_p pZbPacket = new ZbPacket(11);
        pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
        pZbPacket->Push(wNwkAdd >> 8);
        pZbPacket->Push(wNwkAdd & 0xFF);
        pZbPacket->Push(0x01);
        pZbPacket->Push(ZCL_CLUSTER_ID_GEN_BASIC >> 8);
        pZbPacket->Push(ZCL_CLUSTER_ID_GEN_BASIC & 0xFF);
        pZbPacket->Push(ZCL_CMD_READ);
        pZbPacket->Push(0x04); //Payload's length
        pZbPacket->Push(ATTRID_BASIC_MODEL_ID & 0xFF);
        pZbPacket->Push(ATTRID_BASIC_MODEL_ID >> 8);
        pZbPacket->Push(ATTRID_BASIC_MANUFACTURER_NAME & 0xFF);
        pZbPacket->Push(ATTRID_BASIC_MANUFACTURER_NAME >> 8);
        ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
        delete pZbPacket;
}


