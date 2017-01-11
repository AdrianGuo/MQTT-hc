/*
 * ZbZCLGlobCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <debug.hpp>
#include <LogPlus.hpp>
#include <zcl.hpp>
#include <zcl_ha.hpp>
#include <zcl_lumi.hpp>
#include <zcl_general.hpp>
#include <ZbHelper.hpp>
#include <ZbSocketCmd.hpp>
#include <JsonDevGet.hpp>

#include <ZbZclGlobalCmd.hpp>

ZbZclGlobalCmd* ZbZclGlobalCmd::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZclGlobalCmd::ZbZclGlobalCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZclGlobalCmd_p
ZbZclGlobalCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbZclGlobalCmd();
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZclGlobalCmd::~ZbZclGlobalCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::ProcRecvMessage(
    void_p pInBuffer
) {
    ZbPacket_p pZbPacket    = (ZbPacket_p) pInBuffer;
    u8_p pbyBuffer          = pZbPacket->GetBuffer();

    u8_t byCmdID = *(pbyBuffer + 5);

    switch (byCmdID) {
        case ZCL_CMD_READ_RSP:
            ReadAttributeResponse(pbyBuffer);
            break;

        case ZCL_CMD_WRITE_RSP:
            WriteAttributeResponse(pbyBuffer);
            break;

        default:
            LOG_WARN("ZCL GLOBAL COMMAND NOT FOUND!");
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
ZbZclGlobalCmd::ReadAttributeRequest(
        Device_t device,
        Vector<DeviceInfo> vDI
){
    if (vDI.size() == 0) { return; }
    if(device.Modify()->IsInterested()) {
        Map<u16_t, Vector<DeviceInfo>> mapDIGroup;

        for(u8_t i = 0; i < (u8_t) vDI.size(); i++) {
            if(device.Modify()->Action.find(vDI[i]) == device.Modify()->Action.end()) { continue; }
            mapDIGroup[device.Modify()->Action[vDI[i]].DP_ClusterID].push_back(vDI[i]);
        }

        if(mapDIGroup.size() == 0) { return; }

        for (Map<u16_t, Vector<DeviceInfo>>::const_iterator it = mapDIGroup.begin(); it != mapDIGroup.end(); it++) {
            u16_t wNwkAdd = (u16_t) device->Network.GetValue();
            ZbPacket_p pZbPacket = new ZbPacket(7 + 0x02 * it->second.size());
            pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
            pZbPacket->Push(wNwkAdd >> 8);
            pZbPacket->Push(wNwkAdd & 0xFF);
            pZbPacket->Push((u8_t) device->Endpoint.GetValue());
            pZbPacket->Push(device.Modify()->Action[(it->second)[0]].DP_ClusterID >> 8);
            pZbPacket->Push(device.Modify()->Action[(it->second)[0]].DP_ClusterID & 0xFF);
            pZbPacket->Push(ZCL_CMD_READ);
            pZbPacket->Push(0x02 * (it->second).size()); //Payload's length
            for (u8_t j = 0; j < (u8_t) (it->second).size(); j++) {
                pZbPacket->Push(device.Modify()->Action[(it->second)[j]].DP_AttributeID & 0xFF);
                pZbPacket->Push(device.Modify()->Action[(it->second)[j]].DP_AttributeID >> 8);

                Request   tmpReq;
                tmpReq.ReqFrom  = device->OwnersReq.front();
                tmpReq.ReqType  = READ_REQ;
                device.Modify()->PendingReqs((it->second)[j]).push(tmpReq);
            }
            device.Modify()->OwnersReq.pop();
            ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
            delete pZbPacket;
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
ZbZclGlobalCmd::ReadAttributeRequest(
    Device_t device,
    DeviceInfo devInfo
){
    Vector<DeviceInfo> vDI;
    vDI.push_back(devInfo);
    ReadAttributeRequest(device, vDI);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::ReadAttributeResponse(
    u8_p pbyBuffer
){
//    LOG_DEBUG("ReadAttributeResponse.");
    u16_t wNwk              = LittleWord(&pbyBuffer);
    if(wNwk == 0) return;

    if(ZbZdoCmd::s_mapEPInfo.find(wNwk) == ZbZdoCmd::s_mapEPInfo.end()) {
        ZbZdoCmd::GetInstance()->IEEEAddrRequest(wNwk);
    }

    u8_t byEndpoint         = *pbyBuffer++;
    u16_t wClusterID        = LittleWord(&pbyBuffer);
    ++pbyBuffer; //CMD ID
    u8_t byLength = *pbyBuffer++; //Payload's length

    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);

    if(device.Modify() == NULL &&
            wClusterID == ZCL_CLUSTER_ID_GEN_BASIC)
    {
        ProcessException(wNwk, pbyBuffer);
        return;
    }

    if(device.Modify() == NULL) { return; }

    DeviceProperties vResponseDP;
    Vector<u8_p> vpData;

    while (byLength > 0) {
        u16_t wAttributeID  =  BigWord(&pbyBuffer);
        byLength -= 2;

        DeviceProperty temp;
        temp.DP_ClusterID   = wClusterID;
        temp.DP_AttributeID = wAttributeID;

        u8_t byStatus       = *pbyBuffer++;
        byLength--;
        if(byStatus != ZCL_STATUS_SUCCESS) {
            DEBUG1("ZCL_STATUS_UNSUPPORTED_ATTRIBUTE.");
            continue;
        }
        u8_t byAttributeDataType  = *pbyBuffer++;
        byLength--;
        temp.DP_AttributeDataType = byAttributeDataType;

        u8_t byAttributeDataTypeSize = ZbDeviceDb::GetAttributeDataSize(byAttributeDataType, &pbyBuffer);
        if((byAttributeDataType == 0x41) || (byAttributeDataType == 0x42)) byLength -= 1;
        else if((byAttributeDataType == 0x43) || (byAttributeDataType == 0x44)) byLength -= 2;
        temp.DP_AttributeDataSize = byAttributeDataTypeSize;

        u8_p pbyAttributeData = new u8_t[byAttributeDataTypeSize + 1];
        bzero(pbyAttributeData, byAttributeDataTypeSize + 1);
        memcpy(pbyAttributeData, pbyBuffer, byAttributeDataTypeSize);

        bool boCheck = false;

        for(Action_t::const_iterator it = device.Modify()->Action.begin(); it != device.Modify()->Action.end(); it++) {
            if(temp == it->second) {
                temp.DP_DIName = it->first;
                vResponseDP.push_back(temp);

                boCheck = true;
                vpData.push_back(pbyAttributeData);
                break;
            }
        }

        if(boCheck) { pbyAttributeData = NULL; }
        delete pbyAttributeData;

        byLength -= byAttributeDataTypeSize;

        if(byLength > 0) {
            pbyBuffer += byAttributeDataTypeSize;
        }

    }

    if(wClusterID == ZCL_CLUSTER_ID_GEN_BASIC) {
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
        if(devices.size() == 0) { return; }
        for(u8_t i = 0; i < (u8_t) vResponseDP.size(); i++) {
            if(vResponseDP[i].DP_AttributeID == ATTRID_BASIC_MANUFACTURER_NAME) {
                for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                    Device_t tempDevice = (*it);
                    if(tempDevice.Modify()->IsInterested()) {
                        tempDevice.Modify()->Manufacturer = String((const char*) vpData[i]);
                        ZbDriver::s_pZbModel->Add(tempDevice);
                        ZbDriver::s_pZbModel->UpdateChanges();
                    }
                }
            } else if(vResponseDP[i].DP_AttributeID == ATTRID_BASIC_MODEL_ID) {
                String ModelName = String((const char*) vpData[i]);
                LOG_INFO("Device %s at %04X has joined.", ModelName.c_str(), wNwk);
                for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                    Device_t tempDevice = (*it);
                    if(tempDevice.Modify()->IsInterested()) {
                        tempDevice.Modify()->Model = ModelName;
                        tempDevice.Modify()->GenerateDeviceInfo();
                        ZbDriver::s_pZbModel->Add(tempDevice);
                        ZbDriver::s_pZbModel->UpdateChanges();

                        //Request State
                        if(tempDevice->RealType != LUMI_DEVICE_IR)
                            RequestDevicesState(tempDevice);
                    }
                }
                ZbSocketCmd::GetInstance()->SendLstAdd(devices);
            }
            delete[] vpData[i];
        }
        SaveDevicesInfo(wNwk);
        vpData.clear();
    } else {
        device.Modify()->ReceiveInforFromDevice(vResponseDP, vpData);
    }

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::WriteAttributeRequest(
        Device_t device,
        DeviceProperties vDP
) {

    if (vDP.size() == 0) { return; }

    Map<u16_t, DeviceProperties> mapDPGroup;
    for(u8_t i = 0; i < (u8_t) vDP.size(); i++) {
        if(device.Modify()->Action.find(vDP[i].DP_DIName) == device.Modify()->Action.end()) { continue; }
        mapDPGroup[vDP[i].DP_ClusterID].push_back(vDP[i]);
    }

    for (Map<u16_t, DeviceProperties>::const_iterator it = mapDPGroup.begin(); it != mapDPGroup.end(); it++) {
        u16_t wNwkAdd = (u16_t) device->Network.GetValue();
        int_t iDataSize = 0;
        for (u8_t j = 0; j < (it->second).size(); j++) {
            iDataSize += device.Modify()->Action[(it->second[j]).DP_DIName].DP_AttributeDataSize;
        }
        ZbPacket_p pZbPacket = new ZbPacket(7 + 3*(it->second).size() + iDataSize);
        pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
        pZbPacket->Push(wNwkAdd >> 8);
        pZbPacket->Push(wNwkAdd & 0xFF);
        pZbPacket->Push((u8_t) device->Endpoint.GetValue());
        pZbPacket->Push((it->second)[0].DP_ClusterID >> 8);
        pZbPacket->Push((it->second)[0].DP_ClusterID & 0xFF);
        pZbPacket->Push(ZCL_CMD_WRITE);
        pZbPacket->Push(3*(it->second).size() + iDataSize); //Payload's length
        for (u8_t j = 0; j < (u8_t) (it->second).size(); j++) {
            pZbPacket->Push(device.Modify()->Action[(it->second)[j].DP_DIName].DP_AttributeID & 0xFF);
            pZbPacket->Push(device.Modify()->Action[(it->second)[j].DP_DIName].DP_AttributeID >> 8);
            pZbPacket->Push(device.Modify()->Action[(it->second)[j].DP_DIName].DP_AttributeDataType);
            for(u8_t k = 0; k < device.Modify()->Action[(it->second)[j].DP_DIName].DP_AttributeDataSize; k++) {
                pZbPacket->Push((it->second)[j].DP_AttributeData >> (8*k));
            }

            Request   tmpReq;
            tmpReq.ReqFrom  = device->OwnersReq.front();
            tmpReq.ReqType  = WRITE_REQ;
            tmpReq.ReqValue = (it->second)[j].DP_AttributeData;
            device.Modify()->PendingReqs((it->second)[j].DP_DIName).push(tmpReq);
        }
        device.Modify()->OwnersReq.pop();
        ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
        delete pZbPacket;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::WriteAttributeRequest(
        Device_t device,
        DeviceProperty devProperty
){
    DeviceProperties vDP;
    vDP.push_back(devProperty);
    WriteAttributeRequest(device, vDP);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t ZbZclGlobalCmd::WriteAttributeResponse(
        u8_p pbyBuffer
) {
//    u8_t byStatus       = *pbyBuffer++;
//    u16_t wAttributeID  =  LittleWord(&pbyBuffer);
//    device.Modify()->Action[DI_Using].DP_AttributeID = wAttributeID;
//    device.Modify()->SyncDeviceAction();
//    switch (byStatus) {
//        case ZCL_STATUS_SUCCESS:
//            DEBUG2("Write attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DI_Using].DP_AttributeID,
//                device.Modify()->Action[DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//        break;
//
//        case ZCL_STATUS_UNSUPPORTED_ATTRIBUTE:
//            DEBUG2("Attribute %04X of cluster %04X is not supported on device %08d.",
//                device.Modify()->Action[DI_Using].DP_AttributeID,
//                device.Modify()->Action[DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_READ_ONLY:
//        case ZCL_STATUS_NOT_AUTHORIZED:
//            DEBUG2("Permission denied to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DI_Using].DP_AttributeID,
//                device.Modify()->Action[DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_INVALID_DATA_TYPE:
//            DEBUG2("Invalid datatype to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DI_Using].DP_AttributeID,
//                device.Modify()->Action[DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_INVALID_VALUE:
//            DEBUG2("Invalid data to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DI_Using].DP_AttributeID,
//                device.Modify()->Action[DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        default:
//            DEBUG2("Error on write to attribute %04X of cluster %04X on device %08d.\nThe status result: %02X.",
//                    device.Modify()->Action[DI_Using].DP_AttributeID,
//                    device.Modify()->Action[DI_Using].DP_ClusterID,
//                    device->DeviceID.GetValue(),
//                    byStatus);
//            break;
//    }

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::Broadcast() {
    ZbPacket_p pZbPacket = new ZbPacket(9);
    pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
    pZbPacket->Push(0XFFFF >> 8);
    pZbPacket->Push(0XFFFF & 0xFF);
    pZbPacket->Push(0X01);
    pZbPacket->Push(ZCL_CLUSTER_ID_GEN_BASIC >> 8);
    pZbPacket->Push(ZCL_CLUSTER_ID_GEN_BASIC & 0xFF);
    pZbPacket->Push(ZCL_CMD_READ);
    pZbPacket->Push(0x02); //Payload's length
    pZbPacket->Push(ATTRID_BASIC_ZCL_VERSION & 0xFF);
    pZbPacket->Push(ATTRID_BASIC_ZCL_VERSION >> 8);
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
ZbZclGlobalCmd::ProcessException(
    u16_t wNwk,
    u8_p pbyBuffer
) {
    u16_t wAttributeID  =  BigWord(&pbyBuffer);
    u8_t byStatus       = *pbyBuffer++;
    if(wAttributeID != ATTRID_BASIC_MODEL_ID || byStatus != ZCL_STATUS_SUCCESS) return;
    u8_t byAttributeDataType  = *pbyBuffer++;
    u8_t byAttributeDataTypeSize = ZbDeviceDb::GetAttributeDataSize(byAttributeDataType, &pbyBuffer);
    u8_p pbyAttributeData = new u8_t[byAttributeDataTypeSize + 1];
    bzero(pbyAttributeData, byAttributeDataTypeSize + 1);
    memcpy(pbyAttributeData, pbyBuffer, byAttributeDataTypeSize);
    String ModelName = String((const char*) pbyAttributeData);
    delete pbyAttributeData;

    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    if(device.Modify() != NULL) return;
    if(ModelName == "LM-DOOR" ||
            ModelName == "LM-PIR") {
        Device_t first = ZbDriver::s_pZbModel->Add(new ZbDeviceDb());
        first.Modify()->DeviceID = wNwk;
        first.Modify()->Network = wNwk;
        first.Modify()->MAC = ZbZdoCmd::s_mapEPInfo[wNwk].MAC;
        first.Modify()->Model = ModelName;
        first.Modify()->Manufacturer = String("Lumi R&D");
        first.Modify()->Endpoint = 1;
        if(ModelName == "LM-DOOR") {
            first.Modify()->Type = ZCL_HA_DEVICEID_DOOR_LOCK;
        } else {
            first.Modify()->Type = ZCL_HA_DEVICEID_IAS_ZONE;
        }
        first.Modify()->ControllerID = 1;

        Vector<int_t> vecType;
        vecType.push_back(ZCL_HA_DEVICEID_LIGHT_SENSOR);
        vecType.push_back(ZCL_HA_DEVICEID_TEMPERATURE_SENSOR);
        vecType.push_back(ZCL_HA_DEVICEID_THERMOSTAT);
        vecType.push_back(ZCL_LUMI_DEVICEID_POWER);
        vecType.push_back(22);

        for(int_t i = 0; i < 5; i++) {
            Device_t other = ZbDriver::s_pZbModel->Add(new ZbDeviceDb());
            other.Modify()->DeviceID = wNwk;
            other.Modify()->Network = wNwk;
            other.Modify()->MAC = ZbZdoCmd::s_mapEPInfo[wNwk].MAC;
            other.Modify()->Model = ModelName;
            other.Modify()->Manufacturer = String("Lumi R&D");
            if(i == 4) {
                other.Modify()->Endpoint = 49;
            } else {
                other.Modify()->Endpoint = 2 + i;
            }
            other.Modify()->Type = vecType[i];
            other.Modify()->ControllerID = 1;
        }

    }
    ZbDriver::s_pZbModel->UpdateChanges();
    ZbDriver::GetInstance()->Init(false);
    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
    ZbSocketCmd::GetInstance()->SendLstAdd(devices);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::RequestDevicesState(
    Device_t device
) {
    Json::Value jsonDev, jsonVal;
    jsonDev["devid"] = std::to_string(device->DeviceID.GetValue());
    jsonDev["ord"] = std::to_string(device->Endpoint.GetValue());
    jsonDev["net"] = std::to_string(1);
    jsonDev["type"] = std::to_string(device->RealType);
    jsonVal["dev"].append(jsonDev);
    JsonCommand_p pJsonCommand = new JsonCommand(String("dev"), String("get"));
    pJsonCommand->SetJsonObject(jsonVal);
    JsonDevGet_p pJsonDevGet = new JsonDevGet();
    pJsonDevGet->ParseJsonCommand(pJsonCommand);
    ZbMessage_p pZbMessage = new ZbMessage(pJsonDevGet, ZbMessage::Command::GetDevice);
    pZbMessage->SetCmdID(ZCL_CMD_REQ);
    ZbDriver::GetInstance()->ProcSendMessage(pZbMessage);
    pZbMessage = NULL;
    delete pJsonCommand;
    delete pJsonDevGet;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclGlobalCmd::SaveDevicesInfo(
    u16_t wNwk
) {
    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Model!=?").Bind(wNwk).Bind(String(""));
    BackupDev_t tmpBu = ZbDriver::s_pZbModel->Find<BackupInfoDb>().Where("MAC=?").Bind(ZbZdoCmd::s_mapEPInfo[wNwk].MAC);
    if(ZbZdoCmd::s_mapEPInfo[wNwk].IsDone != TRUE || device.Modify() == NULL || tmpBu.Modify() != NULL) return;
    BackupDev_t BuDev = ZbDriver::s_pZbModel->Add(new BackupInfoDb());
    BuDev.Modify()->MAC = ZbZdoCmd::s_mapEPInfo[wNwk].MAC;
    BuDev.Modify()->Model = device->Model;
    BuDev.Modify()->Manufacturer = device->Manufacturer;
    BuDev.Modify()->EndpointNo = ZbZdoCmd::s_mapEPInfo[wNwk].byTotalEP;
    int_t i = 1;
    for(Map<u8_t, u16_t>::const_iterator it = ZbZdoCmd::s_mapEPInfo[wNwk].mapType.begin();
            it != ZbZdoCmd::s_mapEPInfo[wNwk].mapType.end(); it++) {
        *(BuDev.Modify()->EpOrd[i]) = it->first + it->second * 1000;
        i++;
    }
    ZbDriver::s_pZbModel->UpdateChanges();
}
