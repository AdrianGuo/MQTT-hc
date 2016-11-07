/*
 * ZbPacket.hpp
 *
 *  Created on: Sep 10, 2016
 *      Author: taho
 */

#ifndef DRIVERS_ZBPACKET_HPP_
#define DRIVERS_ZBPACKET_HPP_

#include "Packet.hpp"

class ZbPacket : public Packet {
private:
    u8_t m_byCmdID;
    u8_t CalculateChecksum();

public:
    static u8_t m_bySeqOfFrame;
    ZbPacket(u32_t byLength = 0);
    ZbPacket(u8_t bySeqOfFrame,
             u8_t byCmdID,
             u32_t byLength);
    ~ZbPacket();

    u8_t        GetSeqOfFrame();
    u8_t        GetCmdID();

    void_t      SetSeqOfFrame(u8_t bySeqOfFrame);
    void_t      SetCmdID(u8_t byCmdID);

    bool_t      IsChecksumValid(u8_t byChecksum);
    Packet_p    GetFullPacket();
};

typedef ZbPacket ZbPacket_t;
typedef ZbPacket* ZbPacket_p;

#endif /* DRIVERS_ZBPACKET_HPP_ */
