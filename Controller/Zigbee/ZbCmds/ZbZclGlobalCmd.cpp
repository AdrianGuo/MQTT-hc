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
//    u16_t wNwk              = LittleWord(&pbyBuffer);
//    u8_t byEndpoint         = *pbyBuffer++;
//    u16_t wClusterID        = LittleWord(&pbyBuffer);

//    Device_t device          = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Endpoint=?").Bind(wNwk).Bind(byEndpoint);
//    if(device.Modify() == NULL) { return; }
//    device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID = wClusterID;

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
        u8_t numofDIs,
        ...
){
    va_list listDI;
    va_start(listDI, numofDIs);
    Vector<DeviceInfo> vDI;
    for (u16_t i = 0; i < numofDIs; i++) {
        int devInfo = va_arg(listDI, int);
        if(device->Action.find((DeviceInfo) devInfo) == device->Action.end()) { continue; }
        vDI.push_back((DeviceInfo) devInfo);
    }
    va_end(listDI);

    for(int_t i = 0; i < (u8_t) vDI.size(); i++) {

    }

    if(vDI.size() == 0)  { return; }

    if(device.Modify()->IsInterested()) {
        u16_t wNwkAdd = (u16_t) device->Network.GetValue();
        ZbPacket_p pZbPacket = new ZbPacket(9);
        pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
        pZbPacket->Push(wNwkAdd >> 8);
        pZbPacket->Push(wNwkAdd & 0xFF);
        pZbPacket->Push((u8_t) device->Endpoint.GetValue());
        pZbPacket->Push(device.Modify()->Action[vDI[0]].DP_ClusterID >> 8);
        pZbPacket->Push(device.Modify()->Action[vDI[0]].DP_ClusterID & 0xFF);
        pZbPacket->Push(ZCL_CMD_READ);
        pZbPacket->Push(0x02 * vDI.size()); //Payload's length
        for(u8_t i = 0;  i < (u8_t) vDI.size(); i++) {
            pZbPacket->Push(device.Modify()->Action[vDI[i]].DP_AttributeID & 0xFF);
            pZbPacket->Push(device.Modify()->Action[vDI[i]].DP_AttributeID >> 8);
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
ZbZclGlobalCmd::ReadAttributeResponse(
    u8_p pbyBuffer
){
//    u16_t wNwk              = LittleWord(&pbyBuffer);
//    u8_t byEndpoint         = *pbyBuffer++;
//    u16_t wClusterID        = LittleWord(&pbyBuffer);
//    ++pbyBuffer; //CMD ID
//    u8_t byLength = *pbyBuffer++; //Payload's length
//
//    Vector<DeviceProperty> vResponseDI;
//
//    while (byLength != 0) {
//        u16_t wAttributeID  =  BigWord(&pbyBuffer);
//        byLength-=2;
//
//        DeviceProperty temp;
//        temp.DP_ClusterID   = wClusterID;
//        temp.DP_AttributeID = wAttributeID;
//        vResponseDI.push_back(temp);
//
//        u8_t byStatus       = *pbyBuffer++;
//        byLength--;
//        if(byStatus != ZCL_STATUS_SUCCESS) { continue; }
//
//
//
//    }
//    u16_t wAttributeID  =  BigWord(&pbyBuffer);
//    device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeID = wAttributeID;
////    DEBUG2("xxxxxxxxxxxxxxxxx %d, %d", device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID, wAttributeID);
//    device.Modify()->SyncDeviceAction();
//    u8_t byStatus       = *pbyBuffer++;
//    switch (byStatus) {
//        case ZCL_STATUS_SUCCESS: {
//            u8_t byAttributeDataType     = *pbyBuffer++; //Data type
//            if(device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeDataType != byAttributeDataType) {
//                DEBUG2("AttributeDataType not match: %d != %d", byAttributeDataType, device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeDataType); break;
//            }
//            u8_t byAttributeDataTypeSize = ZbDeviceDb::GetAttributeDataSize(byAttributeDataType, &pbyBuffer);
////            if(device.Modify()->Action[DeviceInfo::DI_Using].DP_AttributeDataSize != byAttributeDataTypeSize) break; //Problem with non-fixed datatype like string, etc.
//            u8_p pbyAttributeData       = new u8_t[byAttributeDataTypeSize + 1];
//            bzero(pbyAttributeData, byAttributeDataTypeSize + 1);
//            memcpy(pbyAttributeData, pbyBuffer, byAttributeDataTypeSize);
//            if(device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID != ZCL_CLUSTER_ID_GEN_BASIC) {
//
//                device.Modify()->ReceiveInforFromDevice(byAttributeDataType, pbyAttributeData);
//
//            } else if (device.Modify()->Action[DeviceInfo::DI_Using].DP_ClusterID == ZCL_CLUSTER_ID_GEN_BASIC) {
//
//                if (wAttributeID == ATTRID_BASIC_MANUFACTURER_NAME) {
//                    device.Modify()->Manufacturer = String((const char*) pbyAttributeData);
//                    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(device->Network.GetValue()).Bind(device->Type.GetValue());
//                    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
//                        Device_t tempDevice = (*it);
//                        tempDevice.Modify()->Manufacturer = device->Manufacturer;
//                        ZbDriver::s_pZbModel->Add(tempDevice);
//                        ZbDriver::s_pZbModel->UpdateChanges();
//                    }
//                }
//
//                if (wAttributeID == ATTRID_BASIC_MODEL_ID) {
//                    device.Modify()->Model = String((const char*) pbyAttributeData);
//                    Devices_t devices = ZbDriver::s_pZbModel->Find<ZbDeviceDb>().Where("Network=? AND Type=?").Bind(device->Network.GetValue()).Bind(device->Type.GetValue());
//                    for (Devices_t::const_iterator it = devices.begin(); it != devices.end(); it++) {
//                        Device_t tempDevice = (*it);
//                        tempDevice.Modify()->Model = device->Model;
//                        ZbDriver::s_pZbModel->Add(tempDevice);
//                        ZbDriver::s_pZbModel->UpdateChanges();
//                        tempDevice.Modify()->GenerateDeviceInfo();
//                        ZbZclGlobalCmd::s_pInstance->ReadAttributeRequest(tempDevice, 1, DeviceInfo::DI_State);
//                    }
//                    ZbSocketCmd::GetInstance()->SendLstAdd(devices);
//                }
//            }
//
//            delete pbyAttributeData;
//        }
//            break;
//
//        case ZCL_STATUS_UNSUPPORTED_ATTRIBUTE:
//        default:
//            DEBUG1("ZCL STATUS UNSUPPORTED ATTRIBUTE");
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
ZbZclGlobalCmd::WriteAttributeRequest(
        Device_t device,
        DeviceInfo deviceInfo,
        void_p pAttributeData //Must pass a reference
) {
    if(pAttributeData == NULL) { return; }
    if(device->Action.find(deviceInfo) == device->Action.end()) { return; }
    if(device->RealType > 0) {
        u16_t wNwkAdd = (u16_t) device->Network.GetValue();
        ZbPacket_p pZbPacket = new ZbPacket(10 + device.Modify()->Action[deviceInfo].DP_AttributeDataSize);
        pZbPacket->SetCmdID(ZCL_GLOBAL_CMD_REQ);
        pZbPacket->Push(wNwkAdd >> 8);
        pZbPacket->Push(wNwkAdd & 0xFF);
        pZbPacket->Push((u8_t) device->Endpoint.GetValue());
        pZbPacket->Push(device.Modify()->Action[deviceInfo].DP_ClusterID >> 8);
        pZbPacket->Push(device.Modify()->Action[deviceInfo].DP_ClusterID & 0xFF);
        pZbPacket->Push(ZCL_CMD_WRITE);
        pZbPacket->Push(0x03 + device.Modify()->Action[deviceInfo].DP_AttributeDataSize); //Payload's length
        pZbPacket->Push(device.Modify()->Action[deviceInfo].DP_AttributeID & 0xFF);
        pZbPacket->Push(device.Modify()->Action[deviceInfo].DP_AttributeID >> 8);
        pZbPacket->Push(device.Modify()->Action[deviceInfo].DP_AttributeDataType);
        //Push Attribute Data
        for (int_t i = 0; i< device.Modify()->Action[deviceInfo].DP_AttributeDataSize; i++) {
            pZbPacket->Push(((u8_p) pAttributeData)[i]);
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
