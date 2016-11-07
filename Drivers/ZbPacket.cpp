/*
 * ZbPacket.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: taho
 */
#include <ZbSerialAPI.hpp>
#include <ZbPacket.hpp>

#define INDEX_HEADER                0
#define INDEX_LENGTH                1
#define INDEX_SEQ                  2
#define INDEX_COMMAND               3
#define INDEX_PAYLOAD               4
u8_t ZbPacket::m_bySeqOfFrame = 0;
/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZbPacket::CalculateChecksum() {
    u8_t byCheckSum = 0x00;
//    byCheckSum ^= (Packet::Count() + 3);
//    byCheckSum ^= m_bySeqOfFrame;
    byCheckSum ^= m_byCmdID;
    for (u32_t i = 0; i < Packet::Count(); i++) {
        byCheckSum ^= Packet::AtPosition(i);
    }
    return byCheckSum;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ZbPacket::IsChecksumValid(
    u8_t byChecksum
) {
    return (CalculateChecksum() == byChecksum);
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbPacket::ZbPacket(
    u32_t byLength
    ) : Packet (byLength) {
    if (m_bySeqOfFrame > (0xFF - 1))
        m_bySeqOfFrame = 0;
    else
        ++m_bySeqOfFrame;
    m_byCmdID = 0;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbPacket::ZbPacket(
    u8_t bySeqOfFrame,
    u8_t byCommand,
    u32_t byLength
) : Packet (byLength) {
    if (++m_bySeqOfFrame > 0xFF) {
        if (m_bySeqOfFrame > (0xFF - 1))
            m_bySeqOfFrame = 0;
        else
            ++m_bySeqOfFrame;
    } else
        m_bySeqOfFrame = bySeqOfFrame;
    m_byCmdID = byCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZbPacket::~ZbPacket() {}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbPacket::SetSeqOfFrame(
    u8_t bySeqOfFrame
) {
    m_bySeqOfFrame = bySeqOfFrame;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZbPacket::SetCmdID(
    u8_t byCommand
) {
    m_byCmdID = byCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZbPacket::GetSeqOfFrame() {
    return m_bySeqOfFrame;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZbPacket::GetCmdID() {
    return m_byCmdID;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p
ZbPacket::GetFullPacket() {
    Packet_p pPacket = new Packet(Packet::Count() + 6);
    pPacket->Push(SOF1);
    pPacket->Push(SOF2);
    pPacket->Push(Packet::Count() + 3);
    pPacket->Push(m_bySeqOfFrame);
    pPacket->Push(m_byCmdID);
    pPacket->Push(Packet::GetBuffer(), Packet::Count());
    pPacket->Push(CalculateChecksum());

    return pPacket;
}



