#ifndef BASIC_CMDCLASS_HPP_
#define BASIC_CMDCLASS_HPP_

#include "Typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class BasicCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    BasicCmdClass(u32_t dwHomeId, u8_t byNodeId);
    ValueDevice_p
    HandleBasicReport(u8_p pbCommand, u8_t byLength);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~BasicCmdClass();

    virtual u8_t GetMaxVersion() const  { return BASIC_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_BASIC_V2; }
    static const String GetZwCmdClassName() { return "BASIC"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p GetValue();

    virtual ZwMessage_p SetValue(u8_t byValue);
    virtual ZwMessage_p SetValue(ValueDevice_p pValueDevice);
};

typedef BasicCmdClass  BasicCmdClass_t;
typedef BasicCmdClass* BasicCmdClass_p;

#endif /* !BASIC_CMDCLASS_HPP_ */
