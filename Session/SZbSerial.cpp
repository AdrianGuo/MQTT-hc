/*
 * SZbSerial.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: taho
 */

#include <debug.hpp>
#include <ZbSerialAPI.hpp>
#include <SZbSerial.hpp>

#define MAX_FRAME_SIZE                  (88)
#define MIN_FRAME_SIZE                  (3)

#define MAX_RETRIES                     3
#define WAIT_ACK1                       1000
#define WAIT_ACK2                       1000

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SZbSerial::SZbSerial(
    const_char_p chPortname
) : m_Serial(chPortname, BAUD192) {
    m_enuRecvState = ZB_FRS_SOF_HUNT1;
    m_boIsACKNeeded = FALSE;
    m_boIsReceivedCAN = FALSE;
    m_bySendAttempts = 0;

    m_pZbPacket = NULL;
    m_pDriverRecvFunctor = NULL;

    m_serialSendFunctor = makeFunctor((SerialFunctor_p)NULL, *this, &SZbSerial::BufferToZbPacket);
    m_Serial.SerialRecvFunctor(&m_serialSendFunctor);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
SZbSerial::~SZbSerial() {
    if (m_pDriverRecvFunctor != NULL) {
        delete m_pDriverRecvFunctor;
        m_pDriverRecvFunctor = NULL;
    }

    if (m_pZbPacket != NULL) {
        delete m_pZbPacket;
        m_pZbPacket = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZbSerial::RecvFunctor(
    DriverFunctor_p pDriverFunctor
) {
    if (pDriverFunctor != NULL) {
        m_pDriverRecvFunctor = pDriverFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZbSerial::BufferToZbPacket(
    u8_p pBuffer,
    u32_t dwLen
) {
    for (u32_t i = 0; i < dwLen; i++) {
        ParseData(pBuffer[i]);
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZbSerial::ParseData(
    u8_t byData
) {
    switch (m_enuRecvState) {
    case ZB_FRS_SOF_HUNT1:
            if (byData == SOF1) {
                m_enuRecvState = ZB_FRS_SOF_HUNT2;
            } else if (m_boIsACKNeeded) {
                if (ACK == byData) {
                    m_boIsReceivedCAN = FALSE;
                    m_ACKSignal.Set();
                    DEBUG1("recv ack");
                } else if (NAK == byData) {
                    m_boIsReceivedCAN = FALSE;
                    DEBUG1("recv nak");
                } else if (CAN == byData) {
                    DEBUG1("recv can");
                    m_boIsReceivedCAN = TRUE;
                }
                m_boIsACKNeeded = FALSE;
            }
            break;

    case ZB_FRS_SOF_HUNT2:
        if (byData == SOF2) {
            m_enuRecvState = ZB_FRS_LENGTH;
        } else {
            m_enuRecvState = ZB_FRS_SOF_HUNT1;
        }
        break;

    case ZB_FRS_LENGTH:
            m_pZbPacket = new ZbPacket(byData - 3);
            m_enuRecvState = ZB_FRS_SEQ;
        break;

    case ZB_FRS_SEQ:
        if ((byData < MIN_SEQ) || (byData > MAX_SEQ)) {
            m_enuRecvState = ZB_FRS_SOF_HUNT1; // Reset Receiver State
            delete m_pZbPacket;
            m_pZbPacket = NULL;
        } else {
            m_enuRecvState = ZB_FRS_COMMAND;
            m_pZbPacket->SetSeqOfFrame(byData);
        }
        break;

    case ZB_FRS_COMMAND:
        m_pZbPacket->SetCmdID(byData);
        if (m_pZbPacket->IsFull()) {
            m_enuRecvState = ZB_FRS_CHECKSUM;
        } else {
            m_enuRecvState = ZB_FRS_PAYLOAD;
        }
        break;

    case ZB_FRS_PAYLOAD:
        if (!m_pZbPacket->Push(byData)) {
            m_enuRecvState = ZB_FRS_SOF_HUNT1;
        } else if (m_pZbPacket->IsFull()) {
            m_enuRecvState = ZB_FRS_CHECKSUM;
        }
        break;

    case ZB_FRS_CHECKSUM:
        if (m_pZbPacket->IsChecksumValid(byData)) {
            if(m_boIsACKNeeded){
                DEBUG1("push ack");
                m_Serial.PushData(ACK);
            }

            if (m_pDriverRecvFunctor != NULL) {

                printf("Received packet: ");
                for (u32_t i = 0; i < m_pZbPacket->GetFullPacket()->Length(); i++)
                    printf("%02X ", m_pZbPacket->GetFullPacket()->GetBuffer()[i]);
                printf("\n");

                m_pDriverRecvFunctor->operator ()(m_pZbPacket);

                m_pZbPacket = NULL;
            }

        } else {
            DEBUG1("WRONG CHECKSUM");
            ZbPacket_p pZbPacket = new ZbPacket(2);
            pZbPacket->SetCmdID(0x80);
            pZbPacket->Push(0x01);
            pZbPacket->Push(m_pZbPacket->GetSeqOfFrame());
            PushZbPacket(pZbPacket);
            delete pZbPacket;
            delete m_pZbPacket;
            m_pZbPacket = NULL;
        }
        m_enuRecvState = ZB_FRS_SOF_HUNT1;
        break;

    case ZB_FRS_RX_TIMEOUT:
    default:
        m_enuRecvState = ZB_FRS_SOF_HUNT1;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZbSerial::Start() {
    return m_Serial.Start();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZbSerial::Connect() {
    return m_Serial.Connect();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZbSerial::Close() {
    return m_Serial.Close();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZbSerial::PushZbPacket(
    ZbPacket_p pZbPacket
) {
    m_boIsACKNeeded = FALSE;
    m_bySendAttempts = 0;
    SendZbPacket(pZbPacket);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZbSerial::SendZbPacket(
    ZbPacket_p pZbPacket
) {
    if (m_bySendAttempts < MAX_RETRIES) {
        m_Serial.PushPacket(pZbPacket->GetFullPacket());
        if (m_boIsACKNeeded) {
            m_ACKSignal.Reset();
            if (!m_ACKSignal.Wait(WAIT_ACK1)) {
                m_Serial.PushPacket(pZbPacket->GetFullPacket());
                if (!m_ACKSignal.Wait(WAIT_ACK2)) {
                    m_Serial.PushPacket(pZbPacket->GetFullPacket());
                } else if (m_boIsReceivedCAN) {
                    m_bySendAttempts++;
                    SendZbPacket(pZbPacket);
                }
            } else if (m_boIsReceivedCAN) {
                m_bySendAttempts++;
                SendZbPacket(pZbPacket);
            }
        }
    }
}


