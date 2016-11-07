#ifndef VALUE_ZWNODE_HPP_
#define VALUE_ZWNODE_HPP_

#include "typedefs.h"
#include "ZwNode.hpp"
#include "Value.hpp"

class ValueZwNode : public Value {
private:
    ZwNode_p m_pZwNode;
public:
    ValueZwNode();
    ValueZwNode(ZwNode_p pZwNode);
    virtual ~ValueZwNode();

    ZwNode_p GetValue();
    void_t SetValue(ZwNode_p pZwNode);
};

typedef ValueZwNode    ValueZwNode_t;
typedef ValueZwNode*   ValueZwNode_p;
typedef ValueZwNode_p* ValueZwNode_pp;

#endif /* !VALUE_ZWNODE_HPP_ */
