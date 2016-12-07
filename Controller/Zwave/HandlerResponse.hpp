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

#ifndef HANDLE_RESPONSE_HPP_
#define HANDLE_RESPONSE_HPP_

#include "typedefs.h"
#include "Map.hpp"
#include "Functor.hpp"
#include "ZwPacket.hpp"

typedef Functor1_t(ZwPacket_p)              HandlerResponseFunctor_t;
typedef HandlerResponseFunctor_t*           HandlerResponseFunctor_p;

/******************************************************************************/
/*                                   CLASS                                    */
/******************************************************************************/
class HandlerResponse {
    Map<u8_t, HandlerResponseFunctor_t> m_mapHandlerResponse;
    static HandlerResponse* m_instance;
    HandlerResponse() {}
public:
    static HandlerResponse* GetInstance();
    virtual ~HandlerResponse() { m_mapHandlerResponse.clear(); }
    void_t RegisterHandler(u8_t byHandleId, HandlerResponseFunctor_t funcTor);
    void_t ProcessFunctor(ZwPacket_p pZwPacket);
};

typedef HandlerResponse  HandlerResponse_t;
typedef HandlerResponse* HandlerResponse_p;

#endif /* !HANDLE_RESPONSE_HPP_ */
