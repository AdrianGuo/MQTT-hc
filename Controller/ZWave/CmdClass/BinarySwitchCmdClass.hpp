#ifndef BINARYSWITCH_CMDCLASS_HPP_
#define BINARYSWITCH_CMDCLASS_HPP_

#include "Value.hpp"
#include "ZwCmdClass.hpp"

class BinarySwitchCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    BinarySwitchCmdClass(u32_t dwHomeID, u8_t byNodeID);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeID, u8_t byNodeID);
    virtual ~BinarySwitchCmdClass();

    static const u8_t GetZwCmdClassID();

    virtual u8_t    GetMaxVersion() const;

    virtual Value_p HandleMessage(u8_p pInBuffer, u8_t byLength);
    virtual ZwMessage_p GetValue();
    virtual ZwMessage_p SetValue(Value& inParam);
    virtual ZwMessage_p SetValue(bool_t boParam);
};

typedef BinarySwitchCmdClass  BinarySwitchCmdClass_t;
typedef BinarySwitchCmdClass* BinarySwitchCmdClass_p;

#endif /* !BINARYSWITCH_CMDCLASS_HPP_ */
