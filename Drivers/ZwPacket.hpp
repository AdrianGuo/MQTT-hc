#ifndef ZWPACKET_HPP_
#define ZWPACKET_HPP_

#include "Typedefs.h"
#include "String.hpp"
#include "Packet.hpp"

class ZwPacket : public Packet {
private:
    u8_t m_byTpeOfFrame;
    u8_t m_byFunctionId;

    u8_t CalculateChecksum();

public:
    ZwPacket(u32_t dwLenOfFrame = 0);
    ZwPacket(u8_t  byTpeOfFrame,
             u8_t  byFunctionId,
             u32_t dwLenOfFrame);
    virtual ~ZwPacket();
    
    u8_t GetTpeOfFrame();
    u8_t GetFunctionId();

    void_t SetTpeOfFrame(u8_t byTpeOfFrame);
    void_t SetFunctionId(u8_t byFunctionId);

    bool_t IsChecksumValid(u8_t byChecksum);
    Packet_p GetPacket();
    Packet_p GetFullPacket();
    String PrintfPacket();
};

typedef ZwPacket  ZwPacket_t;
typedef ZwPacket* ZwPacket_p;

#endif /* !ZWPACKET_HPP_ */
