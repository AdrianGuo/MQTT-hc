#include <algorithm>
#include <iterator>
#include <Packet.hpp>

/**
 * @func   Packet
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
        m_pbBuffer = new u8_t[dwLength];
    } else {
        m_pbBuffer = NULL;
    }
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet::~Packet() {
    if (m_pbBuffer != NULL) {
        delete[] m_pbBuffer;
        m_pbBuffer = NULL;
    }
}

/**
 * @func   Push
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
            temp[i] = m_pbBuffer[i];
        }
        delete m_pbBuffer;
        m_pbBuffer = temp;
        temp = NULL;
        delete temp;
    }
    m_pbBuffer[m_dwCount++] = byData;
    return TRUE;
}

/**
 * @func   Push
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::Push(
    u8_p  pbBuffer,
    u32_t dwLength
) {
    if (m_dwCount + dwLength > m_dwLength) {
        m_dwLength = m_dwCount + dwLength;
        u8_p temp = new u8_t[m_dwLength];
        for (u32_t i = 0; i < m_dwCount; i++) {
            temp[i] = m_pbBuffer[i];
        }
        delete m_pbBuffer;
        m_pbBuffer = temp;
        temp = NULL;
        delete temp;
    }
    for (u32_t i = 0; i < dwLength; i++) {
        m_pbBuffer[m_dwCount++] = pbBuffer[i];
    }
    return TRUE;
}

/**
 * @func   GetBuffer
 * @brief  None
 * @param  None
 * @retval None
 */
u8_p
Packet::GetBuffer() {
    return m_pbBuffer;
}

/**
 * @func   AtPosition
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
Packet::AtPosition(
    u32_t dwPosition
) {
    return m_pbBuffer[dwPosition];
}

/**
 * @func   operator[]
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
Packet::operator[](
    u32_t dwPosition
) {
    return m_pbBuffer[dwPosition];
}

/**
 * @func   Count
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Packet::Count() {
    return m_dwCount;
}

/**
 * @func   Length
 * @brief  None
 * @param  None
 * @retval None
 */
u32_t
Packet::Length() {
    return m_dwLength;
}

/**
 * @func   IsEmpty
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::IsEmpty() {
    return (m_dwCount == 0);
}

/**
 * @func   IsFull
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
Packet::IsFull() {
    return (m_dwCount + 1 > m_dwLength);
}

/**
 * @func   ResetPacket
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
Packet::ResetPacket(
    u32_t dwLength
) {
    if (dwLength > 0) {
        if (m_pbBuffer != NULL) {
            delete[] m_pbBuffer;
        }
        m_pbBuffer = new u8_t[dwLength];
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
    if (m_pbBuffer != NULL) {
        delete[] m_pbBuffer;
    }
    m_pbBuffer = new u8_t[m_dwLength = packet.Length()];
    m_dwCount = packet.Count();
    for (u32_t i = 0; i < m_dwLength; i++) {
        m_pbBuffer[i] = packet[i];
    }

    return *this;
}
