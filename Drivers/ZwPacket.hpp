#ifndef ZWPACKET_HPP_
#define ZWPACKET_HPP_

#include "Packet.hpp"

class ZwPacket : public Packet {
private:
    u8_t m_byTypeOfFrame;
    u8_t m_byFunctionId;

    u8_t CalculateChecksum();

public:
    ZwPacket(u32_t byLength = 0);
    ZwPacket(u8_t byTypeOfFrame,
             u8_t byFunctionID,
             u32_t byLength);
    virtual ~ZwPacket();
    
    u8_t GetTypeOfFrame();
    u8_t GetFunctionId();

    void_t SetTypeOfFrame(u8_t byTypeOfFrame);
    void_t SetFunctionId(u8_t byFunctionID);

    bool_t IsChecksumValid(u8_t byChecksum);
    Packet_p GetPacket();
    Packet_p GetFullPacket();
};

typedef ZwPacket  ZwPacket_t;
typedef ZwPacket* ZwPacket_p;

#endif /* !ZWPACKET_HPP_ */
