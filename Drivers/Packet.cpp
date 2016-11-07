#include <algorithm>
#include <iterator>
#include <Packet.hpp>

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet::Packet(
    u32_t dwLength
) {
    m_dwLength = dwLength;
    m_dwCount = 0;
    if (dwLength > 0) {
        m_pByBuffer = new u8_t[dwLength];
    } else {
        m_pByBuffer = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet::~Packet() {
    if (m_pByBuffer != NULL) {
        delete[] m_pByBuffer;
        m_pByBuffer = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::Push(
    u8_t byData
) {
    if (m_dwCount + 1 > m_dwLength) {
        m_dwLength = m_dwCount + 1;
        u8_p temp = new u8_t[m_dwLength];
        for (u32_t i = 0; i < m_dwCount; i++) {
            temp[i] = m_pByBuffer[i];
        }
        delete m_pByBuffer;
        m_pByBuffer = temp;
        temp = NULL;
        delete temp;
    }
    m_pByBuffer[m_dwCount++] = byData;
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::Push(
        u8_p pByBuffer, u32_t dwLength
) {
    if (m_dwCount + dwLength > m_dwLength) {
        m_dwLength = m_dwCount + dwLength;
        u8_p temp = new u8_t[m_dwLength];
        for (u32_t i = 0; i < m_dwCount; i++) {
            temp[i] = m_pByBuffer[i];
        }
        delete m_pByBuffer;
        m_pByBuffer = temp;
        temp = NULL;
        delete temp;
    }
    for (u32_t i = 0; i < dwLength; i++) {
        m_pByBuffer[m_dwCount++] = pByBuffer[i];
    }
    return TRUE;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_p
Packet::GetBuffer() {
    return m_pByBuffer;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
Packet::AtPosition(
    u32_t dwPosition
) {
    return m_pByBuffer[dwPosition];
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
Packet::operator[](
    u32_t dwPosition
) {
    return m_pByBuffer[dwPosition];
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Packet::Count() {
    return m_dwCount;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Packet::Length() {
    return m_dwLength;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::IsEmpty() {
    return (m_dwCount == 0);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::IsFull() {
    return (m_dwCount + 1 > m_dwLength);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Packet::ResetPacket(
    u32_t dwLength
) {
    if (dwLength > 0) {
        if (m_pByBuffer != NULL) {
            delete[] m_pByBuffer;
        }
        m_pByBuffer = new u8_t[dwLength];
        m_dwLength = dwLength;
        m_dwCount = 0;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet&
Packet::operator= (
    Packet& packet
) {
    if (m_pByBuffer != NULL) {
        delete[] m_pByBuffer;
    }
    m_pByBuffer = new u8_t[m_dwLength = packet.Length()];
    m_dwCount = packet.Count();
    for (u32_t i = 0; i < m_dwLength; i++) {
        m_pByBuffer[i] = packet[i];
    }

    return *this;
}
