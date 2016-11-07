#ifndef ASSOCIATION_CMDCLASS_HPP_
#define ASSOCIATION_CMDCLASS_HPP_

#include "Value.hpp"
#include "ZwCmdClass.hpp"

class AssociationCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    AssociationCmdClass(u32_t dwHomeID, u8_t byNodeID);
public:
    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeID, u8_t byNodeID);
    virtual ~AssociationCmdClass();

    static const u8_t GetZwCmdClassID();

    virtual u8_t GetMaxVersion() const;

    virtual Value_p HandleMessage(u8_p pInBuffer, u8_t byLength);
    virtual ZwMessage_p GetValue(Value& inParam);
    virtual ZwMessage_p SetValue(Value& inParam);

    virtual ZwMessage_p Get(u8_t byGroupID);
    virtual ZwMessage_p Set(u8_t byGroupID, u8_t byNodeID);
    virtual ZwMessage_p Remove(u8_t byGroupID, u8_t byNodeID);
};

typedef AssociationCmdClass  AssociationCmdClass_t;
typedef AssociationCmdClass* AssociationCmdClass_p;


#endif /* !ASSOCIATION_CMDCLASS_HPP */
