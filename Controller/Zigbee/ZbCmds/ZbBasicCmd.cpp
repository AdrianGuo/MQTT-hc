/*
 * ZbBasicCmd.cpp
 *
 *  Created on: Nov 1, 2016
 *      Author: taho
 */

#include <ZbDriver.hpp>
#include <ZbSerialAPI.hpp>
#include <debug.hpp>
#include <zcl_lumi.hpp>
#include <ZbHelper.hpp>

#include <ZbBasicCmd.hpp>

ZbBasicCmd* ZbBasicCmd::s_pInstance = NULL;

void_t
ZbBasicCmd::HCError(
    u8_t byError, u8_t bySeq
){
    ZbPacket_p pZbPacket = new ZbPacket(2);
    pZbPacket->Push(byError);
    pZbPacket->Push(bySeq);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

void_t
ZbBasicCmd::MCError(
    ZbPacket_p pZbPacket
) {
//    u8_p pBuffer = pZbPacket->GetBuffer();
}

void_t
ZbBasicCmd::NwkInit(){
    ZbPacket_p pZbPacket = new ZbPacket(3);
    pZbPacket->SetCmdID(NWK_INIT);
    u8_p pbyPanID = new u8_t[2];
    pbyPanID[0] = 0xFF;
    pbyPanID[1] = 0xFF;
    u8_t byChannel = 11;
    pZbPacket->Push(pbyPanID,2);
    pZbPacket->Push(byChannel);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

void_t
ZbBasicCmd::NwkInfoReq(){
    ZbPacket_p pZbPacket = new ZbPacket();
    pZbPacket->SetCmdID(NWK_INFO_REQ);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

void_t
ZbBasicCmd::NwkInfoRsp(
        ZbPacket_p pZbPacket
){
    u8_p pbyBuffer = pZbPacket->GetBuffer();
    String strExPanID = HexToString(pbyBuffer, 8);

    if(((Controllers_t) ZbDriver::s_pZbModel->Find<ZbControllerDb>().Where("ExPanID=?").Bind(strExPanID)).size() == 0) {
        ZbCtrllerDb_p pZbCtrller = new ZbControllerDb();
        pbyBuffer += 8;
        u16_t wPanID = LittleWord(&pbyBuffer);
        pZbCtrller->ExPanID = strExPanID;
        pZbCtrller->ControllerID = 1; //temporary
        pZbCtrller->PanID = wPanID;
        pZbCtrller->Channel = *pbyBuffer;
        ZbDriver::s_pZbModel->Add(pZbCtrller);
        ZbDriver::s_pZbModel->UpdateChanges();
        pZbCtrller = NULL;
        delete pZbCtrller;
    }
}

void_t
ZbBasicCmd::JoinNwkAllow(
    ZbPacket_p pZbPacket
){
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
}

void_t
ZbBasicCmd::JoinNwkInfoReq(){
    ZbPacket_p pZbPacket = new ZbPacket();
    pZbPacket->SetCmdID(NWK_JOIN_INFO_REQ);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

bool_t
ZbBasicCmd::JoinNwkInfoRsp(
        ZbPacket_p pZbPacket
){
    if(*(pZbPacket->GetBuffer()) == 0)
        return FALSE;
    return TRUE;
}

void_t
ZbBasicCmd::ResetDevice(
    Device_t device, u8_t byTime
){
    u16_t wNwk = device->Network.GetValue();
    ZbPacket_p pZbPacket = new ZbPacket(3);
    pZbPacket->SetCmdID(DEV_RESET);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk | 0xFF);
    pZbPacket->Push(byTime);
    ZbDriver::s_pInstance->m_pSZbSerial->PushZbPacket(pZbPacket);
    delete pZbPacket;
}

ZbBasicCmd::ZbBasicCmd() {
}

ZbBasicCmd_p
ZbBasicCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbBasicCmd();
    return s_pInstance;
}

ZbBasicCmd::~ZbBasicCmd() {
}


