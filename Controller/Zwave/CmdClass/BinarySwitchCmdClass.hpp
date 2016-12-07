#ifndef BINARYSWITCH_CMDCLASS_HPP_
#define BINARYSWITCH_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class BinarySwitchCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    BinarySwitchCmdClass(u32_t dwHomeId, u8_t byNodeId);

    ValueDevice_p
    HandleBinarySwitchReport(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~BinarySwitchCmdClass();

    virtual u8_t GetMaxVersion() const { return SWITCH_BINARY_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_SWITCH_BINARY_V2; }
    static const String GetZwCmdClassName() { return "COMMAND_CLASS_SWITCH_BINARY"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();

    virtual ZwMessage_p SetValue(bool_t boValue);
    virtual ZwMessage_p SetValue(ValueDevice_p pValueDevice);
};

typedef BinarySwitchCmdClass  BinarySwitchCmdClass_t;
typedef BinarySwitchCmdClass* BinarySwitchCmdClass_p;

#endif /* !BINARYSWITCH_CMDCLASS_HPP_ */
