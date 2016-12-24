#ifndef PACKET_HPP_
#define PACKET_HPP_

#include <typedefs.h>

class Packet {
private:
    u8_p    m_pbBuffer;
    u32_t   m_dwLength;
    u32_t   m_dwCount;
public:
    Packet(u32_t dwLength = 0);
    virtual ~Packet();

    virtual bool_t Push(u8_t byData);
    virtual bool_t Push(u8_p pbBuffer, u32_t dwLength);

    virtual u8_p GetBuffer();

    virtual u8_t AtPosition(u32_t dwPosition);
    virtual u8_t operator[](u32_t dwPosition);

    virtual u32_t Count();
    virtual u32_t Length();

    virtual bool_t IsEmpty();
    virtual bool_t IsFull();

    virtual void_t ResetPacket(u32_t dwLength);

    virtual Packet& operator= (Packet& packet);
};

typedef Packet  Packet_t;
typedef Packet* Packet_p;

#endif /* !PACKET_HPP_ */
