#include "LogPlus.hpp"
#include "ZwSerialAPI.hpp"
#include "SZwSerial.hpp"

#define MAX_FRAME_SIZE                  (88)
#define MIN_FRAME_SIZE                  (3)

#define MAX_RETRIES                     3
#define WAIT_ACK1                       1000
#define WAIT_ACK2                       1000

/**
 * @func   SZwSerial
 * @brief  None
 * @param  None
 * @retval None
 */
SZwSerial::SZwSerial(
    const_char_p cPortName
) : m_Serial (cPortName, BAUD1152) {
    m_enuRecvState = FRS_SOF_HUNT;
    m_boIsACKNeeded = FALSE;
    m_boIsReceivedCAN = FALSE;
    m_bySendAttempts = 0;

    m_strNamePort = cPortName;
    m_pZwavePacket = NULL;
    m_pDriverRecvFunctor = NULL;

    m_SerialSendFunctor = makeFunctor((SerialFunctor_p)NULL, *this, &SZwSerial::BufferToZwPacket);
    m_Serial.SerialRecvFunctor(&m_SerialSendFunctor);
}

/**
 * @func   ~SZwSerial
 * @brief  None
 * @param  None
 * @retval None
 */
SZwSerial::~SZwSerial() {
    if (m_pZwavePacket != NULL) {
        delete m_pZwavePacket;
        m_pZwavePacket = NULL;
    }
}

/**
 * @func   GetNamePort
 * @brief  None
 * @param  None
 * @retval None
 */
String
SZwSerial::GetNamePort() {
    return m_strNamePort;
}

/**
 * @func   SZwSerialRecvFunctor
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZwSerial::SZwSerialRecvFunctor(
    ZwDriverRecvMsgFunctor_p pDriverFunctor
) {
    if (pDriverFunctor != NULL) {
        m_pDriverRecvFunctor = pDriverFunctor;
        return TRUE;
    }
    return FALSE;
}

/**
 * @func   Start
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZwSerial::Start() {
    return m_Serial.Start();
}

/**
 * @func   Connect
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZwSerial::Connect() {
    return m_Serial.Connect();
}

/**
 * @func   Close
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
SZwSerial::Close() {
    return m_Serial.Close();
}

/**
 * @func   BufferToZwPacket
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZwSerial::BufferToZwPacket(
    u8_p pbyBuffer,
    u32_t dwLength
) {
    for (u32_t i = 0; i < dwLength; i++) {
        ParseData(pbyBuffer[i]);
    }
}

/**
 * @func   ParseData
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZwSerial::ParseData(
    u8_t byData
) {
    switch (m_enuRecvState) {
    case FRS_SOF_HUNT:
        if (byData == SOF) {
            m_enuRecvState = FRS_LENGTH;
        } else if (m_boIsACKNeeded) {
            if (ACK == byData) {
                m_boIsReceivedCAN = FALSE;
                m_ACKSignal.Set();
            } else if (NAK == byData) {
                m_boIsReceivedCAN = FALSE;
            } else if (CAN == byData) {
                m_boIsReceivedCAN = TRUE;
            }
            m_boIsACKNeeded = FALSE;
        }
        break;

    case FRS_LENGTH:
        if ((byData < MIN_FRAME_SIZE) || (byData > MAX_FRAME_SIZE)) {
            LOG_ERROR("len error");
            m_enuRecvState = FRS_SOF_HUNT;
        } else {
            m_pZwavePacket = new ZwPacket(byData - 3);
            m_enuRecvState = FRS_TYPE;
        }
        break;

    case FRS_TYPE:
        if ((byData == REQUEST) || (byData == RESPONSE)) {
            m_enuRecvState = FRS_COMMAND;
            m_pZwavePacket->SetTpeOfFrame(byData);
        } else {
            m_enuRecvState = FRS_SOF_HUNT;
            delete m_pZwavePacket;
            m_pZwavePacket = NULL;
        }
        break;

    case FRS_COMMAND:
        m_pZwavePacket->SetFunctionId(byData);
        if (m_pZwavePacket->IsFull()) {
            m_enuRecvState = FRS_CHECKSUM;
        } else {
            m_enuRecvState = FRS_PAYLOAD;
        }
        break;

    case FRS_PAYLOAD:
        if (!m_pZwavePacket->Push(byData)) {
            m_enuRecvState = FRS_SOF_HUNT;
        } else if (m_pZwavePacket->IsFull()) {
            m_enuRecvState = FRS_CHECKSUM;
        }
        break;

    case FRS_CHECKSUM:
        if (m_pZwavePacket->IsChecksumValid(byData)) {
            m_Serial.PushData(ACK);
            LOG_INFO("recv %s", m_pZwavePacket->PrintfPacket().c_str());
            if (m_pDriverRecvFunctor != NULL) {
                (*m_pDriverRecvFunctor)(m_pZwavePacket);
                m_pZwavePacket = NULL;
            }
        } else {
            m_Serial.PushData(NAK);
            delete m_pZwavePacket;
            m_pZwavePacket = NULL;
        }
        m_enuRecvState = FRS_SOF_HUNT;
        break;

    case FRS_RX_TIMEOUT:
    default:
        m_enuRecvState = FRS_SOF_HUNT;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZwSerial::PushZwPacket(
    ZwPacket_p pZwPacket
) {
    m_boIsACKNeeded = TRUE;
    m_bySendAttempts = 0;
    SendZwPacket(pZwPacket);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
SZwSerial::SendZwPacket(
    ZwPacket_p pZwPacket
) {
    if (m_bySendAttempts < MAX_RETRIES) {
        m_ACKSignal.Reset();
        m_Serial.PushPacket(pZwPacket->GetFullPacket());
        if (!m_ACKSignal.Wait(WAIT_ACK1)) {
            m_Serial.PushPacket(pZwPacket->GetFullPacket());
            if (!m_ACKSignal.Wait(WAIT_ACK2)) {
                m_Serial.PushPacket(pZwPacket->GetFullPacket());
            } else if (m_boIsReceivedCAN) {
                m_bySendAttempts++;
                SendZwPacket(pZwPacket);
            }
        } else if (m_boIsReceivedCAN) {
            m_bySendAttempts++;
            SendZwPacket(pZwPacket);
        }
    }

    if (pZwPacket != NULL) {
        delete pZwPacket;
        pZwPacket = NULL;
    }
}
