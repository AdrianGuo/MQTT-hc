#ifndef VALUE_ZWNODE_HPP_
#define VALUE_ZWNODE_HPP_

#include "Typedefs.h"
#include "ZwTransportAPI.hpp"
#include "ZwNode.hpp"

class ValueZwNode {
private:
    ZwNode_p m_pZwNode;
public:
    ValueZwNode() { m_pZwNode = new ZwNode(); }
    ValueZwNode(ZwNode_p pZwNode) { m_pZwNode = pZwNode; }
    virtual ~ValueZwNode();

    ZwNode_p GetValue() { return m_pZwNode; }
    void_t SetValue(ZwNode_p pZwNode) { m_pZwNode = pZwNode; }
    ZwNode_p operator->() const { return m_pZwNode; }
    operator ZwNode_p()   const { return m_pZwNode; }
};

typedef ValueZwNode    ValueZwNode_t;
typedef ValueZwNode*   ValueZwNode_p;
typedef ValueZwNode_p* ValueZwNode_pp;

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
inline
ValueZwNode::~ValueZwNode() {
    if (m_pZwNode != NULL) {
        delete m_pZwNode;
        m_pZwNode = NULL;
    }
}

#endif /* !VALUE_ZWNODE_HPP_ */
