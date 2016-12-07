#include "debug.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"

#define INDEX_HEADER                0
#define INDEX_LENGTH                1
#define INDEX_TYPE                  2
#define INDEX_COMMAND               3
#define INDEX_PAYLOAD               4

/**
 * @func   CalculateChecksum
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::CalculateChecksum() {
    u8_t byCheckSum = 0xFF;
    byCheckSum ^= (Packet::Count() + 3);
    byCheckSum ^= m_byTypeOfFrame;
    byCheckSum ^= m_byFunctionId;
    for (u32_t i = 0; i < Packet::Count(); i++) {
        byCheckSum ^= Packet::AtPosition(i);
    }
    return byCheckSum;
}

/**
 * @func   IsChecksumValid
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t 
ZwPacket::IsChecksumValid(
    u8_t byChecksum
) {
    return (CalculateChecksum() == byChecksum);
}

/**
 * @func   ZwPacket
 * @brief  None
 * @param  None
 * @retval None
 */
ZwPacket::ZwPacket(
    u32_t byLength
) : Packet (byLength) {
    m_byTypeOfFrame = 0;
    m_byFunctionId = 0;
}

/**
 * @func   ZwPacket
 * @brief  None
 * @param  None
 * @retval None
 */
ZwPacket::ZwPacket(
    u8_t byTypeOfFrame,
    u8_t byCommand,
    u32_t byLength
) : Packet (byLength) {
    m_byTypeOfFrame = byTypeOfFrame;
    m_byFunctionId  = byCommand;
}

/**
 * @func   ~ZwPacket
 * @brief  None
 * @param  None
 * @retval None
 */
ZwPacket::~ZwPacket() {
}

/**
 * @func   SetTypeOfFrame
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwPacket::SetTypeOfFrame(
    u8_t byTypeOfFrame
) {
    m_byTypeOfFrame = byTypeOfFrame;
}

/**
 * @func   SetFunctionID
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwPacket::SetFunctionId(
    u8_t byCommand
) {
    m_byFunctionId = byCommand;
}

/**
 * @func   GetTypeOfFrame
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::GetTypeOfFrame() {
    return m_byTypeOfFrame;
}

/**
 * @func   GetFunctionId
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::GetFunctionId() {
    return m_byFunctionId;
}

/**
 * @func   GetPacket
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p
ZwPacket::GetPacket() {
    Packet_p pPacket = new Packet(Packet::Count());
    pPacket->Push(Packet::GetBuffer(), Packet::Count());
    return pPacket;
}

/**
 * @func   GetFullPacket
 * @brief  None
 * @param  None
 * @retval None
 */
Packet_p
ZwPacket::GetFullPacket() {
    Packet_p pPacket = new Packet(Packet::Count() + 5);
    pPacket->Push(SOF);
    pPacket->Push(Packet::Count() + 3);
    pPacket->Push(m_byTypeOfFrame);
    pPacket->Push(m_byFunctionId);
    pPacket->Push(Packet::GetBuffer(), Packet::Count());
    pPacket->Push(CalculateChecksum());

    return pPacket;
}
