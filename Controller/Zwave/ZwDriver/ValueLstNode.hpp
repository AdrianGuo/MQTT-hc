/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: ValueLstNode.hpp
 *
 * Author: osboxes
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 10 Nov 2016 23:38:12
 *
 ******************************************************************************/
#ifndef VALUE_LSTNODE_HPP_
#define VALUE_LSTNODE_HPP_

#include "Typedefs.h"
#include "ZwTransportAPI.hpp"
#include "ZwNode.hpp"

class ValueLstNode {
private:
    static ValueLstNode* m_pInstance;
    ZwNode_p m_pZwNode[ZW_MAX_NODES];
    ValueLstNode();
    ValueLstNode(const ValueLstNode& rhs);
    ValueLstNode& operator= (const ValueLstNode& rhs);
public:
    static ValueLstNode& GetInstance();
    virtual ~ValueLstNode();

    ZwNode_p& operator[] (u8_t byPosion);
    const ZwNode_p& operator[] (u8_t byPosion) const;
};

typedef ValueLstNode  ValueLstNode_t;
typedef ValueLstNode* ValueLstNode_p;

#endif /* !VALUE_LSTNODE_HPP_ */
