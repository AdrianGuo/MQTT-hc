#ifndef BASIC_CMDCLASS_HPP_
#define BASIC_CMDCLASS_HPP_

#include "Value.hpp"
#include "ZwCmdClass.hpp"

class BasicCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    BasicCmdClass(u32_t dwHomeID, u8_t byNodeID);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeID, u8_t byNodeID);
    virtual ~BasicCmdClass();

    static const u8_t GetZwCmdClassID();

    virtual u8_t GetMaxVersion() const;

    virtual Value_p HandleMessage(u8_p pInBuffer, u8_t byLength);
    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p SetValue(Value& inParam);
    virtual ZwMessage_p SetValue(u8_t   byParam);
};

typedef BasicCmdClass  BasicCmdClass_t;
typedef BasicCmdClass* BasicCmdClass_p;

#endif /* !BASIC_CMDCLASS_HPP_ */
