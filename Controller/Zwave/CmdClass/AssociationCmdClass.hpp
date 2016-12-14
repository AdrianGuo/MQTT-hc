#ifndef ASSOCIATION_CMDCLASS_HPP_
#define ASSOCIATION_CMDCLASS_HPP_

#include "typedefs.h"
#include "Value.hpp"
#include "ZwDefs.hpp"
#include "ZwCmdClass.hpp"

class AssociationCmdClass : public ZwCmdClass {
private:
    u8_t m_byTransmitOptions;
    AssociationCmdClass(u32_t dwHomeId, u8_t byNodeId);
public:
    typedef struct {
        u8_t id;
    } Node_t, *Node_p;

    static ZwCmdClass_p CreateZwCmdClass(u32_t dwHomeId, u8_t byNodeId);
    virtual ~AssociationCmdClass();

    virtual u8_t GetMaxVersion() const  { return ASSOCIATION_VERSION_V2; }
    static const u8_t GetZwCmdClassId() { return COMMAND_CLASS_ASSOCIATION_V2;}
    static const String GetZwCmdClassName() { return "ASSOCIATION"; }

    virtual ValueDevice_p HandleMessage(u8_p pbCommand, u8_t byLength);

    virtual ZwMessage_p Get(u8_t byGroupId);
    virtual ZwMessage_p Set(u8_t byGroupId, u8_t byNodeId);
    virtual ZwMessage_p Set(u8_t byGroupId, Vector<Node_t> vecNodeId);
    virtual ZwMessage_p Remove(u8_t byGroupId, u8_t byNodeId);
};

typedef AssociationCmdClass  AssociationCmdClass_t;
typedef AssociationCmdClass* AssociationCmdClass_p;


#endif /* !ASSOCIATION_CMDCLASS_HPP */
