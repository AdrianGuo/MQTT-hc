/*
 * ZbZCLCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <debug.hpp>
#include <zcl.hpp>
#include <zcl_ha.hpp>
#include <zcl_lumi.hpp>
#include <ZbHelper.hpp>

#include <ZbZclCmd.hpp>

ZbZclCmd* ZbZclCmd::s_pInstance = NULL;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZclCmd::ZbZclCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbZclCmd_p
ZbZclCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbZclCmd();
    return s_pInstance;
}

ZbZclCmd::~ZbZclCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclCmd::ProcRecvMessage(
    void_p pInBuffer
) {

    ZbPacket_p pZbPacket    = (ZbPacket_p) pInBuffer;
    u8_p pbyBuffer          = pZbPacket->GetBuffer();
    u16_t wNwk              = LittleWord(&pbyBuffer);
    u8_t byEndpoint         = *pbyBuffer++;
    u16_t wClusterID        = LittleWord(&pbyBuffer);
    switch (wClusterID) {
    case ZCL_CLUSTER_ID_GEN_LEVEL_CONTROL:
        break;

    default:
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
ZbZclCmd::SetDevice(
        ZbPacket_p pZbPacket,
        Device_t device,
        u8_t byValue
){
    u16_t wNwk = (u16_t) device->Network.GetValue();
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push((u8_t) device->Endpoint.GetValue());
    pZbPacket->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID >> 8);
    pZbPacket->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID & 0xFF);
    switch (device->RealType) {
        case LUMI_DEVICE_SWITCH:
            pZbPacket->Push(0x01);    //Payload's length
            pZbPacket->Push(byValue);  //SWITCH: OFF = 0x00; ON = 0x00; TOGGLE = 0x02
            break;

        case LUMI_DEVICE_DIMMER:
        case LUMI_DEVICE_CURTAIN:
        case LUMI_DEVICE_FAN:
            pZbPacket->Push(0x04);    //Payload's length
            pZbPacket->Push(0x00);    //Move to Level
            pZbPacket->Push(byValue);   //Level 0-255
            pZbPacket->Push(0xFF);   //Transition time
            pZbPacket->Push(0xFF);
            break;

        default:
            break;
    }
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbZclCmd::SetIR(
        ZbPacket_p pZbPacket,
        Device_t device,
        IrCommand irCommand,
        u16_t irID
){
    u16_t wNwk = (u16_t) device->Network.GetValue();
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push((u8_t) device->Endpoint.GetValue());
    pZbPacket->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID >> 8);
    pZbPacket->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID & 0xFF);
    if((irCommand == IrCommand::IRCMD_Active) || (irCommand == IrCommand::IRCMD_Delete)) {
        pZbPacket->Push(0x03);                //Payload's length
        pZbPacket->Push((u8_t) irCommand);    //CMD ID
        pZbPacket->Push(irID & 0xFF);    //Ir ID
        pZbPacket->Push(irID >> 8);
    } else {
        pZbPacket->Push(0x01);                //Payload's length
        pZbPacket->Push((u8_t) irCommand);    //CMD ID
    }
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
}
