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

#define NETWORK_REQ_TIMEOUT     (5)
#define NETWORK_REQ_NO          (5)

ZbBasicCmd* ZbBasicCmd::s_pInstance = NULL;
bool_t ZbBasicCmd::IsNetworkAvail = FALSE;


/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbBasicCmd::ZbBasicCmd() {
    m_pTimer = RTimer::getTimerInstance();
    m_TimerFunctor = makeFunctor((timerFunctor_p) NULL, *this, &ZbBasicCmd::HandleNetworkInfo);
    m_byNetReqCount = 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbBasicCmd_p
ZbBasicCmd::GetInstance(){
    if(s_pInstance == NULL)
        s_pInstance = new ZbBasicCmd();
    return s_pInstance;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbBasicCmd::~ZbBasicCmd() {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbBasicCmd::HCError(
    u8_t byError, u8_t bySeq
){
    ZbPacket_p pZbPacket = new ZbPacket(2);
    pZbPacket->Push(byError);
    pZbPacket->Push(bySeq);
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
ZbBasicCmd::MCError(
    ZbPacket_p pZbPacket
) {
//    u8_p pBuffer = pZbPacket->GetBuffer();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
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
ZbBasicCmd::NwkInfoReq(){
    ZbPacket_p pZbPacket = new ZbPacket();
    pZbPacket->SetCmdID(NWK_INFO_REQ);
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
    delete pZbPacket;
    m_iTimerHandle = m_pTimer->StartTimer(
            RTimer::Repeat::OneTime, NETWORK_REQ_TIMEOUT, &m_TimerFunctor, NULL);
    m_byNetReqCount++;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbBasicCmd::NwkInfoRsp(
        ZbPacket_p pZbPacket
){
    u8_p pbyBuffer = pZbPacket->GetBuffer();
    String strExPanID = HexToString(pbyBuffer, 8);
    Controller_t controller = ZbDriver::s_pZbModel->Find<ZbControllerDb>().Where("ExPanID=?").Bind(strExPanID);
    pbyBuffer += 8;
    if(controller.Modify() == NULL) {
        ZbCtrllerDb_p pZbCtrller = new ZbControllerDb();
        u16_t wPanID = LittleWord(&pbyBuffer);
        pZbCtrller->ExPanID = strExPanID;
        pZbCtrller->ControllerID = 1; //temporary
        pZbCtrller->PanID = wPanID;
        pZbCtrller->Channel = *pbyBuffer;
        ZbDriver::s_pZbModel->Add(pZbCtrller);
        pZbCtrller = NULL;
        delete pZbCtrller;
    } else {
        controller.Modify()->PanID = LittleWord(&pbyBuffer);
        controller.Modify()->ExPanID = strExPanID;
        controller.Modify()->Channel = *pbyBuffer;
    }
    ZbDriver::s_pZbModel->UpdateChanges();
    IsNetworkAvail = TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbBasicCmd::JoinNwkAllow(
    ZbPacket_p pZbPacket
){
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);

//    ZbZclGlobalCmd::GetInstance()->Broadcast();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbBasicCmd::JoinNwkInfoReq(){
    ZbPacket_p pZbPacket = new ZbPacket();
    pZbPacket->SetCmdID(NWK_JOIN_INFO_REQ);
    ZbDriver::GetInstance()->SendZbPacket(pZbPacket);
    delete pZbPacket;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbBasicCmd::JoinNwkInfoRsp(
        ZbPacket_p pZbPacket
){
    if(*(pZbPacket->GetBuffer()) == 0)
        return FALSE;
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbBasicCmd::ResetDevice(
    u16_t wNwk, u8_t byTime
){
    ZbPacket_p pZbPacket = new ZbPacket(3);
    pZbPacket->SetCmdID(DEV_RESET);
    pZbPacket->Push(wNwk >> 8);
    pZbPacket->Push(wNwk & 0xFF);
    pZbPacket->Push(byTime);
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
ZbBasicCmd::HandleNetworkInfo(
    void_p pBuffer
) {
    if(m_byNetReqCount == NETWORK_REQ_NO) {
        ResetDevice(0x0000, 0x00);
        m_byNetReqCount = 0;
        NwkInfoReq();
        return;
    }

    if(IsNetworkAvail == FALSE) {
        NwkInfoReq();
    }
}

