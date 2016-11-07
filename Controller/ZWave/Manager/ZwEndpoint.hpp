#ifndef ZWENDPOINT_HPP_
#define ZWENDPOINT_HPP_

#include "typedefs.h"

class ZwEndpoint {
private:
    u8_t m_byGeneric;
    u8_t m_bySpecific;
public:
    ZwEndpoint(u8_t byGeneric = 0, u8_t bySpecific = 0);
    virtual ~ZwEndpoint();

    void_t SetGeneric(u8_t byGeneric) { m_byGeneric = byGeneric; }
    void_t SetSpecific(u8_t bySpecific) { m_bySpecific = bySpecific; }

    u8_t GetGeneric()  const { return m_byGeneric; }
    u8_t GetSpecific() const { return m_bySpecific; }
    u8_t GetTypeEndpoint();
};

typedef ZwEndpoint  ZwEndpoint_t;
typedef ZwEndpoint* ZwEndpoint_p;


#endif /* !ZWENDPOINT_HPP_ */
