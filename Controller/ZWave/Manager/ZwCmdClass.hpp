#ifndef CMDCLASS_HPP_
#define CMDCLASS_HPP_

#include <ZwMessage.hpp>
#include "typedefs.h"
#include "Functor.hpp"
#include "Value.hpp"

class ZwCmdClass {
private:
    u32_t   m_dwHomeID;
    u8_t    m_byNodeID;
    u8_t    m_byVersion;
    bool_t  m_boSecured;
public:
    ZwCmdClass(u32_t dwHomeID = 0, u8_t byNodeID = 0);
    virtual ~ZwCmdClass();

    virtual u8_t    GetMaxVersion() const = 0;
    virtual Value_p HandleMessage(u8_p pBuffer, u8_t byLength);
    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p GetValue(Value& inParam);
    virtual ZwMessage_p SetValue(Value& inParam);

    u32_t GetHomeID() const;
    u8_t  GetNodeID() const;
    u8_t  GetEndpointID();
};

typedef ZwCmdClass  ZwCmdClass_t;
typedef ZwCmdClass* ZwCmdClass_p;

#endif /* !CMDCLASS_HPP_ */
