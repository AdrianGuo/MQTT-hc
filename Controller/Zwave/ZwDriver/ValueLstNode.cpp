/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueLstNode.cpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 23:39:24
 *
 ******************************************************************************/
#include "ValueLstNode.hpp"

ValueLstNode_p ValueLstNode::m_pInstance = NULL;

/**
 * @func   ValueLstNode
 * @brief  None
 * @param  None
 * @retval None
 */
ValueLstNode::ValueLstNode() {
    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        m_pZwNode[i] = NULL;
    }
}

/**
 * @func   ValueLstNode
 * @brief  None
 * @param  None
 * @retval None
 */
ValueLstNode::ValueLstNode(
    const ValueLstNode& rhs
) {
    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        m_pZwNode[i] = rhs.m_pZwNode[i];
    }
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
ValueLstNode&
ValueLstNode::operator= (
    const ValueLstNode& rhs
) {
    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        m_pZwNode[i] = rhs.m_pZwNode[i];
    }
    return *this;
}

/**
 * @func   ~ValueLstNode
 * @brief  None
 * @param  None
 * @retval None
 */
ValueLstNode::~ValueLstNode() {
    for (u32_t i = 0; i < ZW_MAX_NODES; i++) {
        if (m_pZwNode[i] != NULL) {
            delete m_pZwNode[i];
            m_pZwNode[i] = NULL;
        }
    }
}

/**
 * @func   GetInstance
 * @brief  None
 * @param  None
 * @retval None
 */
ValueLstNode&
ValueLstNode::GetInstance() {
    if (m_pInstance == NULL) {
        m_pInstance = new ValueLstNode();
    }
    return *m_pInstance;
}

/**
 * @func   operator[]
 * @brief  None
 * @param  None
 * @retval None
 */
ZwNode_p&
ValueLstNode::operator[] (
    u8_t byPosion
) {
    return m_pZwNode[byPosion];
}

/**
 * @func   operator[]
 * @brief  None
 * @param  None
 * @retval None
 */
const ZwNode_p&
ValueLstNode::operator[] (
    u8_t byPosion
) const {
    return m_pZwNode[byPosion];
}
