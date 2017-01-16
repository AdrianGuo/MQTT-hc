/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name:
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ
 * Revision:         1.0
 * Last Changed:     Date: 2016-11-01 12:40:00 (Tue, 01 Nov 2016)
 *
 ******************************************************************************/

#ifndef HANDLE_REQUEST_HPP_
#define HANDLE_REQUEST_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "Functor.hpp"
#include "ZwPacket.hpp"

typedef Functor1_t(ZwPacket_p)              HandlerRequestFunctor_t;
typedef HandlerRequestFunctor_t*            HandlerRequestFunctor_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/

class HandlerRequest {
    Map<u8_t, HandlerRequestFunctor_t> m_mapHandlerRequest;
    static HandlerRequest* m_instance;
    HandlerRequest() {}
public:
    static HandlerRequest* GetInstance();
    virtual ~HandlerRequest() { m_mapHandlerRequest.clear(); }
    void_t RegisterHandler(u8_t byHandleId, HandlerRequestFunctor_t funcTor);
    void_t ProcessFunctor(ZwPacket_p pZwPacket);
};

typedef HandlerRequest  HandlerRequest_t;
typedef HandlerRequest* HandlerRequest_p;

#endif /* !HANDLE_REQUEST_HPP_ */
