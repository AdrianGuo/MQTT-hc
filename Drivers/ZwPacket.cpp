#include "debug.hpp"
#include "ZwSerialAPI.hpp"
#include "ZwPacket.hpp"

#define INDEX_HEADER                0
#define INDEX_LENGTH                1
#define INDEX_TYPE                  2
#define INDEX_COMMAND               3
#define INDEX_PAYLOAD               4

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::CalculateChecksum() {
    u8_t byCheckSum = 0xFF;
    byCheckSum ^= (Packet::Count() + 3);
    byCheckSum ^= m_byTypeOfFrame;
    byCheckSum ^= m_byFunctionID;
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
ZwPacket::IsChecksumValid(
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
ZwPacket::ZwPacket(
    u32_t byLength
) : Packet (byLength) {
    m_byTypeOfFrame = 0;
    m_byFunctionID = 0;
}

/**
 * @func
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
    m_byFunctionID  = byCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwPacket::~ZwPacket() {
    //DEBUG1("delete ZwPacket");
}

/**
 * @func
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
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ZwPacket::SetFunctionID(
    u8_t byCommand
) {
    m_byFunctionID = byCommand;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::GetTypeOfFrame() {
    return m_byTypeOfFrame;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
u8_t
ZwPacket::GetFunctionID() {
    return m_byFunctionID;
}

/**
 * @func
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
 * @func
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
    pPacket->Push(m_byFunctionID);
    pPacket->Push(Packet::GetBuffer(), Packet::Count());
    pPacket->Push(CalculateChecksum());

    return pPacket;
}
