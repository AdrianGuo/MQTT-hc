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

ZbZclCmd::ZbZclCmd() {
}

ZbZclCmd_p
ZbZclCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbZclCmd();
    return s_pInstance;
}

ZbZclCmd::~ZbZclCmd() {
}

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

void_t
ZbZclCmd::SetDevice(
        ZbMessage_p pZbMessage,
        Device_t device,
        u8_t byValue
){

    u16_t wNwk = (u16_t) device->Network.GetValue();
    pZbMessage->Push(wNwk >> 8);
    pZbMessage->Push(wNwk & 0xFF);
    pZbMessage->Push((u8_t) device->Endpoint.GetValue());
    pZbMessage->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID >> 8);
    pZbMessage->Push(device.Modify()->Action[DeviceInfo::DI_State].DP_ClusterID & 0xFF);
    switch (device->RealType) {
        case LUMI_DEVICE_SWITCH:
            pZbMessage->Push(0x01);    //Payload's length
            pZbMessage->Push(byValue);  //OFF = 0x00; ON = 0x00; TOGGLE = 0x02
            break;

        case LUMI_DEVICE_DIMMER:
        case LUMI_DEVICE_CURTAIN:
            pZbMessage->Push(0x04);    //Payload's length
            pZbMessage->Push(0x00);    //Move to Level
            if(byValue <= 55)
                byValue *= 3;
            else if(byValue > 55)
                byValue = 165 + (byValue - 55) * 2;
            else
                byValue = 0;
            pZbMessage->Push(byValue);   //Level 0-255
            pZbMessage->Push(0xFF);   //Transition time
            pZbMessage->Push(0xFF);
            break;

        case LUMI_DEVICE_FAN:
            pZbMessage->Push(0x04);    //Payload's length
            pZbMessage->Push(0x00);    //Move to Level
            if(byValue < 4)
                byValue *= 63;
            else if(byValue == 4)
                byValue = 255;
            else
                byValue = 0;
            pZbMessage->Push(byValue);   //Level 0-255
            pZbMessage->Push(0xFF);   //Transition time
            pZbMessage->Push(0xFF);
            break;

        default:
            return;
    }
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket((ZbPacket_p) pZbMessage);
}


