#include "ValueZwNode.hpp"

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueZwNode::ValueZwNode() : Value (ValueType_ZwNode) {
    m_pZwNode = new ZwNode();
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueZwNode::ValueZwNode(
    ZwNode_p pZwNode
) : Value (ValueType_ZwNode) {
    m_pZwNode = pZwNode;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueZwNode::~ValueZwNode() {
    if (m_pZwNode != NULL) {
        delete m_pZwNode;
        m_pZwNode = NULL;
    }

}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p
ValueZwNode::GetValue() {
    return m_pZwNode;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
void_t
ValueZwNode::SetValue(
    ZwNode_p pZwNode
) {
    m_pZwNode = pZwNode;
    SetChange();
}
