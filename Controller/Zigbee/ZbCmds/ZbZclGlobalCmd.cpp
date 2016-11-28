/*
 * ZbZCLGlobCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <debug.hpp>
#include <zcl.hpp>
#include <zcl_lumi.hpp>
#include <zcl_general.hpp>
#include <ZbHelper.hpp>
#include <ZbSocketCmd.hpp>
#include <JsonZbGet.hpp>

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
            DEBUG1("ZCL GLOBAL COMMAND NOT FOUND!")
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

        for (Map<u16_t, Vector<DeviceInfo>>::const_iterator_t it = mapDIGroup.begin(); it != mapDIGroup.end(); it++) {
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
            }
           ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
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
    u16_t wNwk              = LittleWord(&pbyBuffer);

    //Devices not in DB will be forced to leave.
    //not have all enpoints joined, forced to leave.
    if(ZbZdoCmd::s_mapEPInfor.find(wNwk) == ZbZdoCmd::s_mapEPInfor.end()){ // ||
            //(ZbZdoCmd::s_mapEPInfor[wNwk].byEPCount != ZbZdoCmd::s_mapEPInfor[wNwk].byTotalEP)) {
        ZbZdoCmd::GetInstance()->LeaveRequest(wNwk);
    }


    u8_t byEndpoint         = *pbyBuffer++;
    u16_t wClusterID        = LittleWord(&pbyBuffer);
    ++pbyBuffer; //CMD ID
    u8_t byLength = *pbyBuffer++; //Payload's length

    Device_t device = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);
    if(device.Modify() == NULL) {
//        ZbZdoCmd::GetInstance()->LeaveRequest(wNwk); //Prevent Spamming!!!
        return;
    }

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

        //DEBUG
        u8_t byAttributeDataTypeSize = ZbDeviceDb::GetAttributeDataSize(byAttributeDataType, &pbyBuffer);
        if((byAttributeDataType == 0x41) || (byAttributeDataType == 0x42)) byLength -= 1;
        else if((byAttributeDataType == 0x43) || (byAttributeDataType == 0x44)) byLength -= 2;
        temp.DP_AttributeDataSize = byAttributeDataTypeSize;

        u8_p pbyAttributeData = new u8_t[byAttributeDataTypeSize + 1];
        bzero(pbyAttributeData, byAttributeDataTypeSize + 1);
        memcpy(pbyAttributeData, pbyBuffer, byAttributeDataTypeSize);

        vpData.push_back(pbyAttributeData);
        pbyAttributeData = NULL;
        delete pbyAttributeData;

        byLength -= byAttributeDataTypeSize;

        for(Action_t::const_iterator_t it = device.Modify()->Action.begin(); it != device.Modify()->Action.end(); it++) {
            if(temp == it->second) {
                temp.DP_DIName = it->first;
            }
        }
        vResponseDP.push_back(temp);

        temp = {};
        if(byLength > 0) {
            pbyBuffer += byAttributeDataTypeSize;
        }

    }

    if(wClusterID == ZCL_CLUSTER_ID_GEN_BASIC) {
        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=?").Bind(wNwk);
//        Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(wNwk).Bind(device->Type.GetValue());
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
                DEBUG2("Device %s has joined.", ModelName.c_str());
                for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
                    Device_t tempDevice = (*it);
                    if(tempDevice.Modify()->IsInterested()) {
                        tempDevice.Modify()->Model = ModelName;
                        tempDevice.Modify()->GenerateDeviceInfo();
                        ZbDriver::s_pZbModel->Add(tempDevice);
                        ZbDriver::s_pZbModel->UpdateChanges();

                        //Request State
                        Json::Value jsonVal;
                        jsonVal["devid"] = std::to_string(tempDevice->DeviceID.GetValue());
                        jsonVal["ord"] = std::to_string(tempDevice->Endpoint.GetValue());
                        JsonCommand_p pJsonCommand = new JsonCommand(String("dev"), String("get"));
                        pJsonCommand->SetJsonObject(jsonVal);
                        JsonZbGet_p pJsonZbGet = new JsonZbGet();
                        pJsonZbGet->ParseJsonCommand(pJsonCommand);
                        ZbMessage_p pZbMessage = new ZbMessage(pJsonZbGet, ZbMessage::Command::GetDevice);
                        pZbMessage->SetCmdID(ZCL_CMD_REQ);
                        ZbDriver::GetInstance()->ProcSendMessage(pZbMessage);
                        pZbMessage = NULL;
                        delete pJsonCommand;
                        delete pJsonZbGet;
                    }

                }
                ZbSocketCmd::GetInstance()->SendLstAdd(devices);
            }
            delete[] vpData[i];
        }
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

    for (Map<u16_t, DeviceProperties>::const_iterator_t it = mapDPGroup.begin(); it != mapDPGroup.end(); it++) {
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
        }

        ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
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
//    device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID = wAttributeID;
//    device.Modify()->SyncDeviceAction();
//    switch (byStatus) {
//        case ZCL_STATUS_SUCCESS:
//            DEBUG2("Write attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//        break;
//
//        case ZCL_STATUS_UNSUPPORTED_ATTRIBUTE:
//            DEBUG2("Attribute %04X of cluster %04X is not supported on device %08d.",
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_READ_ONLY:
//        case ZCL_STATUS_NOT_AUTHORIZED:
//            DEBUG2("Permission denied to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_INVALID_DATA_TYPE:
//            DEBUG2("Invalid datatype to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        case ZCL_STATUS_INVALID_VALUE:
//            DEBUG2("Invalid data to write to attribute %04X of cluster %04X on device %08d.",
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                device->DeviceID.GetValue());
//            break;
//
//        default:
//            DEBUG2("Error on write to attribute %04X of cluster %04X on device %08d.\nThe status result: %02X.",
//                    device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID,
//                    device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID,
//                    device->DeviceID.GetValue(),
//                    byStatus);
//            break;
//    }

}
