#ifndef SWITCHMULTILEVEL_CMDCLASS_HPP_
#define SWITCHMULTILEVEL_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class MultilevelSwitchCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    MultilevelSwitchCmdClass(u32_t dwHomeId, u8_t byNodeId);

    ValueDevice_p
    HandleMultilevelSwitchReport(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~MultilevelSwitchCmdClass();

    virtual u8_t GetMaxVersion() const { return SWITCH_MULTILEVEL_VERSION_V4; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_SWITCH_MULTILEVEL_V4; }
    static const String GetZwCmdClassName() { return "SWITCH_MULTILEVEL"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();

    virtual ZwMessage_p SetValue(u8_t byLevel);
    virtual ZwMessage_p SetValue(ValueDevice_p pValueDevice);
};

typedef MultilevelSwitchCmdClass  MultilevelSwitchCmdClass_t;
typedef MultilevelSwitchCmdClass* MultilevelSwitchCmdClass_p;

#endif /* !SWITCHMULTILEVEL_CMDCLASS_HPP_ */
